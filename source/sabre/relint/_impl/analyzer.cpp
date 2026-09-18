/// Sabre Includes
#include "sabre/relint/analyzer.hpp"
#include "sabre/import/service.hpp"
#include "sabre/relint/visitor.hpp"
#include "sabre/runtime/container.hpp"
#include "sabre/type/metadata.hpp"

//  CONSTRUCTORS  //

Sabre::Relint::Analyzer::Analyzer() : Analyzer($::Global::get<Runtime::Container>()) {}
Sabre::Relint::Analyzer::Analyzer(XI::Container *services) : Context(services) {}

//  PUBLIC METHODS  //

$::Unique::Pointer<Sabre::Relint::Exports>
Sabre::Relint::Analyzer::audit(const Syntax::Tree *tree, Diagnostic::Reporter *reporter) {
  // ensure that we are given a valid reporter to use
  if (reporter == nullptr) return $::Unique::New<Exports>();

  // we construct a new context instance
  $_UNUSED $_AUTO = m_using(reporter);
  m_mirrors = $::Unique::New<Exports>();
  m_scope = m_mirrors->m_references.get();

  // then we verify and move the metadata result
  return verify(tree), m_scope = nullptr, std::move(m_mirrors);
}

Sabre::Relint::Mirror *
Sabre::Relint::Analyzer::verify(const Syntax::Node *node, const Syntax::Node *parent, bool visit) {
  // ignore if this node is invalid
  if ($_UNLIKELY(node == nullptr)) return nullptr;

  // pre-construct the incoming mirror to be used now
  auto *mirror = m_mirrors->resolve(node, parent);

  // attempt visiting the node in question
  if (visit) Visitor::visit(node, this);

  // and resolve the resulting mirror now
  return traverse(node), m_finalize(mirror), mirror;
}

void Sabre::Relint::Analyzer::traverse(const Syntax::Node *node) {
  // ignore if this node is invalid
  if ($_UNLIKELY(node == nullptr)) return;

  // get the incoming rules to be handled
  auto rules = m_hooks->m_resolve(node);

  // apply all available rules to the current node in question
  for (const auto &rule : rules) rule(node, this);
}

const Sabre::Relint::Scope *Sabre::Relint::Analyzer::import(const $::String::View &path) {
  Import::Service *modules = *m_services; // resolve now
  auto result = modules->resolve(path, resource().body());
  return result.has_value() ? import(*result) : nullptr;
}

const Sabre::Relint::Scope *Sabre::Relint::Analyzer::import(const $::URI::Buffer &resource) {
  Import::Service *modules = *m_services;
  auto *found = modules->fetch(resource);
  if (found == nullptr) return nullptr;

  // always forcibly analyze the module
  found->analyze(m_services);

  // then we can safely resolve the references now
  auto *metadata = found->metadata<Module::Phase::TYPED>();
  auto *mirrors = metadata->mirrors().get();
  return mirrors ? mirrors->references() : nullptr;
}

//  PRIVATE METHODS  //

void Sabre::Relint::Analyzer::m_finalize(Mirror *mirror) {
  // get the underlying details of the mirror
  auto *origin = mirror->origin();
  auto *trivia = origin->trivia();

  // ignore if we do not have a declaration node
  if (!origin->is<Syntax::Declaration>()) return;

  /// TODO: use the traits to get bind documentation comments
  $_PP_IGN(trivia);
}

XLSP::Symbol *Sabre::Relint::Analyzer::m_bind(XLSP::Symbol &&symbol, const Syntax::Node *node) {
  // get the current context for which we should bind the symbol
  auto &siblings = m_symbol ? m_symbol->children : m_mirrors->m_symbols;

  // get the current bounds to be used
  const auto &bounds = node->trivia()->bounds();

  // update the symbol location as well for use
  symbol.range = bounds.outer().client();
  symbol.selection = bounds.inner().client();

  // resolve this symbol reference
  return &siblings.emplace_back(std::move(symbol));
}
