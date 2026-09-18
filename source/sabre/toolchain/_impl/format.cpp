/// Sabre Includes
#include "sabre/toolchain/format.hpp"
#include "sabre/diagnostic/reporter.hpp"
#include "sabre/format/pipeline.hpp"
#include "sabre/syntax/tree.hpp"
#include "sabre/toolchain/parse.hpp"

//  PUBLIC METHODS  //

Sabre::Format::Result Sabre::Toolchain::format(const Document::Buffer &document, const Format::Options &options) {
  return format(document.view(), options);
}

Sabre::Format::Result Sabre::Toolchain::format(const $::String::View &buffer, const Format::Options &options) {
  // prepare an empty reporter to be used
  auto reporter = Diagnostic::Reporter();

  // prepare the lexer options to be used
  Lexer::Options lexer = {.comments = true, .reporter = &reporter};

  // parse our tokens and our syntax-tree to be used for formatting
  auto tokens = lex(buffer, lexer);

  // fail early when there are syntax errors found
  if (reporter.failed()) return std::nullopt;

  // construct a formatting pipeline to handle our document
  return Format::Pipeline(&options).process(&tokens);
}
