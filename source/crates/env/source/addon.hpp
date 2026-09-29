#ifndef _CRATES_PACKAGE_ENV_HPP
#define _CRATES_PACKAGE_ENV_HPP

/// Addon Includes
#include <sabre/dylib/_inline/dylib.ipp>

//  X-MACROS  //

#define CRATE_XX_ENV_METHODS(X) \
  X(get)                        \
  X(set)                        \
  X(drop)

//  NAMESPACES  //

namespace Sabre::Package {

/// @brief Environ Package Addon.
struct Environ : public Dylib::Mixin<"env"> {
  //  CONSTRUCTORS  //

  /**
   * @brief Constructs a "env" library.
   * @param isolate               Runtime isolate.
   * @param exports               Addon exports.
   */
  explicit Environ(Runtime::Isolate *isolate, Dylib::Exports &exports);

private:
  //  PRIVATE METHODS  //

  CRATE_XX_ENV_METHODS(SABRE_MM_DYLIB_DEFINE)
};

} // namespace Sabre::Package

#endif
