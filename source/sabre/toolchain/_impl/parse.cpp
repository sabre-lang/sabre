/// Sabre Includes
#include "sabre/toolchain/parse.hpp"
#include "sabre/diagnostic/reporter.hpp"
#include "sabre/lexer/dispatch.hpp"
#include "sabre/parser/dispatch.hpp"
#include "sabre/parser/stream.hpp"
#include "sabre/syntax/tree.hpp"
#include "sabre/variable/analyzer.hpp"

//  PUBLIC METHODS  //

Sabre::Lexer::Buffer Sabre::Toolchain::lex(const Document::Buffer &document, const Lexer::Options &options) {
  return lex(document.view(), options);
}

Sabre::Lexer::Buffer Sabre::Toolchain::lex(const $::String::View &buffer, const Lexer::Options &options) {
  auto tokens = Lexer::Buffer(options.comments);
  auto lexer = Lexer::Scanner(buffer, options);
  return Lexer::Dispatch::next(lexer, tokens), tokens;
}

$::Unique::Pointer<Sabre::Syntax::Tree>
Sabre::Toolchain::parse(const Lexer::Buffer *tokens, Diagnostic::Reporter *reporter) {
  // prepare a suitable tree and parser to be used
  auto tree = $::Unique::New<Syntax::Tree>(tokens->buffer(), reporter->resource());
  auto parser = $::Unique::New<Parser::Stream>(tokens, tree.get(), reporter);

  // attempt parsing the tree then ensure we also visit marks
  Parser::Dispatch::stream(parser.get(), tree->statements());
  return Variable::Analyzer().visit(tree.get()), std::move(tree);
}
