/// Sabre Modules
#include "sabre/bundle/codec.hpp"

//  CONSTRUCTORS  //

Sabre::Bundle::Filesystem::Filesystem(const std::vector<Image::Arena *> &arenas) {
  // reserve the size of the scripts
  scripts.reserve(arenas.size());

  // iterate over the arenas and clone them
  for (const auto *arena : arenas) {
    // pre-emplace the URI resource but do not use
    resources.emplace_back(arena->resource.buffer());

    // construct a new arena instance now
    auto *script = scripts.emplace_back($::Unique::New<Image::Arena>()).get();

    // update all the non-copyable components
    script->binary = arena->binary;
    script->strings = arena->strings;
    script->resource = arena->resource;
    script->constants = arena->constants;

    // ensure we reserve the functions sizing
    script->functions.reserve(arena->functions.size());

    // copy across the functions now
    for (const auto &function : arena->functions) {
      auto bytecode = function->bytecode(); // prepare the buffer to be copied now
      auto info = $::Unique::New<Function::Info>(script, *function->shared());
      info->bytecode() = script->binary.slice(bytecode.offset(), bytecode.size());
      script->functions.emplace_back(std::move(info)); // and emplace the function
    }
  }
}
