#ifndef _SABRE_FORMAT_STORAGE_HPP
#define _SABRE_FORMAT_STORAGE_HPP

/// Format Includes
#include "sabre/format/_inline/pieces.ipp"

namespace Sabre::Format {

/// @brief Format Storage Container.
class Storage {
  //  PROPERTIES  //

  /// @brief The current grouping sigil.
  Sigil m_group = 0;

  /// @brief All constructed pieces.
  std::vector<$::Unique::Pointer<Piece>> m_pieces = {};

  static inline auto s_empty = Piece::Empty();   // An empty piece.
  static inline auto s_expand = Piece::Expand(); // An expansion piece.

  static inline auto s_attr = Piece::Unicode("#["); // Attribute opening.
  static inline auto s_decor = Piece::Unicode("@"); // Decorator opening.

  static inline auto s_term = Piece::Unicode(";");     // Terminator piece.
  static inline auto s_query = Piece::Unicode("?");    // Query piece.
  static inline auto s_colon = Piece::Unicode(":");    // Colon piece.
  static inline auto s_comma = Piece::Unicode(",");    // Comma piece.
  static inline auto s_assign = Piece::Unicode("=");   // Assignment piece.
  static inline auto s_period = Piece::Unicode(".");   // Period piece.
  static inline auto s_policy = Piece::Unicode("::");  // Policy piece.
  static inline auto s_spread = Piece::Unicode("..."); // Spread piece.

  static inline auto s_arrow_thin = Piece::Unicode("->"); // Lambda typing arrow.
  static inline auto s_arrow_bold = Piece::Unicode("=>"); // Lambda expression arrow.

  static inline auto s_paren_left = Piece::Unicode("(");  // Opening parenthesis.
  static inline auto s_paren_right = Piece::Unicode(")"); // Closing parenthesis.
  static inline auto s_brace_left = Piece::Unicode("{");  // Opening brace.
  static inline auto s_brace_right = Piece::Unicode("}"); // Closing brace.
  static inline auto s_brack_left = Piece::Unicode("[");  // Opening bracket.
  static inline auto s_brack_right = Piece::Unicode("]"); // Closing bracket.

  static inline auto s_space_hard = Piece::Space(Break::HARD);
  static inline auto s_space_soft = Piece::Space(Break::SOFT);

  static inline auto s_line_hard = Piece::Line(Break::HARD);
  static inline auto s_line_soft = Piece::Line(Break::SOFT);
  static inline auto s_line_literal = Piece::Line(Break::LITERAL);
  static inline auto s_line_dynamic = Piece::Line(Break::DYNAMIC);

public:
  //  CONSTRUCTORS  //

  /// @brief Constructs a defaulted set of storage.
  constexpr Storage() = default;

  //  PUBLIC METHODS  //

  /// @brief Gets the next avialable group-sigil.
  inline constexpr Sigil sigil() noexcept { return m_group++; }

  inline constexpr Piece *attr() const noexcept { return &s_attr; }
  inline constexpr Piece *decor() const noexcept { return &s_decor; }

  inline constexpr Piece *term() const noexcept { return &s_term; }
  inline constexpr Piece *empty() const noexcept { return &s_empty; }
  inline constexpr Piece *colon() const noexcept { return &s_colon; }
  inline constexpr Piece *comma() const noexcept { return &s_comma; }
  inline constexpr Piece *query() const noexcept { return &s_query; }
  inline constexpr Piece *assign() const noexcept { return &s_assign; }
  inline constexpr Piece *expand() const noexcept { return &s_expand; }
  inline constexpr Piece *period() const noexcept { return &s_period; }
  inline constexpr Piece *policy() const noexcept { return &s_policy; }
  inline constexpr Piece *spread() const noexcept { return &s_spread; }

  /// @brief Gets the available options for arrows.
  inline constexpr const auto arrow() const noexcept {
    static constexpr struct {
      Piece *thin = &s_arrow_thin;
      Piece *bold = &s_arrow_bold;
    } s_arrow_getter;
    return s_arrow_getter;
  }

  /// @brief Gets the available options for parenthesis.
  inline constexpr const auto &paren() const noexcept {
    static constexpr struct {
      Piece *left = &s_paren_left;
      Piece *right = &s_paren_right;
    } s_paren_getter;
    return s_paren_getter;
  }

  /// @brief Gets the available options for braces.
  inline constexpr const auto &brace() const noexcept {
    static constexpr struct {
      Piece *left = &s_brace_left;
      Piece *right = &s_brace_right;
    } s_brace_getter;
    return s_brace_getter;
  }

  /// @brief Gets the available options for brackets.
  inline constexpr const auto &brack() const noexcept {
    static constexpr struct {
      Piece *left = &s_brack_left;
      Piece *right = &s_brack_right;
    } s_brack_getter;
    return s_brack_getter;
  }

  /// @brief Gets the available options for spaces.
  inline constexpr const auto &space() const noexcept {
    static constexpr struct {
      Piece *hard = &s_space_hard;
      Piece *soft = &s_space_soft;
    } s_space_getter;
    return s_space_getter;
  }

  /// @brief Gets the available options for lines.
  inline constexpr const auto &line() const noexcept {
    static constexpr struct {
      Piece *hard = &s_line_hard;
      Piece *soft = &s_line_soft;
      Piece *literal = &s_line_literal;
      Piece *dynamic = &s_line_dynamic;
    } s_line_getter;
    return s_line_getter;
  }

  /**
   * @brief Handles minimizing targets.
   * @param target                    Target to compress.
   */
  inline constexpr Piece::Minimize *minimize(Piece *target) { return m_allocate<Piece::Minimize>(target); }

  /**
   * @brief Constructs a comment node.
   * @param text                      Comment text.
   */
  inline constexpr Piece *comment(const $::String::View &text) { return unicode($::Trim::both(text)); }

  /**
   * @brief Constructs a text node.
   * @param args                      Text arguments.
   */
  template <class... As> inline constexpr Piece::Unicode *unicode(As &&...args) {
    return m_allocate<Piece::Unicode>(std::forward<As>(args)...);
  }

  /**
   * @brief Constructs a decision node.
   * @param args                      Decision arguments.
   */
  template <class... As> inline constexpr Piece::Decide *decide(As &&...args) {
    return m_allocate<Piece::Decide>(std::forward<As>(args)...);
  }

  /**
   * @brief Constructs a decision node.
   * @param group                     Group to bind.
   * @param args                      Decision arguments.
   */
  template <class... As> inline constexpr Piece::Decide *decide(Piece::Group *group, As &&...args) {
    return decide(group->sigil(), std::forward<As>(args)...);
  }

  /**
   * @brief Constructs a negation node.
   * @param alternative               Alternative node.
   */
  inline constexpr Piece::Decide *negate(Piece *alternative) { return decide(empty(), alternative); }

  /**
   * @brief Constructs a negation node.
   * @param sigil                     Group to bind.
   * @param alternative               Alternative node.
   */
  inline constexpr Piece::Decide *negate(Sigil sigil, Piece *alternative) {
    return decide(sigil, empty(), alternative);
  }

  /**
   * @brief Constructs a negation node.
   * @param group                     Group to bind.
   * @param alternative               Alternative node.
   */
  inline constexpr Piece::Decide *negate(Piece::Group *group, Piece *alternative) {
    return decide(group, empty(), alternative);
  }

  /**
   * @brief Constructs a list node.
   * @param args                      List arguments.
   */
  template <class... As> inline constexpr Piece::List *list(As &&...args) {
    auto *view = m_allocate<Piece::List>(sizeof...(args));
    if constexpr (sizeof...(As)) append(view, std::forward<As>(args)...);
    return view; // and return the resulting view now as needed
  }

  /**
   * @brief Constructs an indentation node.
   * @param args                      List arguments.
   */
  template <class... As> inline constexpr Piece::Indent *indent(As &&...args) {
    auto *view = m_allocate<Piece::Indent>(sizeof...(args));
    if constexpr (sizeof...(As)) append(view, std::forward<As>(args)...);
    return view; // and return the resulting view now as needed
  }

  /**
   * @brief Constructs a grouped node.
   * @param args                      List arguments.
   */
  template <class... As> inline constexpr Piece::Group *group(As &&...args) {
    return group(sigil(), std::forward<As>(args)...);
  }

  /**
   * @brief Constructs a grouped node.
   * @param args                      List arguments.
   */
  template <class... As> inline constexpr Piece::Group *group(Sigil sigil, As &&...args) {
    auto *view = m_allocate<Piece::Group>(sigil, sizeof...(As));
    if constexpr (sizeof...(As)) append(view, std::forward<As>(args)...);
    return view; // and return the resulting view now as needed
  }

  /**
   * @brief Concatenates items onto a list.
   * @param lead                      Potential list node.
   * @param args                      Arguments to append.
   */
  template <class... As> inline constexpr Piece::List *append(Piece *lead, As &&...args) {
    auto *view = lead->is<Piece::List>() ? lead->as<Piece::List>() : list(lead);
    return m_append(view, std::forward<As>(args)...); // and append as needed
  }

private:
  //  PRIVATE METHODS  //

  /**
   * @brief Handles allocating a format node.
   * @param args                      Piece arguments.
   */
  template <std::derived_from<Piece> T, class... As> inline constexpr T *m_allocate(As &&...args) {
    return m_pieces.emplace_back($::Unique::New<T>(std::forward<As>(args)...))->template as<T>();
  }

  /**
   * @brief Concatenates items onto a list.
   * @param view                      Potential list view.
   * @param piece                     Node to be appended.
   * @param rest                      Additional arguments.
   */
  template <std::derived_from<Piece::List> T, class N, class... As>
  inline constexpr T *m_append(T *view, N &&piece, As &&...args) {
    if (piece == nullptr || piece->hash() == $::RTTI::Hash<Piece::Empty>()) $_PP_NOOP();
    else if (piece->hash() != $::RTTI::Hash<Piece::List>()) view->pieces().emplace_back(piece);
    else $::Ranges::Append(view->pieces(), piece->template as<Piece::List>()->pieces());

    if constexpr (sizeof...(As) == 0) return view; // no more parsing
    else return m_append<T, As...>(view, std::forward<As>(args)...);
  }
};

} // namespace Sabre::Format

#endif
