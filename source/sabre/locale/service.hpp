#ifndef _SABRE_LOCALE_SERVICE_HPP
#define _SABRE_LOCALE_SERVICE_HPP

/// Sabre Includes
#include "sabre/locale/generator.hpp"
#include "sabre/locale/options.hpp"

namespace Sabre::Locale {

/// @brief Underlying Locale Service.
class Service : public XI::Singleton, public XI::Immediate {
  //  PROPERTIES  //

  /// @brief Underlying options available.
  const Options *m_options;

  /// @brief The bound locale generator.
  Generator m_generator = {};

  /// @brief Keep a copy of the base locale.
  std::locale m_classic = m_generator();

public:
  //  CONSTRUCTORS  //

  /// @brief Constructs a defaulted locale service.
  explicit Service();

  /**
   * @brief Constructs a locale service.
   * @param options               Options to bind.
   */
  explicit Service(const Options *options);

  /**
   * @brief Constructs a locale service.
   * @param services              Services container.
   */
  explicit Service(XI::Container *services);

  //  PUBLIC METHODS  //

  /// @brief Gets the underlying garbage options.
  inline constexpr const Options *options() const noexcept { return m_options; }

  /// @brief Handles resetting the current locale.
  inline void reset() { change(m_classic); }

  /**
   * @brief Handles updating the locale with construction.
   * @tparam As                       Argument types.
   * @param args                      Locale arguments.
   */
  template <class... As> inline void change(As &&...args) {
    const auto &locale = m_generator(std::forward<As>(args)...);
    return change(locale); // and forcibly update from overload
  }

  /**
   * @brief Sets the current global locale.
   * @param locale                    Locale to be bound.
   */
  inline void change(const std::locale &locale) {
    auto color = $::Color::Enabled(std::cout); // get the color enablement
    std::locale::global(std::locale(locale, new $::Color::Facet(color)));
  }

  /**
   * @brief Compares two strings using the base locale.
   * @param left                      Left-most string.
   * @param right                     Right-most string.
   */
  inline int64_t compare(const $::String::View &left, const $::String::View &right) const {
    return m_compare(left, right, m_generator());
  }

  /**
   * @brief Compares two strings using the base locale.
   * @param left                      Left-most string.
   * @param right                     Right-most string.
   * @param locale                    Locale to inherit.
   */
  inline int64_t compare(const $::String::View &left, const $::String::View &right, const std::locale &locale) const {
    return m_compare(left, right, locale);
  }

  /**
   * @brief Handles updating a values case.
   * @param input                     Input to transform.
   * @param locale                    Locale to inherit.
   */
  inline $::String::Buffer lowercase(const $::String::View &input) const { return m_lower(input, m_generator()); }
  inline $::String::Buffer lowercase(const $::String::View &input, const std::locale &locale) const {
    return m_lower(input, locale);
  }

  /**
   * @brief Handles updating a values case.
   * @param input                     Input to transform.
   * @param locale                    Locale to inherit.
   */
  inline $::String::Buffer uppercase(const $::String::View &input) const { return m_upper(input, m_generator()); }
  inline $::String::Buffer uppercase(const $::String::View &input, const std::locale &locale) const {
    return m_upper(input, locale);
  }

private:
  //  PRIVATE METHODS  //

  /**
   * @brief Handles updating a values case.
   * @param input                     Input to transform.
   * @param locale                    Locale to inherit.
   */
  $::String::Buffer m_lower(const $::String::View &input, const std::locale &locale) const;
  $::String::Buffer m_upper(const $::String::View &input, const std::locale &locale) const;

  /**
   * @brief Compares two strings using the base locale.
   * @param left                      Left-most string.
   * @param right                     Right-most string.
   * @param locale                    Locale to inherit.
   */
  int64_t m_compare(const $::String::View &left, const $::String::View &right, const std::locale &locale) const;
};

} // namespace Sabre::Locale

#endif
