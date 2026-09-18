#ifndef _SABRE_ARCHIVE_UNIT_HPP
#define _SABRE_ARCHIVE_UNIT_HPP

/// Sabre Includes
#include "sabre/forward/function.hpp"
#include "sabre/image/binary.hpp"
#include "sabre/string/intern.hpp"

namespace Sabre::Image {

/// @brief Image Compilation Unit.
struct Arena {
  //  PROPERTIES  //

  /// @brief Associated bytecode binary.
  Binary binary = {};

  /// @brief Reference to parent resource.
  $::URI::View resource = {};

  /// @brief All bound constant values.
  std::vector<Value::Any> constants = {};

  /// @brief All interned string values.
  std::vector<String::Intern> strings = {};

  /// @brief Bound functions to compilation unit.
  std::vector<$::Unique::Pointer<Function::Info>> functions = {};

protected:
  //  PRIVATE METHODS  //

  /**
   * @brief Handles printing archive units.
   * @param os                    Output stream.
   * @param self                  Unit to print.
   */
  static void m_print(std::ostream &os, const Arena &self);
};

} // namespace Sabre::Image

#endif
