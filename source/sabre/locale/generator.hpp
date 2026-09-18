#ifndef _SABRE_LOCALE_GENERATOR_HPP
#define _SABRE_LOCALE_GENERATOR_HPP

/// Sabre Includes
#include "sabre/forward/locale.hpp"

namespace Sabre::Locale {

/// @brief Handles generating locales.
class Generator {
  //  PROPERTIES  //

  /// @brief Internal generator context.
  void *m_context = nullptr;

public:
  //  CONSTRUCTORS  //

  /// @brief Handles constructing the generator.
  Generator();

  /// @brief Handles destructing the generator.
  ~Generator();

  //  OPERATOR METHODS  //

  /**
   * @brief Handles wrapping a classic locale.
   * @param id                      Identifier to use.
   */
  inline constexpr std::locale operator()(const $::String::Buffer &id = {}) const noexcept {
    return m_bind(std::locale::classic(), id);
  }

  /**
   * @brief Handles wrapping a locale.
   * @param locale                  Locale to bind.
   * @param id                      Identifier to use.
   */
  inline constexpr std::locale operator()(const std::locale &locale, const $::String::Buffer &id = {}) const noexcept {
    return m_bind(locale, id);
  }

private:
  //  PRIVATE METHODS  //

  /**
   * @brief Handles wrapping a locale.
   * @param locale                  Locale to bind.
   * @param id                      Identifier to use.
   */
  std::locale m_bind(const std::locale &locale, const $::String::Buffer &id = "") const noexcept;
};

} // namespace Sabre::Locale

#endif
