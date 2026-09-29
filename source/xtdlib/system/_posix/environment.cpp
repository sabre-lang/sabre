/// Platform Includes
#include "xtdlib/system/platform.hpp"

#if $_PLATFORM_POSIX

/// POSIX Includes
#include <unistd.h>

/// Library Includes
#include "xtdlib/macros/forward.hpp"
#include "xtdlib/system/environment.hpp"

/// Forward Declarations
$_FWD($::Environ, size_t separator(const String::View &pair))

//  MACROS  //

#if defined(__APPLE__)
#include <crt_externs.h>
#if !defined(environ)
#define environ (*_NSGetEnviron())
#endif
#elif defined(__MACH__) || defined(__FreeBSD__) || defined(__DragonFly__) || defined(__NetBSD__) || \
    defined(__OpenBSD__) || defined(__sun)
extern "C" {
extern char **environ;
}
#endif

//  PUBLIC METHODS  //

std::optional<$::String::Buffer> $::Environ::get(const String::Buffer &key) {
  auto *result = ::getenv(key.c_str()); // attempt finding
  return result ? std::optional(result) : std::nullopt;
}

bool $::Environ::del(const String::Buffer &key) { return ::unsetenv(key.c_str()) == 0; }
bool $::Environ::set(const String::Buffer &key, const String::Buffer &value) {
  return ::setenv(key.c_str(), value.c_str(), true) == 0;
}

size_t $::Environ::separator(const String::View &pair) {
  // prepare the underlying separator value
  static constexpr auto value = '=';

  // attempt an initial find
  if (auto equals = pair.find(value)) return equals;

  // otherwise search one-place further
  auto equals = pair.find(value, 1);
  return equals == String::Term ? 0 : equals;
}

$::Map::Dict<$::String::Buffer> $::Environ::view() {
  // prepare the output now
  auto output = Map::Dict<String::Buffer>();

  // attempt iterating over the environment now
  for (auto *ii = environ; *ii != nullptr; ++ii) {
    // prepare the pair and equals location
    auto pair = String::Buffer(*ii);
    auto equals = separator(pair);

    // resolve the middle value to be used
    auto middle = equals == String::Term ? pair.size() : equals;

    // and push our key/value to the dictionary
    output.emplace(pair.substr(0, middle), pair.substr(equals + 1));
  }

  // return the resulting output now
  return output;
}

#endif
