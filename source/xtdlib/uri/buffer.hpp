#ifndef _XTDLIB_URI_BUFFER_HPP
#define _XTDLIB_URI_BUFFER_HPP

/// Library Includes
#include "xtdlib/serde/value.hpp"
#include "xtdlib/uri/scheme.hpp"

namespace $::URI {

/// @brief URI components of the form "<scheme>:[//<authority>]<body>".
class Buffer {
  //  TYPEDEFS  //

  /// @brief Allow views internal access.
  friend class View;

  /// @brief Allows parsing buffers.
  friend Buffer Parse(const String::View &);

  //  PROPERTIES  //

  /// @brief Starting index of the body.
  size_t m_offset = 0;

  /// @brief Encompasing buffer of the URI.
  String::Buffer m_buffer = {};

public:
  //  CONSTRUCTORS  //

  /// @brief Constructs an anonymous buffer instance.
  constexpr Buffer() : Buffer(*m_anonymous()) {}

  /**
   * @brief Constructs a URI from given details.
   * @param scheme                       Scheme value.
   * @param authority                    Authority value.
   * @param body                         Body value.
   */
  constexpr Buffer(const String::View &body) : Buffer(Scheme::FILE, body) {}
  constexpr Buffer(const String::View &scheme, const String::View &body) : Buffer(scheme, {}, body) {}
  constexpr Buffer(const String::View &scheme, const String::View &authority, const String::View &body) {
    String::Buffer slashes = (authority.size() || scheme == Scheme::FILE) ? "//" : "";
    m_offset = 1 + scheme.size() + slashes.size() + authority.size(); // set the offset to be used
    m_buffer = String::Buffer(scheme) + ':' + slashes + String::Buffer(authority) + m_normalize(body);
  }

  //  OPERATOR METHODS  //

  /// @brief Checking URI's is as simple as checking equality of the buffer.
  inline constexpr bool operator==(const Buffer &other) const noexcept { return m_buffer == other.m_buffer; }

  //  PUBLIC METHODS  //

  /// @brief Gets the internal buffer value as a view.
  inline constexpr String::View view() const noexcept { return m_buffer; }

  /// @brief Gets the internal raw buffer value.
  inline constexpr String::Buffer raw() const noexcept { return m_buffer; }

  /// @brief Checks if a buffer is anonymous.
  inline constexpr bool anonymous() const noexcept { return m_buffer == m_anonymous()->m_buffer; }

  /// @brief Gets the underlying scheme value.
  inline constexpr String::View scheme() const noexcept { return view().substr(0, m_delimiter()); }

  /// @brief Gets the underlying authority value.
  inline constexpr String::View authority() const noexcept {
    auto value = view().substr(0, m_offset);
    value = value.substr(m_delimiter() + 1);
    return value.starts_with("//") ? value.substr(2) : value;
  }

  /// @brief Gets the underlying body value.
  inline constexpr String::View body() const noexcept {
    auto value = m_trailing();

#if $_PLATFORM_WINDOWS
    if (scheme() != Scheme::FILE || !value.starts_with('/') || value.size() < 2) return value;
    return value.at(2) == ':' && std::isalpha(view().at(1)) ? value.substr(1) : value;
#else
    return value;
#endif
  }

  /**
   * @brief Gets a relative path from the given body.
   * @param hint                      Directory hint to use.
   */
  inline constexpr FS::Path relative(const FS::Path &hint = Path::current()) const {
    if (scheme() != Scheme::FILE) return m_buffer;
    return Path::relative(body(), hint); // resolve
  }

protected:
  //  PROTECTED METHODS  //

  /**
   * @brief Handles encoding a resource value.
   * @param self                      Resource to be encoded.
   */
  static Serde::Value m_encode(const Buffer &self);

  /**
   * @brief Handles decoding a resource value.
   * @param value                     Value to decode.
   */
  static Buffer m_decode(const Serde::Value &value);

  /**
   * @brief Dumps the current resource value.
   * @param os                        Output stream.
   * @param self                      URI instance.
   */
  static inline void m_print(std::ostream &os, const Buffer &self) { os << self.m_buffer; }

private:
  //  PRIVATE METHODS  //

  /// @brief Gets the ":" delimiter index.
  inline constexpr size_t m_delimiter() const noexcept {
    auto index = m_buffer.find_first_of(':');
    return $_ASSERT(index != String::Term), index;
  }

  /// @brief Gets the unmodified trailing path.
  inline constexpr String::View m_trailing() const noexcept {
    return {m_buffer.data() + m_offset, m_buffer.size() - m_offset};
  }

  /// @brief Gets the internal anonymous resource.
  static inline const Buffer *m_anonymous() noexcept {
    static auto s_resource = Buffer(Scheme::EVAL, "<anonymous>");
    return &s_resource; // return the anonymous reference now
  }

  /**
   * @brief Handles normalizing URI bodies.
   * @param body                        Path to normalize.
   */
  static inline String::Buffer m_normalize(const String::View &body) {
    // prepare a buffer to be modified
    auto buffer = String::Buffer(body);

#if $_PLATFORM_WINDOWS
    // for windows platforms we require a little manipulation
    std::replace(buffer.begin(), buffer.end(), '\\', '/');
    if (buffer.size() > 1 && buffer.at(1) == ':') buffer = '/' + buffer;
#endif

    // and return the resulting buffer now
    return buffer;
  }
};

} // namespace $::URI

#endif
