#ifndef _SABRE_DOTENV_READER_HPP
#define _SABRE_DOTENV_READER_HPP

/// Sabre Includes
#include "sabre/forward/document.hpp"
#include "sabre/forward/dotenv.hpp"

namespace Sabre::Dotenv {

/// @brief Environment File Reader.
struct Reader : public $::Ensure::Static {
  //  PUBLIC METHODS  //

  /**
   * @brief Handles parsing an input environment.
   * @param input                   Input to parse.
   * @param inherit                 Inherit base values.
   */
  static View &&parse(const $::String::View &input, View &&inherit);
  static View &&parse(const $::String::View &input, bool inherit = false);

private:
  //  PRIVATE METHODS  //

  /**
   * @brief Handles parsing a singular line.
   * @param environ                 Environment map.
   * @param line                    Line to parse.
   */
  static void m_parse(View &environ, const $::String::View &line);

  /**
   * @brief Attempts unescaping incoming values.
   * @param environ                 Environment map.
   * @param value                   Value to decipher.
   */
  static $::String::Buffer m_decipher(const View &environ, $::String::View &value);

  /**
   * @brief Handles inserting a pair into the environment.
   * @param environ                 Environment map.
   * @param key                     Key to emplace.
   * @param value                   Value to emplace.
   */
  static void m_emplace(View &environ, const $::String::View &key, const $::String::Buffer &value);
};

} // namespace Sabre::Dotenv

#endif
