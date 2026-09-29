#ifndef _XPC_SHELL_PATH_HPP
#define _XPC_SHELL_PATH_HPP

/// XPC Includes
#include "xpc/forward/shell.hpp"

namespace XPC::Shell::Path {

/// @brief The underlying path delimiter.
static inline constexpr char delimiter() { return $_PLATFORM_WINDOWS ? ';' : ':'; }

/// @brief Available program extensions.
static inline constexpr std::vector<$::String::Buffer> extensions() {
  if (!$_PLATFORM_WINDOWS) return {};
  return {".exe", ".cmd", ".ps1"};
}

/// @brief Gets the best shell-binary.
static inline constexpr std::optional<$::String::Buffer> binary() {
#if $_PLATFORM_WINDOWS
  // fast-resolve the Windows shell
  return "C:\\Windows\\System32\\cmd.exe";
#else
  static std::optional<$::String::Buffer> s_best = std::nullopt;
  if (s_best.has_value()) return s_best; // resolve best firstly

  // prepare a list of hardcoded shells that could be available
  static constexpr $::String::View s_hardcoded[] = {
      "/bin/bash",
      "/usr/bin/bash",
      "/usr/local/bin/bash",
      "/bin/sh",
      "/usr/bin/sh",
      "/usr/bin/zsh",
      "/usr/local/bin/zsh",
  };

  // try each of the hardcoded options now
  for (size_t ii = 0; ii < sizeof(s_hardcoded) / sizeof($::String::View); ++ii) {
    if ($::Path::is_file(s_hardcoded[ii])) return s_best = s_hardcoded[ii];
  }

  // failed to find a suitable shell
  return std::nullopt;
#endif
}

/// @brief Attempts finding a program from "PATH".
static inline std::optional<$::String::Buffer>
resolve(const $::String::View &name, std::optional<$::String::Buffer> path = std::nullopt) {
  // prepare the separator values if necessary
  static constexpr auto s_separator = $_PLATFORM_WINDOWS ? "/\\" : "/";

  // if the name is empty, immediately stop
  if (name.empty()) return std::nullopt;

  // if the name contains "/" or "\\" on windows, then resolve to an absolute path
  if (name.find_first_of(s_separator) != $::String::Term) return $::Path::absolute(name).string();

  // otherwise we have a purely searchable name
  path = path.or_else([] { return $::Environ::get("PATH"); });
  if (!path.has_value()) return std::nullopt; // failed to find

  // attempt checking for item in the path
  for (auto candidate : std::views::split(*path, delimiter())) {
    auto program = $::FS::Path($::String::View(candidate)) / name;
    if ($::Path::is_file(program)) return program.string(); // end

#if $_PLATFORM_WINDOWS
    // attempt checking against different extensions now
    for (const auto &extension : extensions()) {
      if (program.extension() == extension) break;
      program = program.replace_extension(extension);
      if ($::Path::is_file(program)) return program.string();
    }
#endif
  }

  // and stop since nowhere else to search
  return std::nullopt;
}

} // namespace XPC::Shell::Path

#endif
