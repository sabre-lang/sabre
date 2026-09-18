#ifndef _CRATES_PACKAGE_MEMORY_HPP
#define _CRATES_PACKAGE_MEMORY_HPP

/// Sabre Includes
#include <sabre/heap/service.hpp>

/// Addon Includes
#include <sabre/dylib/_inline/dylib.ipp>

//  X-MACROS  //

#define CRATE_XX_MEMORY_METHODS(X) \
  X(heap_avail)                    \
  X(heap_total)                    \
  X(heap_usage)                    \
                                   \
  X(thread_avail)                  \
  X(thread_total)                  \
  X(thread_usage)

//  NAMESPACES  //

namespace Sabre::Package {

/// @brief Memory Package Addon.
struct Memory : public Dylib::Mixin<"mem"> {
  //  CONSTRUCTORS  //

  /**
   * @brief Constructs an assertion library.
   * @param isolate               Runtime isolate.
   * @param exports               Addon exports.
   */
  explicit Memory(Runtime::Isolate *isolate, Dylib::Exports &exports);

private:
  //  PRIVATE METHODS  //

  CRATE_XX_MEMORY_METHODS(SABRE_MM_DYLIB_DEFINE)
};

} // namespace Sabre::Package

#endif
