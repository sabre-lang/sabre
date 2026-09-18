/// Format Includes
#include "sabre/format/_inline/macros.ipp"

//  PRIVATE METHODS  //

SABRE_MM_FORMAT_PREFIX(Lambda, reader) {
  // attempt parsing the incoming signature value
  auto *signature = m_signature(reader);
  if (signature == nullptr) return nullptr;

  // prepare the piece storage/builder
  auto *options = reader->options();
  auto *storage = reader->storage();

  // prepare the leading header group
  auto *header = storage->group(signature);

  // attempt parsing the incoming body now
  auto *body = [&] -> Piece * {
    if (reader->check(Lexer::Kind::PUNC_LBRACE)) return m_block(reader, header);
    if (!reader->match(Lexer::Kind::ARROW_BOLD)) return nullptr; // invalid token

    // prepare the leading comments to be used now
    auto callback = [](Reader *reader) { return m_expression(reader); };

    // attempt consuming the incoming expression now
    auto *expression = m_leading(reader, Callback(callback));
    if (expression == nullptr) return nullptr; // failed here

    // emplace the leading details for the arrow expression (must be within the signature)
    storage->append(header, storage->space().hard, storage->arrow().bold);

    auto *style = options->arrows_align ? storage->line().dynamic : storage->line().hard;
    return storage->indent(style, expression); // prepare indentation for our expression
  }();

  // and construct the resulting lambda to be shown
  return body ? storage->group(header, body) : nullptr;
}
