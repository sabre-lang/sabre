/// Sabre Modules
#include "sabre/server/style.hpp"

/// Type Modules
#include "sabre/type/_inline/type.ipp"

/// Forward Declarations
$_FWD(Sabre::Server::Style, $::String::Buffer hover(const $::Notate::Block &))

//  PROPERTIES  //

/// @brief The baseline typing style.
static constexpr auto g_typing_failed = "";

/// @brief Constant notation options.
static $::Notate::Options g_notate_options = {
    .named = {"param", "generic"},
};

//  PUBLIC METHODS  //

$::String::Buffer Sabre::Server::Style::typing(const Relint::Mirror *mirror) {
  // prepare the baseline details if we having invalid details
  if ($_UNLIKELY(mirror == nullptr)) return g_typing_failed;

  // prepare the visitors to be used now
  Relint::Overloads visitors = {
      // for regular mirrors we will return the base typing
      [](const Relint::Mirror *mirror) -> Type::Erased { return mirror->type(); },

      // for certain syntax nodes, we want no output to be shown
      [](const Syntax::Import *) -> Type::Erased { return nullptr; },

      // for call operations we should lookup the function typing instead
      [](const Syntax::Call *node) -> Type::Erased { return node->callee()->trivia()->type(); },

      // for execution policies, we want to transform the return-type to be asynchronous
      [](const Syntax::Execute *node) -> Type::Erased { return Type::New::asyncify(node->callee()->trivia()->type()); },
  };

  // attempt visiting our nodes as necessary now
  auto type = mirror->visit(std::move(visitors));

  // ensure our typing is valid before continuing
  if (type == nullptr || type->is<Type::Unset, Type::Poison>()) return g_typing_failed;

  // otherwise we want to format to a string value now
  return fmt::to_string(*type);
}

$::String::Buffer Sabre::Server::Style::hover(const $::String::View &block) {
  return hover($::Notate::Parse(block, g_notate_options));
}

$::String::Buffer Sabre::Server::Style::hover(const $::Notate::Segments &segments) {
  return hover($::Notate::Parse(segments, g_notate_options));
}

$::String::Buffer Sabre::Server::Style::hover(const std::vector<$::String::Buffer> &buffers) {
  return hover($::Notate::Parse(buffers, g_notate_options));
}

$::String::Buffer Sabre::Server::Style::hover(const $::Notate::Block &block) {
  // prepare the baseline output stream
  auto oss = std::stringstream();

  // iterate over the available sections in the block
  for (const auto &[ii, section] : $::Ranges::Each(block)) {
    // render each of the required items now
    auto describe = section.description();
    auto label = $::String::Buffer(section.label());
    auto param = $::String::Buffer(section.parameter());

    // update our details depending on their existence
    if (label.size()) label = "*@" + label + '*';
    if (param.size()) param = '`' + param + '`';
    if (label.size()) describe = "&mdash; " + describe;

    // and join all our items together now with spaces
    auto merged = label + (label.size() ? " " : "") + param;
    merged += (merged.size() ? " " : "") + describe;

    // finally append this to the output stream
    oss << (ii > 0 ? "\n\n" : "") << merged;
  }

  // return the resulting hover string
  return oss.str();
}
