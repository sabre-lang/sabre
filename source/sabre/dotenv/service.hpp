#ifndef _SABRE_DOTENV_SERVICE_HPP
#define _SABRE_DOTENV_SERVICE_HPP

/// Sabre Includes
#include "sabre/forward/dotenv.hpp"

namespace Sabre::Dotenv {

/// @brief Environment Variables Service.
class Service : public XI::Singleton, public XI::Immediate {
  //  PROPERTIES  //

  /// @brief The internal view of the environment variables.
  View m_variables = $::Environ::view();

  /// @brief The core services container.
  XI::Container *m_services = nullptr;

public:
  //  CONSTRUCTORS  //

  /**
   * @brief Constructs a new environment service.
   * @param services                Services container.
   */
  explicit Service();
  explicit Service(XI::Container *services);

  //  PUBLIC METHODS  //

  /**
   * @brief Handles getting an environment variable.
   * @param key                     Key to get.
   */
  inline constexpr std::optional<$::String::Buffer> get(const $::String::View &key) const noexcept {
    return m_variables.contains(key) ? std::optional(m_variables.at(key)) : std::nullopt;
  }

  /**
   * @brief Handles removing an environment variable.
   * @param key                     Key to remove.
   */
  inline constexpr bool del(const $::String::View &key) noexcept { return m_variables.erase(key); }

  /**
   * @brief Handles setting an environment variable.
   * @param key                     Key to set.
   * @param value                   Value to set.
   */
  inline constexpr bool set(const $::String::View &key, const $::String::View &value) noexcept {
    return m_variables.insert_or_assign(key, $::String::Buffer(value)).second;
  }

private:
  //  PRIVATE METHODS  //

  /**
   * @brief Parses an environment file.
   * @param envfile                 Resource to read.
   */
  void m_parse(const $::FS::Path &envfile) noexcept;

  /**
   * @brief Resolves a suitable environment file.
   * @param relative                Relative path.
   * @param hint                    Directory hint.
   */
  $::FS::Path m_resolve(const $::String::View &relative, const $::FS::Path &hint = $::System::cwd()) const noexcept;

  /**
   * @brief Handles warning user about environment issues.
   * @param verbose                 Verbosity to use.
   * @param message                 Format message.
   * @param args                    Format arguments.
   */
  template <class... As> void m_warn(bool verbose, fmt::format_string<As...> message, As &&...args) const noexcept {
    if (!verbose) return; // ignore if not running verbosely
    std::cerr << $::Dye::yellow("Warning.Env").bold() << ": ";
    std::cerr << fmt::format(message, std::forward<As>(args)...) << '\n';
  }
};

} // namespace Sabre::Dotenv

#endif
