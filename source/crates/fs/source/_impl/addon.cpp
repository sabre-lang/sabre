/// Crate Includes
#include "crates/fs/source/addon.hpp"

//  PROPERTIES  //

/// @brief The underlying FS addon installer.
SABRE_MM_DYLIB_ADDON(FS, CRATE_XX_FS_METHODS)

//  ADDON METHODS  //

SABRE_MM_DYLIB_METHOD(FS, read_file, isolate, args) {
  SABRE_MM_ASSERT_ARGC(isolate, args.size(), 1);
  SABRE_MM_ASSERT_TYPEOF(isolate, String::Any, args[0]);

  // get the incoming file to be read
  auto file_path = args.at<String::Any>(0);
  auto buffer = $::FS::Read($::Path::absolute(file_path.view()));

  // convert our result into a suitable output now
  return String::Any(isolate, buffer);
}

SABRE_MM_DYLIB_METHOD(FS, read_dir, isolate, args) {
  SABRE_MM_ASSERT_ARGC(isolate, args.size(), 1);
  SABRE_MM_ASSERT_TYPEOF(isolate, String::Any, args[0]);
  return isolate->todo();
}
