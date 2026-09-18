/// Sabre Modules
#include "sabre/bundle/archive.hpp"
#include "sabre/bundle/codec.hpp"
#include "sabre/bytecode/metadata.hpp"
#include "sabre/import/service.hpp"

//  PRIVATE METHODS  //

XJCT::Blob::Bytes Sabre::Bundle::Archive::m_encode(Import::Service *modules) const noexcept {
  // ensure we have a set of services as well
  $_ASSERT(!headless(), "Encoding modules archives requires a services container");

  // prepare a list of futures for compilation
  auto futures = std::vector<std::future<Image::Arena *>>();

  // attempt compiling each of the incoming files now
  for (auto &module : modules->storage()->each()) {
    if (!module->is<Module::Script>()) continue;
    futures.emplace_back(m_request(module.get()));
  }

  // prepare the outgoing filesystem instance
  auto arenas = std::vector<Image::Arena *>();
  arenas.reserve(futures.size()); // pre-reserve

  // wait for all the futures to be completed
  for (auto &future : futures) arenas.emplace_back(future.get());

  // finally encode the incoming filesystem now
  return encode(Filesystem(arenas));
}

Sabre::Image::Arena *Sabre::Bundle::Archive::m_compile(Module::Abstract *module) const noexcept {
  return module->compile(m_services), module->metadata<Module::Phase::COMPILED>()->arena().get();
}

std::future<Sabre::Image::Arena *> Sabre::Bundle::Archive::m_request(Module::Abstract *module) const noexcept {
  return std::async(std::launch::async, &Archive::m_compile, this, module);
}
