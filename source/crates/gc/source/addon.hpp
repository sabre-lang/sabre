#ifndef _CRATES_PACKAGE_GARBAGE_HPP
#define _CRATES_PACKAGE_GARBAGE_HPP

/// Sabre Includes
#include <sabre/garbage/service.hpp>

/// Addon Includes
#include <sabre/dylib/_inline/dylib.ipp>

//  X-MACROS  //

#define CRATE_XX_GARBAGE_METHODS(X) \
  X(cycles)                         \
  X(collect)

//  NAMESPACES  //

namespace Sabre::Package {

/// @brief Garbage Package Addon.
class Garbage : public Dylib::Mixin<"gc"> {
  //  TYPEDEFS  //

  /// @brief Available service typing.
  using Service = ::Sabre::Garbage::Service;

public:
  //  CONSTRUCTORS  //

  /**
   * @brief Constructs an assertion library.
   * @param isolate               Runtime isolate.
   * @param exports               Addon exports.
   */
  explicit Garbage(Runtime::Isolate *isolate, Dylib::Exports &exports);

private:
  //  PRIVATE METHODS  //

  CRATE_XX_GARBAGE_METHODS(SABRE_MM_DYLIB_DEFINE)
};

} // namespace Sabre::Package

#endif
