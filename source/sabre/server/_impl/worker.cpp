/// Sabre Includes
#include "sabre/server/worker.hpp"
#include "sabre/document/service.hpp"
#include "sabre/import/service.hpp"
#include "sabre/server/connection.hpp"
#include "sabre/toolchain/format.hpp"

//  PUBLIC METHODS  //

void Sabre::Server::Worker::analyze(Refresh mode) const noexcept {
  // we always lock the service when it is being analyzed
  $_UNUSED $_AUTO = m_connection->guard();

  // declare that we are analyzing the incoming item
  $_TRACE("--| analyze: requested '{0}'", m_resource.relative());

  // attempt getting the document service
  auto *modules = m_connection->service<Import::Service>();
  auto *documents = m_connection->service<Document::Service>();

  // get the underlying modules to be analyzed now
  auto resources = documents->opened();

  // prepare the publisher to be used now
  auto *publisher = documents->publisher();

  // handle refreshes based on the incoming mode
  switch (mode) {
  // for stale mode refreshes we want to ignore handling
  case Refresh::STALE: break;

  // for singular refreshes we reset this resource
  case Refresh::SINGLE: publisher->refresh(m_resource); break;

  // for partial refreshes, we want to only update some items
  case Refresh::PARTIAL: publisher->refresh(resources); break;

  // for entire refreshes, we clear the underlying drafts
  case Refresh::ENTIRE: modules->storage()->clear(); break;
  }

  // attempt analyzing each resource as needed
  auto elapsed = $::Clock::Measure([&] { modules->analyze(resources, false); });

  // show the measured time that we found for analysis
  $_TRACE("--| analyze: elapsed time {0}", elapsed);

  // finally refresh the necessary diagnostics
  for (const auto &resource : resources) {
    auto *module = modules->fetch(resource);     // attempt getting
    if ($_UNLIKELY(module == nullptr)) continue; // failed to get
    publisher->publish(resource, module->reporter()->view());
  }
}

std::vector<XLSP::Document::Edit> Sabre::Server::Worker::format() const noexcept { return format(m_crate->format()); }
std::vector<XLSP::Document::Edit> Sabre::Server::Worker::format(const Format::Options &options) const noexcept {
  // show that a format was requested
  $_TRACE("--| format: requested '{0}'", m_resource.relative());

  // get some necessary services to be used
  auto *documents = m_connection->service<Document::Service>();

  // get the source to be used now
  auto source = documents->resolve(m_resource);

  // and attempt running the formatter now
  auto [result, elapsed] = $::Clock::Measure.apply([&] { return Toolchain::format(*source, options); });
  $_TRACE("--| format: elapsed time {0}", elapsed); // show the total elapsed time formatting

  // if the result failed, then stop handling
  if (!result.has_value()) return {};

  // count the total lines of the source
  uint32_t lines = std::ranges::count(source->view(), '\n');
  auto range = XLSP::Range(XLSP::Position(), {lines + 1, 0});

  // return the final edits to be used now
  return std::vector({XLSP::Document::Edit(*result, range)});
}
