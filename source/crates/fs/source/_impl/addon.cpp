/// Crate Includes
#include "crates/fs/source/addon.hpp"

//  PROPERTIES  //

/// @brief The underlying FS addon installer.
SABRE_MM_DYLIB_ADDON(FS, CRATE_XX_FS_METHODS)

//  ADDON METHODS  //

SABRE_MM_DYLIB_METHOD(FS, read_file, isolate, args) {
  // prepare the baase details to be used
  SABRE_MM_ASSERT_ARGC(isolate, args.size(), 1);
  SABRE_MM_ASSERT_TYPEOF(isolate, String::Any, args[0]);

  // get the incoming file to be read
  auto input_path = args.at<String::Any>(0);
  auto file_path = $::Path::canonical(input_path.view());

  // ensure the incoming file actually exists firstly
  if (!$::Path::exists(file_path)) return isolate->panic(6002000, input_path);

  // can safely construct our buffer as necessary
  auto buffer = $::FS::Read(file_path);

  // convert our result into a suitable output now
  return isolate->create<Iterable::Buffer>(std::move(buffer));
}

SABRE_MM_DYLIB_METHOD(FS, read_text, isolate, args) {
  // prepare the baase details to be used
  SABRE_MM_ASSERT_ARGC(isolate, args.size(), 1);
  SABRE_MM_ASSERT_TYPEOF(isolate, String::Any, args[0]);

  // get the incoming file to be read
  auto input_path = args.at<String::Any>(0);
  auto file_path = $::Path::canonical(input_path.view());

  // ensure the incoming file actually exists firstly
  if (!$::Path::exists(file_path)) return isolate->panic(6002000, input_path);

  // can safely construct our buffer as necessary
  auto buffer = $::FS::Read(file_path);

  // convert our result into a suitable output now
  return String::Any(isolate, buffer.view());
}

SABRE_MM_DYLIB_METHOD(FS, write_file, isolate, args) {
  // prepare the baase details to be used
  SABRE_MM_ASSERT_ARGC(isolate, args.size(), 2);
  SABRE_MM_ASSERT_TYPEOF(isolate, String::Any, args[0]);
  SABRE_MM_ASSERT_TYPEOF(isolate, Iterable::Buffer, args[1]);

  // get the incoming file to be read
  auto input_path = args.at<String::Any>(0);
  auto input_buffer = args.at<Iterable::Buffer>(1);
  auto file_path = $::Path::canonical(input_path.view());

  // attempt writing our file now
  return $::FS::Overwrite(file_path, input_buffer.view()), Value::Void();
}

SABRE_MM_DYLIB_METHOD(FS, write_text, isolate, args) {
  // prepare the base details to be used
  SABRE_MM_ASSERT_ARGC(isolate, args.size(), 2);
  SABRE_MM_ASSERT_TYPEOF(isolate, String::Any, args[0]);
  SABRE_MM_ASSERT_TYPEOF(isolate, String::Any, args[1]);

  // get the incoming file to be read
  auto input_path = args.at<String::Any>(0);
  auto input_buffer = args.at<String::Any>(1);
  auto file_path = $::Path::canonical(input_path.view());

  // attempt writing our file now
  return $::FS::Overwrite(file_path, input_buffer.view()), Value::Void();
}

SABRE_MM_DYLIB_METHOD(FS, remove_entry, isolate, args) {
  // prepare the base details to be used
  SABRE_MM_ASSERT_ARGC(isolate, args.size(), 1);
  SABRE_MM_ASSERT_TYPEOF(isolate, String::Any, args[0]);

  // get the incoming file to be removed
  auto input_path = args.at<String::Any>(0);
  auto file_path = $::Path::canonical(input_path.view());

  // ignore if the file does not currently exist
  if (!$::Path::exists(file_path)) return Value::Void();

  // attempt removing the file as necessary now
  return std::filesystem::remove(file_path), Value::Void();
}

SABRE_MM_DYLIB_METHOD(FS, temp_file, isolate, args) {
  // generate a suitably unique file name to be used
  auto prefix = args.when<String::Any>(0);
  auto suffix = args.when<String::Any>(1);

  // ensure we construct a suitable unique basename now
  auto basename = XH::UUID::V7().to_string();
  if (prefix) basename = prefix->clone() + basename;
  basename = basename + (suffix ? suffix->clone() : ".txt");

  // construct the unique path now as necessary
  auto file_path = std::filesystem::temp_directory_path() / basename;

  // ensure the file is now created
  $_UNUSED $_AUTO = $::FS::Touch(file_path, true);

  // return the resulting file-path now
  return String::Any(isolate, file_path.string());
}

SABRE_MM_DYLIB_METHOD(FS, exists_file, isolate, args) {
  // prepare the base details to be used
  SABRE_MM_ASSERT_ARGC(isolate, args.size(), 1);
  SABRE_MM_ASSERT_TYPEOF(isolate, String::Any, args[0]);

  // get the incoming file to be removed
  auto input_path = args.at<String::Any>(0);
  auto file_path = $::Path::canonical(input_path.view());

  // only return true if a file and exists
  return Value::Boolean($::Path::is_file(file_path));
}

SABRE_MM_DYLIB_METHOD(FS, exists_entry, isolate, args) {
  // prepare the base details to be used
  SABRE_MM_ASSERT_ARGC(isolate, args.size(), 1);
  SABRE_MM_ASSERT_TYPEOF(isolate, String::Any, args[0]);

  // get the incoming file to be removed
  auto input_path = args.at<String::Any>(0);
  auto file_path = $::Path::canonical(input_path.view());

  // only return true if a file and exists
  return Value::Boolean($::Path::exists(file_path));
}
