#ifndef _SABRE_BUNDLE_WORKER_HPP
#define _SABRE_BUNDLE_WORKER_HPP

/// Sabre Includes
#include "sabre/async/entry.hpp"
#include "sabre/bundle/archive.hpp"
#include "sabre/bundle/options.hpp"

namespace Sabre::Bundle {

/// @brief Bundler Worker Thread.
class Worker : public Async::Entry {
  //  PROPERTIES  //

  /// @brief Bundler options.
  Options m_options = {};

  /// @brief Verbose spinner details.
  $::Spinner::Pointer m_spinner = nullptr;

  /// @brief Binary archive instance.
  $::Unique::Pointer<Archive> m_archive = nullptr;

public:
  //  CONSTRUCTORS  //

  /**
   * @brief Constructs a bundle worker.
   * @param options           Bundler options.
   */
  explicit Worker(const Options &options = {});
  explicit Worker(XI::Container *services, const Options &options = {});

protected:
  //  PRIVATE METHODS  //

  /// @brief Handles launching the bundler.
  $_NORETURN void m_execute() final;

  /**
   * @brief Handles resolving the script.
   * @param script                Entry script.
   */
  $::URI::Buffer m_resolve(const $::String::View &script);

  /**
   * @brief Handles codesigning the output.
   * @param output                Output path.
   */
  bool m_codesign(const $::FS::Path &output) const noexcept;

  /**
   * @brief Handles imbuing a binary with data.
   * @param binary                Binary to imbue.
   * @param blob                  Resource blob.
   */
  bool m_imbue(Executable &binary, const XJCT::Blob::View &blob) const noexcept;
  bool m_imbue(Executable &binary, const XJCT::Blob::Bytes &buffer) const noexcept;

  /**
   * @brief Handles resolving outputs.
   * @param script                Script resource.
   * @param extension             Extension to inherit.
   */
  $::FS::Path m_output(const $::URI::View &script);
  $::FS::Path m_output(const $::URI::View &script, const $::String::View &extension);

  /**
   * @brief Shows bundle failures.
   * @param code                  Diagnostic code.
   * @param args                  Message arguments.
   */
  template <class... As> inline constexpr bool m_failure(Diagnostic::Code code, As &&...args) const noexcept {
    auto category = Diagnostic::Inspect::category(code);
    auto title = $::Dye::red("Exception.{0}", category).bold();
    auto message = Diagnostic::Inspect::format(code, std::forward<As>(args)...);
    if (m_spinner) m_spinner->dismiss(); // ensure we finalize now
    return std::cerr << title << ": " << message << '\n', false;
  }
};

} // namespace Sabre::Bundle

#endif
