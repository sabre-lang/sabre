/// Sabre Modules
#include "sabre/server/connection.hpp"
#include "sabre/server/style.hpp"

//  EVENT METHODS  //

void Sabre::Server::Events::on_document_hover(Request::DOCUMENT_HOVER request) {
  // destructure the incoming items to be used
  auto cursor = request.params();
  auto position = cursor.position;
  auto resource = cursor.identifier.resource;

  // show that a message was requested
  $_TRACE("--| hover: requested '{0}:{1}'", resource.relative(), position);

  // prepare an empty response to be returned
  auto response = request.prepare();

  // start by resolving the node at the resource/position now
  if (auto *source = m_utilities->syntax_node_at(resource, position)) {
    // trace what we actually found for debug viewing
    $_TRACE("--| hover: found node '{0}' at {1}", source->canonical(), source->range());

    // prepare the properties we use for hover outputs
    auto typing = Style::typing(source);

    // prepare all of the output segments to be used
    auto sections = std::vector<$::String::Buffer>();

    // prepare hover response details now
    response.range = source->range().client();
    response.contents = XLSP::Markup::Formatted();

    // attempt binding the current typing to the top of the value
    if (typing.size()) sections = XLSP::Markup::Code(typing, SABRE_MM_IDENTIFIER);

    // bind a suitable declaration comments if necessary
    if (auto comments = m_utilities->comments_view_at(source)) $::Ranges::Append(sections, comments.span());

    // otherwise bind the node comments last since it rounds out the details
    else if ((comments = source->comments())) $::Ranges::Append(sections, comments.span());

    // and finally write each of our sections to the response
    response.contents.stream() << Style::hover(sections);
  }

  // and ensure we reply as expected now
  request.reply(std::move(response));
}
