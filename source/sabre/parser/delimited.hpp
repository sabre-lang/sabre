#ifndef _SABRE_PARSER_DELIMITED_HPP
#define _SABRE_PARSER_DELIMITED_HPP

/// Sabre Includes
#include "sabre/parser/stream.hpp"

namespace Sabre::Parser {

/// @brief Delimited List Parsing.
template <std::derived_from<Syntax::Node> T, Lexer::Kind S = Lexer::Kind::PUNC_COMMA> struct Delimited {
  //  TYPEDEFS  //

  /// @brief The underlying result typing.
  using Result = std::vector<T *>;

  /// @brief Delimited Parser Callback.
  using Callback = $::Shared::Functor<T *(Stream *) const>;

protected:
  //  PROPERTIES  //

  /// @brief Callback to be used.
  Callback m_callback;

public:
  //  CONSTRUCTORS  //

  /**
   * @brief Constructs a delimited parser.
   * @param callback              Result callback.
   */
  explicit constexpr Delimited(Callback &&callback) : m_callback(callback) {}

  //  PUBLIC METHODS  //

  /**
   * @brief Handles dispatching results.
   * @param parser                Syntax parser.
   */
  virtual inline constexpr std::optional<Result> dispatch(Stream *parser) const {
    // prepare the output list to be used
    auto result = Result();

    // attempt emplacing all the nodes now as necessary
    do { result.emplace_back(m_callback(parser)); } while (m_condition(parser));

    // and return the resulting nodes as long as they are not empty
    return std::ranges::contains(result, nullptr) ? std::nullopt : std::optional(result);
  }

protected:
  //  PRIVATE METHODS  //

  /**
   * @brief Prepare a base condition for iterating.
   * @param parser                Syntax parser.
   */
  virtual inline constexpr bool m_condition(Stream *parser) const { return parser->match(S); }
};

/// @brief Enclosed List Parsing.
template <std::derived_from<Syntax::Node> T, Lexer::Kind O, Lexer::Kind C, Lexer::Kind S = Lexer::Kind::PUNC_COMMA>
class Enclosed : public Delimited<T, S> {
  //  PROPERTIES  //

  /// @brief Trailing state.
  bool m_trailing;

public:
  //  CONSTRUCTORS  //

  /**
   * @brief Constructs an enclosed list parser.
   * @param callback              Result callback.
   * @param opening               Opening kind.
   * @param closing               Closing kind.
   * @param separator             Separator kind.
   */
  explicit constexpr Enclosed(Delimited<T, S>::Callback &&callback) :
      Delimited<T, S>(std::move(callback)), m_trailing(S == Lexer::Kind::PUNC_COMMA || S == Lexer::Kind::PUNC_TERM) {}

  //  PUBLIC METHODS  //

  /**
   * @brief Handles dispatching results.
   * @param parser                Syntax parser.
   */
  inline constexpr std::optional<typename Delimited<T>::Result> dispatch(Stream *parser) const final {
    if (!m_expect<O>(parser)) return std::nullopt; // invalid here
    if (parser->match(C)) return typename Delimited<T>::Result();

    auto result = Delimited<T, S>::dispatch(parser); // attempt running the base dispatcher
    if (!result.has_value()) return std::nullopt;    // ensure that the value return is now valid

    // if we have a non-trailing condition, then declare as such
    if (!m_validate(parser) && result->size()) parser->report(result->back(), 2000100, ";");

    // and ensure that we have a suitable closing token now
    return m_expect<C>(parser) ? result : std::nullopt;
  }

protected:
  //  PRIVATE METHODS  //

  /**
   * @brief Handles validating final delimited state.
   * @param parser                Parser stream.
   */
  inline constexpr bool m_validate(Stream *parser) const noexcept {
    if constexpr (S != Lexer::Kind::PUNC_TERM) return true;
    else return parser->previous()->kind() == Lexer::Kind::PUNC_TERM;
  }

  /**
   * @brief Prepare a base condition for iterating.
   * @param parser                Syntax parser.
   */
  inline constexpr bool m_condition(Stream *parser) const final {
    auto separator = Delimited<T, S>::m_condition(parser);
    return separator && !(m_trailing && parser->check(C));
  }

  /**
   * @brief Checks an expected value.
   * @param parser                Syntax parser.
   */
  template <Lexer::Kind K> inline constexpr bool m_expect(Stream *parser) const noexcept {
    if constexpr (K == Lexer::Kind::MISC_MAX) return true;
    else return parser->expect(K); // otherwise expect here
  }
};

} // namespace Sabre::Parser

#endif
