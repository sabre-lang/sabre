/// XSIO Includes
#include "xsio/task/scheduler.hpp"
#include "xsio/async/manager.hpp"
#include "xsio/virtual/worker.hpp"

//  PRIVATE METHODS  //

void XSIO::Task::Scheduler::m_execute() {
  // attempt running whilst possible to do so
  while (m_runtime->running()) {
    // get the current worker and processor
    auto *worker = m_thread->worker();
    auto *processor = worker->processor();

    // fire any potential timer events now
    processor->timers()->fire(m_thread, processor);

    // get the next available thread to schedule
    auto *next = processor->acquire();

    // get the next available thread now
    if (next == nullptr) m_busy(processor);
    else m_acquired(next, processor);
  }

  // if we reach here, then we need to exit safely
  Switch::Context::yield_to_worker(this), $::System::unreachable();
}

void XSIO::Task::Scheduler::m_acquired(Virtual::Thread *thread, Virtual::Processor *processor) {
  // get the current scheduler instance
  auto *scheduler = m_runtime->scheduler();

  // ensure some details about the thread now
  $_ASSERT(thread->worker() == nullptr, "Thread already assigned");
  $_ASSERT(thread->state() == Virtual::State::READY, "Thread not ready");

  // yield to the thread to begin running
  Switch::Context::yield_to_thread(this, thread);

  // handle all the outcome thread states
  switch (auto state = thread->state()) {
  case Virtual::State::READY: scheduler->schedule(thread); break;
  case Virtual::State::EXITED: scheduler->recycle(thread); break;
  case Virtual::State::WAITING: Switch::Context::wait_for_thread(thread, processor); break;

  default: {
    $_ASSERT(state >= Virtual::State::SHUTDOWN, "Unexpected thread state!");
    auto exit_code = static_cast<int32_t>(state) - static_cast<int32_t>(Virtual::State::SHUTDOWN);
    $_ASSERT(exit_code > -1, "Invalid shutdown state '{0}'", static_cast<uint8_t>(state));
    scheduler->recycle(thread), m_runtime->exit(exit_code); // force an exit to occur now
  } break;
  }
}

void XSIO::Task::Scheduler::m_busy(Virtual::Processor *processor) {
  auto now = Timer::Point();
  auto next = processor->timers()->next();

  // handle various results of the next value
  if (next == Timer::Ticks()) return Switch::Context::yield_to_worker(this);

  // attempt running a native section now to stall the loop
  if (next > now) m_thread->native([&] { m_runtime->shutdown()->wait(next - now); });
}
