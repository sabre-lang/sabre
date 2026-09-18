#ifndef _SABRE_CRATES_MANIFEST_HPP
#define _SABRE_CRATES_MANIFEST_HPP

/// Sabre Includes
#include "sabre/format/options.hpp"
#include "sabre/forward/crate.hpp"
#include "sabre/relint/options.hpp"

//  X-MACROS  //

#define SABRE_XX_CRATE_OPTIONALS(X)   \
  X(title, $::String::Buffer)         \
  X(describe, $::String::Buffer)      \
                                      \
  X(issues, $::String::Buffer)        \
  X(homepage, $::String::Buffer)      \
  X(repository, $::String::Buffer)    \
  X(documentation, $::String::Buffer)

#define SABRE_XX_CRATE_DICTIONARIES(X) \
  X(tasks, $::String::Buffer)          \
  X(constraints, $::String::Buffer)    \
  X(dependencies, $::String::Buffer)

//  NAMESPACES  //

namespace Sabre::Crate {

/// @brief Crate Manifest File.
class Manifest {
  //  PROPERTIES  //

  $::String::Buffer m_version = "0.0.0";        // Specification version.
  $::String::Buffer m_name = "@sabre/unknown";  // Associated crate-name.
  $::String::Buffer m_path = $::System::root(); // Prepare the base-path.

  /// @brief Explicit main script.
  std::optional<$::String::Buffer> m_main = std::nullopt;

#define X(N, T, ...) std::optional<T> m_##N = std::nullopt;
  SABRE_XX_CRATE_OPTIONALS(X)
#undef X

#define X(N, T, ...) $::Map::Dict<T> m_##N = {};
  SABRE_XX_CRATE_DICTIONARIES(X)
#undef X

  /// @brief Incoming linting options.
  Relint::Options m_lint = {};

  /// @brief Incoming formatter options.
  Format::Options m_format = {};

public:
  //  CONSTRUCTORS  //

  /// @brief Constructs a set of default options.
  constexpr Manifest() = default;

  /**
   * @brief Constructs options with a path.
   * @param path                    Crate options path.
   */
  constexpr Manifest(const $::String::Buffer &path) : m_path(path) {}

  //  PUBLIC METHODS  //

  inline constexpr $::String::View path() const { return m_path; }
  inline constexpr $::String::View name() const { return m_name; }
  inline constexpr $::String::View version() const { return m_version; }

  inline constexpr const Relint::Options &lint() const noexcept { return m_lint; }
  inline constexpr const Format::Options &format() const noexcept { return m_format; }

#define X(N, T, ...)                                                                                               \
  inline constexpr $::String::View N() const noexcept { return m_##N.has_value() ? $::String::View(*m_##N) : ""; }
  SABRE_XX_CRATE_OPTIONALS(X)
#undef X

#define X(N, T, ...)                                                           \
  inline constexpr const $::Map::Dict<T> &N() const noexcept { return m_##N; }
  SABRE_XX_CRATE_DICTIONARIES(X)
#undef X

  /// @brief Gets the current main resource.
  inline constexpr std::optional<$::URI::Buffer> main() const {
    if (!m_main.has_value()) return std::nullopt; // stop early if missing
    return $::URI::Buffer($::URI::Scheme::FILE, resolve(*m_main).string());
  }

  /**
   * @brief Resolves resources relative to this crate.
   * @param relative                Relative path to resolve.
   */
  inline constexpr $::FS::Path resolve(const $::FS::Path &relative) const noexcept {
    return $::Path::canonical(relative, m_path);
  }

protected:
  //  PRIVATE METHODS  //

  /**
   * @brief Handles decoding crate manifest files.
   * @param value                   Value to decode.
   */
  static $::Unique::Pointer<Manifest> m_decode(const $::Serde::Value &value);
  static $::Unique::Pointer<Manifest> m_decode(const $::Serde::Value &value, const $::String::Buffer &path);
};

} // namespace Sabre::Crate

#endif
