/// Sabre Includes
#include "sabre/relint/visitor.hpp"

//  PUBLIC METHODS  //

SABRE_MM_LINT_NODE(Lambda, node, analyzer) {
  // prepare the scoping to be used for this lambda
  $_UNUSED $_AUTO = analyzer->scope();

  // bind this declaration now as a suitable symbol
  $_UNUSED $_AUTO = analyzer->symbol("<function>", node, XLSP::Symbol::Kind::FUNCTION);

  // always pre-verify the signature for the lambda
  analyzer->verify(node->signature(), node);

  // and then verify the resulting body
  analyzer->verify(node->body(), node);
}
