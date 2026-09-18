/// Sabre Includes
#include "sabre/syntax/tree.hpp"
#include "sabre/bytecode/visitor.hpp"
#include "sabre/lexer/buffer.hpp"
#include "sabre/relint/visitor.hpp"
#include "sabre/variable/visitor.hpp"

/// Syntax Includes
#include "sabre/syntax/annotation/qualifier.hpp"

/// Type Includes
#include "sabre/type/_inline/type.ipp"

//  CONSTRUCTORS  //

Sabre::Syntax::Tree::Tree(const $::URI::View &resource) : Tree(0, resource) {}
Sabre::Syntax::Tree::Tree(size_t capacity, const $::URI::View &resource) :
    Mixin(this), m_storage(capacity), m_resource(resource) {
  // prepare the empty return typing to be used
  auto *empty = allocate<Identifier>("Void");
  auto *returns = allocate<Qualifier>(empty);

  // construct the prototype and the signature for the tree
  auto *prototype = allocate<Constructor>();
  auto *signature = allocate<Signature>(prototype, returns);

  // finally construct the resulting instance
  m_main = allocate<Lambda>(signature, m_block = allocate<Block>());
}

Sabre::Syntax::Tree::Tree(const std::vector<Lexer::Token> &tokens, const $::URI::View &resource) :
    Tree(tokens.size() / 4, resource) {
  // update our internal bounds with first and last tokens now
  if (tokens.size()) trivia()->m_bounds = Syntax::Bounds(&tokens.front(), &tokens.back());
}

//  PUBLIC METHODS  //

SABRE_MM_CAPTURE_NODE(Tree, node, analyzer) { analyzer->visit(node->statements()); }

SABRE_MM_CHECK_NODE(Tree, node, analyzer) {
  $_ASSERT(analyzer->depth() == -1); // ensure at global-depth
  auto world = analyzer->scope();    // scope the incoming world
  $_ASSERT(analyzer->depth() == 0);  // and ensure now exporting

  // prepare our typing details that we will use
  auto result = analyzer->check(node->statements());
  auto &exports = analyzer->exports()->structure()->fields();

  auto retval = Type::New::none(); // prepare return value
  node->main()->trivia()->type() = Type::New::function(retval);

  // bind all the available exports now
  for (const auto &[name, entity] : world->entities()) {
    if (entity.exported()) exports.emplace(name, entity);
  }

  // always declare as suitably passable now
  return result.flow->passable() ? analyzer->passable(retval) : analyzer->unreachable();
}

SABRE_MM_LINT_NODE(Tree, node, analyzer) { analyzer->verify(node->statements(), node); }
SABRE_MM_LOWER_NODE(Tree, node, compiler, ) { compiler->enqueue(node->main()); }
