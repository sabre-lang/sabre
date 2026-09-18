#ifndef _SABRE_DOCUMENT_PUBLISHER_HPP
#define _SABRE_DOCUMENT_PUBLISHER_HPP

/// Sabre Includes
#include "sabre/forward/document.hpp"
#include "sabre/forward/import.hpp"
#include "sabre/forward/server.hpp"

namespace Sabre::Document {

/// @brief Document Publisher Service.
class Publisher : public XI::Transient {
  //  PROPERTIES  //

  /// @brief Available services container.
  XI::Container *m_services = nullptr;

  /// @brief Server connection instance.
  Server::Connection *m_connection = nullptr;

public:
  //  CONSTRUCTORS  //

  /**
   * @brief Constructs a publisher.
   * @param connection              Server connection.
   */
  explicit Publisher();
  explicit Publisher(XI::Container *services);

  //  PUBLIC METHODS  //

  /**
   * @brief Handles clearing document diagnostics.
   * @param resource                  Document resource.
   */
  void clear(const $::URI::View &resource);

  /**
   * @brief Forces an update event to occur.
   * @param resource                  Document resource.
   */
  void refresh(const $::URI::View &resource);
  void refresh(const std::vector<$::URI::View> &resources);
  void refresh(const std::vector<$::URI::Buffer> &resources);

  /**
   * @brief Handles publishing document diagnostics.
   * @param resource                  Document resource.
   * @param diagnostics               Diagnostics to publish.
   */
  void publish(const $::URI::View &resource, const std::vector<XLSP::Diagnostic> &diagnostics);

private:
  //  PRIVATE METHODS  //

  /// @brief Gets the underlying modules service.
  Import::Service *m_modules() noexcept;
};

} // namespace Sabre::Document

#endif
