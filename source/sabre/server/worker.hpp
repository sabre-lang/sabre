#ifndef _SABRE_SERVER_WORKER_HPP
#define _SABRE_SERVER_WORKER_HPP

/// Sabre Includes
#include "sabre/crate/registry.hpp"
#include "sabre/forward/async.hpp"
#include "sabre/forward/server.hpp"

namespace Sabre::Server {

/// @brief Server Worker Callback.
using Callback = $::Unique::Functor<void(Worker *)>;

/// @brief The analysis refresh mode available.
enum class Refresh : uint8_t {
  STALE,   // ignore refreshing
  SINGLE,  // singule resource update
  PARTIAL, // partially update items
  ENTIRE,  // fully refresh all files
};

/// @brief Server Worker Task.
class Worker : public Async::Task<Worker> {
  //  PROPERTIES  //

  /// @brief The underlying worker callback.
  Callback m_callback;

  /// @brief Bound server connection.
  Connection *m_connection;

  /// @brief Underlying resource instance.
  $::URI::Buffer m_resource;

  /// @brief The associated crate descriptor.
  $::Unique::Pointer<Crate::Manifest> m_crate;

public:
  //  CONSTRUCTORS  //

  /**
   * @brief Constructs a anonymous server worker.
   * @param connection                Server connection.
   * @param callback                  Worker callback.
   */
  explicit Worker(Connection *connection, Callback &&callback) : Worker(connection, {}, std::move(callback)) {}

  /**
   * @brief Constructs a server worker.
   * @param connection                Server connection.
   * @param resource                  Document resource.
   * @param callback                  Worker callback.
   */
  explicit Worker(Connection *connection, const $::URI::Buffer &resource, Callback &&callback) :
      m_callback(std::move(callback)), m_connection(connection), m_resource(resource),
      m_crate(Crate::Registry::scan(m_resource)) {}

  //  PUBLIC METHODS  //

  /// @brief Gets the underlying resource value.
  inline constexpr $::URI::View resource() const noexcept { return m_resource; }

  /**
   * @brief Forces the worker to analyze all the cached documents.
   * @param mode                      Refresh mode to use.
   */
  void analyze(Refresh mode = Refresh::ENTIRE) const noexcept;

  /**
   * @brief Handles formatting a file.
   * @param options                   Format options.
   */
  std::vector<XLSP::Document::Edit> format() const noexcept;
  std::vector<XLSP::Document::Edit> format(const Format::Options &options) const noexcept;

protected:
  //  PRIVATE METHODS  //

  /// @brief Handles executing the underlying task.
  void m_execute() final { m_callback(this); }
};

} // namespace Sabre::Server

#endif
