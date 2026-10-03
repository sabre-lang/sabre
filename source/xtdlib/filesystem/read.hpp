#ifndef _XTDLIB_FILESYSTEM_READ_HPP
#define _XTDLIB_FILESYSTEM_READ_HPP

/// Library Includes
#include "xtdlib/filesystem/path.hpp"
#include "xtdlib/macros/forward.hpp"
#include "xtdlib/string/buffer.hpp"
#include "xtdlib/string/view.hpp"

/// Forward Declarations
$_FWD($::Memory, class Region)

namespace $::FS {

/**
 * @brief Reads a file into memory.
 * @param file_path             File to read.
 */
Memory::Region Read(const Path &file_path);
Memory::Region Read(const String::View &file_path);
Memory::Region Read(const String::Buffer &file_path);

} // namespace $::FS

#endif
