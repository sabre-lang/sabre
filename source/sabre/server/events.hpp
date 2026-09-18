#ifndef _SABRE_SERVER_EVENTS_HPP
#define _SABRE_SERVER_EVENTS_HPP

/// Sabre Includes
#include "sabre/forward/document.hpp"
#include "sabre/forward/server.hpp"

//  X-MACROS  //

#define X(C, ...) $_FWD(Sabre::Server::Request, using C = XLSP::Request::Handle<XLSP::Request::Channel::C>)
XLSP_XX_REQUEST_CHANNELS(X)
#undef X

#define X(C, ...)                                                                                          \
  $_FWD(Sabre::Server::Notification, using C = XLSP::Notification::Params<XLSP::Notification::Channel::C>)
XLSP_XX_NOTIFICATION_CHANNELS(X)
#undef X

//  NAMESPACES  //

namespace Sabre::Server {

/// @brief Lifecycle Handlers.
class Events : public XI::Transient {
  //  PROPERTIES  //

  /// @brief Attached connection instance.
  Connection *m_connection;

  /// @brief Available helper utilities.
  const Utilities *m_utilities;

  /// @brief Bound documents service.
  Document::Service *m_documents;

public:
  //  CONSTRUCTORS  //

  /**
   * @brief Constructs a connection instance.
   * @param services              Services container.
   * @param connection            Explicit connection.
   */
  explicit Events();
  explicit Events(XI::Container *services);
  explicit Events(XI::Container *services, Connection *connection);

  //  PUBLIC METHODS  //

  void on_initialize(Request::LIFECYCLE_INIT request);
  void on_shutdown(Request::LIFECYCLE_SHUTDOWN request);

  void on_document_opened(const Notification::DOCUMENT_OPENED &params);
  void on_document_closed(const Notification::DOCUMENT_CLOSED &params);
  void on_document_changed(const Notification::DOCUMENT_CHANGED &params);
  void on_workspace_watcher(const Notification::WORKSPACE_FILE_CHANGED &params);

  void on_document_hover(Request::DOCUMENT_HOVER request);
  void on_document_links(Request::DOCUMENT_LINKS request);
  void on_document_format(Request::DOCUMENT_FORMAT request);
  void on_document_symbols(Request::DOCUMENT_SYMBOLS request);
  void on_document_vardef(Request::DOCUMENT_VARDEF request);
  void on_document_typedef(Request::DOCUMENT_TYPEDEF request);
  void on_document_completes(Request::DOCUMENT_COMPLETIONS request);
  void on_document_references(Request::DOCUMENT_REFERENCES request);

private:
  //  PRIVATE METHODS  //

  /// @brief Allows ignoring incoming notifications.
  template <Channel C> inline constexpr void m_ignore(const XLSP::Notification::Params<C> &) {}

  /// @brief Allows ignoring incoming requests.
  template <Channel C, bool Cancel = true>
  inline constexpr void m_ignore(XLSP::Request::Handle<C, XLSP::Event::Answer> handle) {
    if constexpr (!Cancel) handle.reply($::Serde::Null());
    else handle.reply(XLSP::Error::Code::SERVER_CANCELLED);
  }
};

} // namespace Sabre::Server

#endif
