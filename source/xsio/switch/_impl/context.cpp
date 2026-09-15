/// Vendor Includes
#include <boost/context/detail/fcontext.hpp>

/// XSIO Includes
#include "xsio/async/manager.hpp"
#include "xsio/switch/context.hpp"
#include "xsio/task/scheduler.hpp"
#include "xsio/virtual/worker.hpp"

/// Forward Declarations
$_FWD(XSIO::Switch::Operation, Transfer jump(Pointer, void *))
$_FWD(XSIO::Switch::Operation, Pointer make(void *, size_t, void (*)(Transfer)))

//  PROPERTIES  //

/// @brief Global thread instance.
thread_local static XSIO::Virtual::Thread *g_thread = nullptr;

//  PUBLIC METHODS  //

XSIO::Switch::Transfer XSIO::Switch::Operation::jump(const Pointer to, void *vp) {
  auto transfer = boost::context::detail::jump_fcontext(to, vp);
  return {.data = transfer.data, .fctx = transfer.fctx};
}

XSIO::Switch::Pointer XSIO::Switch::Operation::make(void *sp, size_t size, void (*fn)(Transfer)) {
  // prepare a static delegate for converting our transfer types
  static auto s_delegate = [fn](boost::context::detail::transfer_t transfer) {
    fn({.data = transfer.data, .fctx = transfer.fctx}); // run callback
  };

  // and construct the result context constructor to the used now
  return boost::context::detail::make_fcontext(sp, size, [](auto transfer) { s_delegate(transfer); });
}

void XSIO::Switch::Context::wait_for_thread(Virtual::Thread *thread, Virtual::Processor *processor) {
  if (thread->m_callback) thread->m_callback(thread, processor), thread->m_callback = nullptr;
}

void XSIO::Switch::Context::yield_to_worker(Task::Scheduler *loop) {
  // get the current worker instance
  auto *thread = loop->thread();
  auto *worker = thread->m_worker;

  // update the current thread state to be waiting now
  thread->m_update(Virtual::State::RUNNING, Virtual::State::WAITING), g_thread = nullptr;

#if $_ASAN_ENABLED
  __sanitizer_start_switch_fiber(&thread->m_asan.stack, thread->m_asan.bottom, thread->m_asan.size);
#endif

  // and attempt jump function contexts
  auto transfer = Operation::jump(worker->context(), nullptr);

  // ensure some details about the context switch
  $_ASSERT(transfer.data == thread, "Failed function-context jump");
  $_ASSERT(thread->m_worker == worker, "Worker mismatch after context jump");
  $_ASSERT(worker->thread() == thread, "Scheduler mismatch after context jump");

  // successful, so update as necessary
  worker->m_context = transfer.fctx, g_thread = thread;

#if $_ASAN_ENABLED
  __sanitizer_finish_switch_fiber(thread->m_asan.stack, &thread->m_asan.bottom, &thread->m_asan.size);
#endif
}

void XSIO::Switch::Context::yield_to_scheduler(Virtual::Worker *worker) {
  auto *loop = worker->m_scheduler.load();
  loop->awaken(); // ensure woken up now

  $_ASSERT(worker->thread() == nullptr, "Worker already assigned"); // failure
  if ((worker->m_thread = loop)->m_stack == nullptr) m_allocate(loop, worker);

  loop->m_update(Virtual::State::READY, Virtual::State::RUNNING);
  loop->m_worker = worker; // update the thread worker to be used now
  loop->m_ts.compare_exchange_strong(Timer::Yield::NEVER, Timer::Point());

#if $_ASAN_ENABLED
  void *asan_stack_fake = nullptr; // prepare a fake address-sanitizer stack
  auto *asan_stack_bottom = reinterpret_cast<void *>(loop->m_stack->buffer().address());
  __sanitizer_start_switch_fiber(&asan_stack_fake, asan_stack_bottom, loop->m_stack->size());
#endif

  // attempt jumping context now
  auto transfer = Operation::jump(loop->m_context, loop);
  $_ASSERT(transfer.data == nullptr, "Failed function-context jump");

  // update all the details about the scheduler loop
  loop->m_context = transfer.fctx;
  loop->m_ts = Timer::Yield::NEVER;
  loop->m_worker = nullptr;
  worker->m_thread = nullptr;

#if $_ASAN_ENABLED
  __sanitizer_finish_switch_fiber(asan_stack_fake, nullptr, nullptr);
#endif

  $_ASSERT(loop->state() == Virtual::State::WAITING, "Scheduler thread not completed");
  $_ASSERT(worker->m_scheduler == loop, "Scheduler thread mismatch occured");
}

void XSIO::Switch::Context::yield_to_thread(Task::Scheduler *loop, Virtual::Thread *to) {
  auto *from = loop->thread(); // incoming scheduler loop thread
  $_ASSERT(!to->is<Task::Scheduler>(), "Expected a non-scheduler thread");

  auto *worker = from->m_worker; // get the worker instance now
  $_ASSERT(worker->m_scheduler == from, "Scheduler thread mismatch");
  $_ASSERT(worker->thread() == from, "Worker thread mismatch");

  worker->m_thread = to; // pre-clear the working thread details
  from->m_update(Virtual::State::RUNNING, Virtual::State::WAITING);
  from->m_ts = Timer::Yield::NEVER, from->m_worker = nullptr;

  // ensure the incoming thread has a valid stack
  if (to->m_stack == nullptr) m_allocate(to, worker);

  // set the incoming thread into a valid state for running
  to->m_update(Virtual::State::READY, Virtual::State::RUNNING);
  to->m_ts.compare_exchange_strong(Timer::Yield::NEVER, Timer::Point());
  to->m_worker = worker; // and swap the worker instance back again

#if $_ASAN_ENABLED
  void *asan_stack_fake = nullptr; // prepare a fake address-sanitizer stack
  auto *asan_stack_bottom = reinterpret_cast<void *>(to->m_stack->buffer().address());
  __sanitizer_start_switch_fiber(&asan_stack_fake, asan_stack_bottom, to->m_stack->size());
#endif

  // attempt the context jump now
  auto transfer = Operation::jump(to->m_context, to);
  $_ASSERT(transfer.data == to, "Failed function-context jump");
  to->m_context = transfer.fctx, g_thread = from; // update now

#if $_ASAN_ENABLED
  __sanitizer_finish_switch_fiber(asan_stack_fake, nullptr, nullptr);
#endif

  // re-enter scheduler thread
  $_ASSERT(worker->m_scheduler == from, "Scheduler thread mismatch");
  $_ASSERT(from->m_worker == worker, "Scheduler worker mismatch");
  $_ASSERT(worker->thread() == from, "Worker thread mismatch");

  // ensure we get the timestamp assigned in milliseconds
  from->m_ts.compare_exchange_strong(Timer::Yield::NEVER, Timer::Point());
}

void XSIO::Switch::Context::yield_to_scheduler(Virtual::Thread *thread, Virtual::State state) {
  yield_to_scheduler(thread, state, nullptr);
}

void XSIO::Switch::Context::yield_to_scheduler(Virtual::Thread *thread, Virtual::State state, Callback &&callback) {
  auto *worker = thread->m_worker;
  auto *loop = worker->m_scheduler.load();

  // run some simple assertions that should always be true
  $_ASSERT(loop->is<Task::Scheduler>(), "Expected a scheduler thread");
  $_ASSERT(!thread->is<Task::Scheduler>(), "Thread should not be a scheduler");
  $_ASSERT(thread->m_callback == nullptr, "Thread should not have a callback");
  $_ASSERT(loop->state() == Virtual::State::WAITING, "Scheduler thread not waiting");

  // attempt running our necessary details now
  thread->m_update(Virtual::State::RUNNING, state);
  thread->m_ts = Timer::Yield::NEVER, thread->m_worker = nullptr;
  thread->m_callback = std::move(callback); // set the wait-callback

  // update the scheduler details now
  loop->m_update(Virtual::State::WAITING, Virtual::State::RUNNING);
  loop->m_worker = worker, worker->m_thread = loop; // resolve worker

#if $_ASAN_ENABLED
  auto *asan_stack_fake = state >= Virtual::State::EXITED ? nullptr : &thread->m_asan.stack;
  __sanitizer_start_switch_fiber(asan_stack_fake, thread->m_asan.bottom, thread->m_asan.size);
#endif

  // attempt the context jump now
  auto transfer = Operation::jump(loop->m_context, thread);
  $_ASSERT(transfer.data == thread, "Failed function-context jump");

  // update our final details now
  loop = thread->m_worker->m_scheduler.load();
  loop->m_context = transfer.fctx, g_thread = thread;

#if $_ASAN_ENABLED
  __sanitizer_finish_switch_fiber(thread->m_asan.stack, &thread->m_asan.bottom, &thread->m_asan.size);
#endif
}

//  PRIVATE METHODS  //

void XSIO::Switch::Context::m_allocate(Virtual::Thread *thread, Virtual::Worker *worker) {
  $_ASSERT(thread->m_stack == nullptr, "Thread stack already acquired");
  auto *stack = worker->m_runtime->scheduler()->acquire<Memory::Stack>();
  $_ASSERT(stack, "Failed to allocate a thread-stack"); // ensure allocated

  thread->m_stack = stack; // update the stack value before making a function-context now
  thread->m_context = Operation::make(stack->high(), stack->size(), &m_transfer);
}

$_NORETURN void XSIO::Switch::Context::m_transfer(Transfer transfer) {
  // cast the incoming transfer data now
  auto *thread = static_cast<Virtual::Thread *>(transfer.data);

  auto *worker = thread->m_worker;         // get the worker instance
  auto *loop = worker->m_scheduler.load(); // get the scheduler instance

  // set the current thread instance
  g_thread = thread;

  // ensure we have a non-thread mismatch now
  $_ASSERT(worker->thread() == thread, "Context handler thread-mismatch");

  // if we have a scheduler thread, then run another assertion check here
  if (!thread->is<Task::Scheduler>()) loop->m_context = transfer.fctx;
  else worker->m_context = transfer.fctx, $_ASSERT(loop == thread, "Scheduler thread mismatch");

#if $_ASAN_ENABLED
  __sanitizer_finish_switch_fiber(nullptr, &thread->m_asan.bottom, &thread->m_asan.size);
#endif

  // and run the handler now to be used
  thread->m_execute(), thread->exit();
}
