#ifndef _CRATES_PACKAGE_TOML_HPP
#define _CRATES_PACKAGE_TOML_HPP

/// Addon Includes
#include <sabre/dylib/_inline/dylib.ipp>

//  X-MACROS  //

#define CRATE_XX_TOML_METHODS(X) \
  X(encode)                      \
  X(decode)

//  NAMESPACES  //

namespace Sabre::Package {

/// @brief TOML Package Addon.
struct TOML : public Dylib::Mixin<"toml"> {
  //  CONSTRUCTORS  //

  /**
   * @brief Constructs a "toml" library.
   * @param isolate               Runtime isolate.
   * @param exports               Addon exports.
   */
  explicit TOML(Runtime::Isolate *isolate, Dylib::Exports &exports);

private:
  //  PRIVATE METHODS  //

  CRATE_XX_TOML_METHODS(SABRE_MM_DYLIB_DEFINE)
};

} // namespace Sabre::Package

#endif
