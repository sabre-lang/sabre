#ifndef _XTDLIB_SYSTEM_INFORMATION_HPP
#define _XTDLIB_SYSTEM_INFORMATION_HPP

/// Library Includes
#include "xtdlib/filesystem/path.hpp"
#include "xtdlib/string/buffer.hpp"
#include "xtdlib/string/view.hpp"
#include "xtdlib/system/platform.hpp"

namespace $::System {

/// @brief Gets/Sets the current working directory.
inline constexpr $::String::Buffer cwd() { return $::Path::current().string(); }
inline constexpr void cwd(const $::String::View &path) { std::filesystem::current_path(path); }

/// @brief Gets the global root directory.
inline constexpr $::String::Buffer root() { return $::Path::current().root_path().string(); }

/// @brief Gets the systems EOL value.
inline consteval String::View eol() {
#if $_PLATFORM_WINDOWS
  return "\r\n";
#else
  return "\n";
#endif
}

} // namespace $::System

#endif