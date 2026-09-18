#ifndef _SABRE_PRODUCT_SEMVER_HPP
#define _SABRE_PRODUCT_SEMVER_HPP

/// Vendor Includes
#include <xtdlib/xtdlib.hpp>

//  MACROS  //

/// @brief The underlying revision details.
#define SABRE_MM_REVISION SABRE_MM_VERSION "+" SABRE_MM_COMMIT

//  NAMESPACES  //

namespace Sabre::Version {

/// @brief Semantic Versioning Structure.
struct Semantic {
  //  PROPERTIES  //

  uint32_t m_major = 0; // Major versioning.
  uint32_t m_minor = 0; // Minor versioning.
  uint32_t m_patch = 0; // Patch versioning.

  /// @brief Optional build versioning.
  $::String::View m_build = "";

public:
  //  CONSTRUCTORS  //

  /// @brief Constructs a zeroed version.
  constexpr Semantic() = default;

  /**
   * @brief Attempts parsing a semantic version.
   * @param version               Version text.
   */
  constexpr Semantic(const $::String::View &version) : Semantic(m_parse(version)) {}

  /**
   * @brief Constructs a full version.
   * @param major                 Major value.
   * @param minor                 Minor value.
   * @param patch                 Patch value.
   * @param build                 Build number.
   */
  constexpr Semantic(uint32_t major, uint32_t minor, uint32_t patch, const $::String::View &build = "")
      : m_major(major), m_minor(minor), m_patch(patch), m_build(build) {}

  //  PUBLIC METHODS  //

  inline constexpr uint32_t major() const noexcept { return m_major; }
  inline constexpr Semantic &major(uint32_t value) noexcept { return m_major = value, *this; }

  inline constexpr uint32_t minor() const noexcept { return m_minor; }
  inline constexpr Semantic &minor(uint32_t value) noexcept { return m_minor = value, *this; }

  inline constexpr uint32_t patch() const noexcept { return m_patch; }
  inline constexpr Semantic &patch(uint32_t value) noexcept { return m_patch = value, *this; }

  inline constexpr Semantic &build(const $::String::View &value) noexcept { return m_build = value, *this; }
  inline constexpr $::String::View build() const noexcept {
    return m_build.starts_with('-') ? m_build.substr(1) : m_build;
  }

  /// @brief A shortened version string.
  inline constexpr $::String::Buffer terse() const noexcept {
    return fmt::format("{0}.{1}.{2}", m_major, m_minor, m_patch);
  }

  /// @brief A full version string.
  inline constexpr $::String::Buffer full() const noexcept {
    return fmt::format("{0}.{1}.{2}{3}", m_major, m_minor, m_patch, m_build);
  }

protected:
  //  PRIVATE METHODS  //

  /**
   * @brief Handles parsing versions.
   * @param input                 Input to parse.
   */
  static inline Semantic m_parse($::String::View input, size_t offset = 0) {
    // prepare the outgoing version now
    auto version = Semantic();

    // remove any leading characters now (eg: "v")
    input = $::Trim::leading(input, "vV");
    auto buffer = $::String::Buffer(input);

    // parse out the first leading number
    version.m_major = std::stoul(buffer, &offset);

    // slice off the current offset
    input = input.substr(offset);

    // parse out the next numeric (if possible)
    if (input.starts_with('.')) {
      buffer = $::String::Buffer(input.substr(1));
      version.m_minor = std::stoul(buffer, &offset);
      input = input.substr(offset + 1); // bounce
    }

    // parse out the next numeric (if possible)
    if (input.starts_with('.')) {
      buffer = $::String::Buffer(input.substr(1));
      version.m_patch = std::stoul(buffer, &offset);
      input = input.substr(offset + 1); // bounce
    }

    // any trailing parts become the build-number
    if (input.size()) version.m_build = input;

    // and return the resulting version
    return version;
  }

  /**
   * @brief Handles printing versions.
   * @param os                    Output stream.
   * @param self                  Version instance.
   */
  static inline void m_print(std::ostream &os, const Semantic &self) { os << 'v' << self.full(); }
};

} // namespace Sabre::Version

#endif
