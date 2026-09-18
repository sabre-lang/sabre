/// Sabre Includes
#include "sabre/module/archive.hpp"
#include "sabre/bytecode/metadata.hpp"
#include "sabre/image/arena.hpp"
#include "sabre/runtime/container.hpp"

//  CONSTRUCTORS  //

Sabre::Module::Archive::Archive($::Unique::Pointer<Image::Arena> &&arena) :
    Archive($::Global::get<Runtime::Container>(), std::move(arena)) {}

Sabre::Module::Archive::Archive(XI::Container *services, $::Unique::Pointer<Image::Arena> &&arena) :
    Mixin(services, arena->resource.buffer()) {
  arena->resource = resource(); // update the resource
  metadata<Phase::COMPILED>()->arena() = std::move(arena);
}
