/// Sabre Includes
#include "sabre/bytecode/optimizer.hpp"
#include "sabre/runtime/container.hpp"

//  CONSTRUCTORS  //

Sabre::Bytecode::Optimizer::Optimizer(Binder *labels) : Optimizer($::Global::get<Runtime::Container>(), labels) {}
Sabre::Bytecode::Optimizer::Optimizer(XI::Container *services, Binder *labels) : Optimizer(labels, *services) {}
Sabre::Bytecode::Optimizer::Optimizer(Binder *labels, const Runtime::Options *options) :
    m_labels(labels), m_options(options) {}

//  PUBLIC METHODS  //

void Sabre::Bytecode::Optimizer::process(Routine *routine) const {
  // prepare the current callable for optimization
  if (!m_options->flags.optless) {
    m_remove_dead_ir(routine);      // start by removing dead ir
    m_remove_empty_blocks(routine); // then removing any empty blocks
  }

  // build the control-flow graph
  m_build_graph(routine);

  // and apply some optimization techniques
  if (!m_options->flags.optless) {
    m_rewrite_chained_branches(routine); // fixes chained branches
    m_remove_dead_blocks(routine);       // removes any now dead blocks
    m_remove_useless_jumps(routine);     // removes useless coinciding jumps
  }

  // allow dumping graphs when desired
  if (m_options->dump.graphs) m_dump_graph(routine);
}

//  PRIVATE METHODS  //

void Sabre::Bytecode::Optimizer::m_build_graph(Routine *routine) const {
  // pre-ensure we are not at the end before continuing
  if (routine->blocks.empty()) return;

  // get the underlying details now
  auto iter = routine->blocks.begin();
  auto end = routine->blocks.end();

  // available labelled blocks
  const auto &labelled = m_labels->blocks();

  // attempt updating whilst possible to do so
  while (iter != end) {
    // get the current block value now
    auto *block = iter->get();
    auto next = std::next(iter);

    // empty blocks fall through to next block
    if (block->empty() && next != end) {
      block->link(next->get()), iter = next;
      continue; // should continue immediately
    }

    // get the last-most instruction now
    auto *last = block->instructions().back().get();

    // handle based on the last operation code now
    switch (last->glyph().encoded()) {
    // jumps require additional handling
    case Glyph::JUMP_TO: {
      auto jump = last->cast<Glyph::JUMP_TO>();
      block->link(labelled.at(jump->get<0>()));
    } break;

    // all templated jumps can use the base cast
    case Glyph::JUMP_FILLED: $_FALLTHROUGH;
    case Glyph::JUMP_FALSEY: $_FALLTHROUGH;
    case Glyph::JUMP_TRUTHY: $_FALLTHROUGH;
    case Glyph::MATCH_VOID: $_FALLTHROUGH;
    case Glyph::MATCH_TRUE: $_FALLTHROUGH;
    case Glyph::MATCH_FALSE: $_FALLTHROUGH;
    case Glyph::MATCH_CONST: $_FALLTHROUGH;
    case Glyph::MATCH_TEXT: $_FALLTHROUGH;
    case Glyph::MATCH_GUARD: {
      auto jump = last->cast<Glyph::JUMP_TPL>(); // use the template
      block->link(labelled.at(jump->get<0>())), block->link(next->get());
    } break;

    // these instructions do not directly link to other blocks
    case Glyph::EXEC_ABORT: $_FALLTHROUGH;
    case Glyph::EXEC_PANIC: $_FALLTHROUGH;
    case Glyph::EXEC_RETURN: break;

    // regular instructions should link to next block
    default: {
      if (next != end) block->link(next->get());
    } break;
    }

    // update the current iterator now
    iter = next;
  }
}

void Sabre::Bytecode::Optimizer::m_dump_graph(Routine *routine) const {
  $::Debug::println("\n===== CFG / {0} =====", fmt::ptr(routine->shared));
  if (!routine->blocks.empty()) $::Debug::println(); // stop if needed

  // attempt dumping each basic-block of the function now
  for (const auto &block : routine->blocks) $::Debug::println(*block);
}

void Sabre::Bytecode::Optimizer::m_remove_dead_ir(Routine *routine) const {
  // prepare the predicate to be used here
  static auto predicate = [](const auto &instruction) { return instruction->terminates(); };

  // and iteratively remove dead instructions now
  for (auto &block : routine->blocks) {
    auto &instructions = block->instructions();
    auto begin = instructions.begin(), end = instructions.end();
    auto iter = std::ranges::find_if(begin, end, predicate);
    if (iter != end) instructions.erase(std::next(iter), end);
  }
}

void Sabre::Bytecode::Optimizer::m_remove_empty_blocks(Routine *routine) const {
  // prepare the blocks to be resolved now
  auto &blocks = routine->blocks;
  auto iter = blocks.begin(), end = blocks.end();

  // prepare the details now for labelled blocks
  auto &labelled = m_labels->blocks();

  // prepare the removal process now
  while (std::next(iter) != end && std::next(iter)->get() != nullptr) {
    // get the incoming current block and next block
    auto *block = iter->get(), *next = std::next(iter)->get();

    // only remove if the current block is empty
    auto removable = block->empty();

    // handle update labels now if necessary
    if (auto labels = block->labels(); removable && labels.size()) {
      for (const auto &label : labels) labelled.insert_or_assign(label, next);
      next->labels().insert(labels.begin(), labels.end()); // move across
    }

    // and update as necessary for our details
    removable ? block->unlink(), iter = blocks.erase(iter) : ++iter;
  }
}

void Sabre::Bytecode::Optimizer::m_rewrite_chained_branches(Routine *routine) const {
  // iterate over all the available blocks
  for (auto &block : routine->blocks) {
    // get the underlying instructions
    auto &instructions = block->instructions();

    // skip all empty blocks
    if (instructions.empty()) continue;

    // jump instructions can sometimes be rewritten
    auto &last = instructions.back();
    if (last->glyph() != Glyph::JUMP_TO) continue;

    // get the target block and catch infinite loops
    auto target = *block->outgoing().begin();
    if (target == block.get()) continue;

    // check if the target block contains a single operation
    if (target->instructions().size() != 1) continue;

    // get the target blocks only item now
    auto *compare = target->instructions().front().get();

    // only handle some comparable opcodes
    switch (compare->glyph().encoded()) {
    // replace source jumps with return now
    case Glyph::EXEC_RETURN:
      block->unlink(target); // unlink the incoming target now
      last = $::Unique::New<Instruction>(Glyph::EXEC_RETURN);
      break;

    // ignore all other opcodes
    default: break;
    }
  }
}

void Sabre::Bytecode::Optimizer::m_remove_dead_blocks(Routine *routine) const {
  // get the list of available blocks now
  auto &blocks = routine->blocks;
  std::list<Block *> reachable = {};

  // push the initial block when we have one
  if (blocks.size()) reachable.push_back(blocks.front().get());

  // mark all reachable blocks
  while (!reachable.empty()) {
    auto block = reachable.front();
    reachable.pop_front(); // and pop now

    // check if the block has been reached yet
    if (block->reachable()) continue;

    // dclare a currently reachable now
    block->reachable() = true;

    // update the reachable blocks to be checked now
    for (const auto &outgoing : block->outgoing()) reachable.push_back(outgoing);
  }

  // delete all non-reachable blocks
  for (auto iter = blocks.cbegin(); iter != blocks.cend();) {
    // get the block instance now
    auto &block = *iter;

    if (block->reachable()) ++iter; // skip if reachable
    else block->unlink(), iter = blocks.erase(iter);
  }
}

void Sabre::Bytecode::Optimizer::m_remove_useless_jumps(Routine *routine) const {
  for (auto iter = routine->blocks.begin(); std::next(iter) != routine->blocks.end(); ++iter) {
    // prepare some details to be handled
    auto &block = *iter, &next = *std::next(iter);
    auto &instructions = block->instructions();

    // ignore if the block is currently empty
    if (instructions.empty()) continue;

    // and handle the incoming jumps as necessary
    switch (auto *last = instructions.back().get(); last->glyph().encoded()) {
#define SABRE_XX_GLYPH_JUMP(N, ...)                                       \
  case Glyph::N: {                                                        \
    auto *jump = last->cast<Glyph::N>();                                  \
    if (next->labels().contains(jump->get<0>())) instructions.pop_back(); \
  } break;
#include "sabre/bytecode/_defines/glyphs.def"
    default: break;
    }
  }
}
