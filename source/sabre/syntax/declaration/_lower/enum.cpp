/// Sabre Modules
#include "sabre/bytecode/visitor.hpp"

//  PUBLIC METHODS  //

SABRE_MM_LOWER_NODE(Variant, , , ) { /** variants are type-system only */ }
SABRE_MM_LOWER_NODE(Enum, node, compiler, ) {
  // trace the incoming enumeration node now
  $_UNUSED $_AUTO = compiler->trace(node);

  // prepare the destination of the enumeration
  auto [dest, leaked] = compiler->declare(node);
  $_ASSERT(!dest.nowhere(), "Declaration does not exist");
  auto treg = leaked ? compiler->registers()->temporary() : dest;

  // prepare a suitable variables list
  auto arguments = compiler->registers()->list();

  // attempt building our values into a list
  for (const auto &variant : node->variants()) {
    // ensure we trace on each of the variants
    $_UNUSED $_AUTO = compiler->trace(variant);

    // prepare the name, label and value registers
    auto kreg = arguments.grow(), lreg = arguments.grow(), vreg = arguments.grow();

    // prepare each of our items now
    compiler->emit<Glyph::STRING_MAKE>(kreg, compiler->string(variant->key()));

    if (auto *label = variant->label()) compiler->lower(label, lreg);
    else compiler->emit<Glyph::REG_MOVE>(lreg, kreg); // copy here

    if (auto *value = variant->value()) compiler->lower(value, vreg);
    else compiler->emit<Glyph::LOAD_VOID>(vreg); // auto-build
  }

  // once complete, construct an enumeration literal
  if (arguments.empty()) compiler->emit<Glyph::ENUM_EMPTY>(treg);
  else compiler->emit<Glyph::ENUM_MAKE>(treg, arguments);

  // post-emit the expose handler
  compiler->expose(node, treg);

  // finally emit the outgoing details now
  if (leaked) compiler->emit<Glyph::STORE_CONTEXT>(dest, treg);
}
