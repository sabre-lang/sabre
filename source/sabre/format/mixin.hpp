#ifndef _SABRE_FORMAT_MIXIN_HPP
#define _SABRE_FORMAT_MIXIN_HPP

/// Sabre Includes
#include "sabre/format/wrap.hpp"

//  X-MACROS  //

#define XX_FORMAT_NODES(X) \
  X(Line, class)           \
  X(Space, class)          \
  X(Empty, struct)         \
  X(Unicode, class)        \
                           \
  X(List, class)           \
  X(Group, class)          \
  X(Indent, struct)        \
                           \
  X(When, class)           \
  X(Decide, class)         \
  X(Expand, struct)        \
  X(Minimize, class)

//  NAMESPACES  //

namespace Sabre::Format {

/// @brief Line/Space Break Typing.
enum class Break : uint8_t {
  SOFT,    // soft break
  HARD,    // hard break
  DYNAMIC, // dynamic line
  LITERAL, // literal line
};

/// @brief Format Piece Base.
struct Piece : public $::RTTI::Dynamic {
  //  TYPEDEFS  //

#define X(N, D, ...) D N;
  XX_FORMAT_NODES(X);
#undef X

  /// @brief Format Piece Abstraction.
  template <class T, std::derived_from<Piece> B = Piece> using Mixin = $::RTTI::Mixin<T, B>;

  //  CONSTRUCTORS  //

  /// @brief Defaulted node constructor.
  explicit Piece() = default;

  /// @brief Virtual abstract destructor.
  virtual ~Piece() = default;

  //  PUBLIC METHODS  //

  /// @brief Gets the most derived piece hash.
  inline constexpr $::RTTI::Tag hash() const noexcept { return m_hash(); }

  /// @brief Downcasts nodes to derived types.
  template <std::derived_from<Piece> T> inline constexpr T *as() noexcept { return $::RTTI::Cast<T>(this); }
  template <std::derived_from<Piece> T> inline constexpr const T *as() const noexcept {
    return $::RTTI::Cast<const T>(this);
  }

  /**
   * @brief Gets the width of a node.
   * @param wrapped               Wrapped groups.
   * @param force                 Forced width flag.
   */
  inline constexpr size_t width(const Wrap::List &wrapped = {}, bool force = false) const noexcept {
    return m_width(wrapped, force);
  }

  /**
   * @brief Handles writing a node.
   * @param writer                Format writer.
   * @param wrapping              Current wrapping.
   */
  inline constexpr void render(Writer *writer, Wrap wrapping = Wrap::State::DETECT) const noexcept {
    m_render(writer, wrapping);
  }

protected:
  //  PRIVATE METHODS  //

  /**
   * @brief Handles writing a node.
   * @param writer                Format writer.
   * @param wrapping              Current wrapping.
   */
  virtual void m_render(Writer *write, Wrap wrapping) const noexcept = 0;

  /**
   * @brief Gets the width of a node.
   * @param wrapped               Wrapped groups.
   * @param force                 Forced width flag.
   */
  virtual size_t m_width(const Wrap::List &wrapped, bool force) const noexcept = 0;

  /**
   * @brief Gets the width of multiple nodes.
   * @param nodes                 Nodes to revise.
   * @param wrapped               Wrapped groups.
   * @param force                 Forced width flag.
   */
  inline constexpr size_t m_apply(const Buffer &nodes, const Wrap::List &wrapped, bool force) const noexcept {
    // prepare the baseline accumulator to be used
    size_t acc = 0;

    // iterate over the available nodes to be used
    for (const auto *node : nodes) {
      acc = Math::Saturating::add(acc, node->width(wrapped, force));
      if (acc == SIZE_MAX) return SIZE_MAX; // stop early now
    }

    // and return the final accumulator now
    return acc;
  }
};

} // namespace Sabre::Format

//  UNDEFINES  //

#undef XX_FORMAT_NODES

#endif
