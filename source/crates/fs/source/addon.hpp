#ifndef _CRATES_PACKAGE_FS_HPP
#define _CRATES_PACKAGE_FS_HPP

/// Addon Includes
#include <sabre/dylib/_inline/dylib.ipp>

//  X-MACROS  //

#define CRATE_XX_FS_METHODS(X) \
  X(read_file)                 \
  X(read_text)                 \
  X(write_file)                \
  X(write_text)                \
  X(remove_entry)              \
  X(temp_file)                 \
  X(exists_file)               \
  X(exists_entry)

//  NAMESPACES  //

namespace Sabre::Package {

/// @brief FS Package Addon.
struct FS : public Dylib::Mixin<"fs"> {
  //  CONSTRUCTORS  //

  /**
   * @brief Constructs a "fs" library.
   * @param isolate               Runtime isolate.
   * @param exports               Addon exports.
   */
  explicit FS(Runtime::Isolate *isolate, Dylib::Exports &exports);

private:
  //  PRIVATE METHODS  //

  CRATE_XX_FS_METHODS(SABRE_MM_DYLIB_DEFINE)
};

} // namespace Sabre::Package

#endif
