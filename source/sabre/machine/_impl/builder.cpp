/// Sabre Includes
#include "sabre/machine/builder.hpp"
#include "sabre/machine/frame.hpp"

/// Machine Includes
#include "sabre/machine/_inline/glue.ipp"

//  PRIVATE METHODS  //

void Sabre::Machine::Builder::m_prolog() {
  // construct the underlying function
  auto *node = XASM::Function::Build<Reference, Runtime::Isolate *, const Frame *, const Value::Any &>(compiler);

  // bind all the incoming base registers to be used
  isolate = compiler->new_gpz();
  feedback = compiler->new_gpz();
  frame = compiler->new_gpz();
  params = compiler->new_gpz();
  argv = compiler->new_gpz();
  envp = compiler->new_gpz();

  // prepare the incoming frame details
  auto stack = compiler->new_gpz();

  // register the incoming accumulator as well now
  result = registers->allocate(Register::Accumulator);

  // bind all the incoming labels necessary
  panic = compiler->new_named_label("_panic");
  interrupt = compiler->new_named_label("_interrupt");

  // declare that we are setting up the function
  logger->header("-- Function Prolog --");

  // then we want to bind our arguments now
  node->set_arg(0, isolate);
  node->set_arg(1, frame);
  node->set_arg(2, stack);

  // preload the frame pointer for references to the environment, args and stack
  if (facts.any()) logger->spacing();

  // panic offset are held at the top of the stack
  if (facts.panics()) compiler->mov(feedback, stack);

  // ensure we pre-load the incoming context, arguments and stack
  if (facts.envp()) compiler->load_u64(envp, emitter->mem(stack, Offset::STK_ENVP * sizeof(Value::Any)));
  if (facts.params()) compiler->load_u64(params, emitter->mem(stack, Offset::STK_PASS * sizeof(Value::Any)));
  if (facts.argv()) compiler->load_u64(argv, emitter->mem(stack, Offset::STK_DATA * sizeof(Value::Any)));

  // declare that we are now in the user-code
  logger->header("-- Function Code --");
}

void Sabre::Machine::Builder::m_epilog() {
  // show that we are emitting exceptions
  logger->header("--  Function Exceptions --\n");

  // bind the fallback return condition
  emitter->ret(Constants::Void);

  // bind the optimizable interrupt condition
  compiler->bind(interrupt), emitter->call(Glue::cancel, isolate);

  // bind the optimizable panic condition
  compiler->bind(panic), emitter->ret(Constants::Fail);

  // and annotate the incoming function epilog
  logger->header("-- Function Epilog --\n");
}
