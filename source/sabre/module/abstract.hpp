#ifndef _SABRE_MODULE_ABSTRACT_HPP
#define _SABRE_MODULE_ABSTRACT_HPP

/// Sabre Includes
#include "sabre/async/thenable.hpp"
#include "sabre/diagnostic/reporter.hpp"
#include "sabre/engine/metadata.hpp"
#include "sabre/forward/document.hpp"
#include "sabre/forward/syntax.hpp"
#include "sabre/module/phase.hpp"

namespace Sabre::Module {

/// @brief Module Mixin Abstraction.
template <class T> using Mixin = $::RTTI::Mixin<T, Abstract>;

/// @brief Baseline Module Abstraction.
class $_ABSTRACT Abstract : public XI::Transient, public $::RTTI::Dynamic {
  //  PROPERTIES  //

  /// @brief Module mutex instance.
  mutable $::Mutex::Light m_mutex;

  /// @brief The current interface phase.
  $::Async::Atomic<Phase> m_phase = Phase::CLEANED;

  /// @brief Constructs a baseline reporter.
  $::Unique::Pointer<Diagnostic::Reporter> m_reporter = nullptr;

  /// @brief All available metadata values.
  std::array<$::Unique::Pointer<Metadata>, std::to_underlying(Phase::EXPORTED)> m_metadata = {};

public:
  //  CONSTRUCTORS  //

  /**
   * @brief Constructs a module interface.
   * @param services                Services container.
   * @param resource                Module resource.
   */
  explicit Abstract(const $::URI::Buffer &resource = {});
  explicit Abstract(XI::Container *services, const $::URI::Buffer &resource = {});

  //  PUBLIC METHODS  //

  /// @brief Gets the current module phase.
  inline constexpr Phase phase() const noexcept { return m_phase; }

  /// @brief Gets the resource of the module.
  inline constexpr $::URI::View resource() const noexcept { return m_reporter->resource(); }

  /// @brief Denotes if the module is ready for execution.
  inline constexpr bool ready() const noexcept { return phase() >= Phase::COMPILED; }

  /// @brief Gets the underlying diagnostic reporter instance.
  inline constexpr Diagnostic::Reporter *reporter() noexcept { return m_reporter.get(); }
  inline constexpr const Diagnostic::Reporter *reporter() const noexcept { return m_reporter.get(); }

  /// @brief Gets all available imports.
  inline constexpr Syntax::Dependencies dependencies() const noexcept { return m_dependencies(); }

  /// @brief Safely constructs metadata values.
  template <Phase P> inline constexpr Metadata::Wrapper<P> *metadata() {
    static constexpr auto s_index = static_cast<uint8_t>(P) - 1; // get the current index
    if (m_metadata[s_index] == nullptr) m_metadata[s_index] = $::Unique::New<Metadata::Wrapper<P>>();
    return static_cast<Metadata::Wrapper<P> *>(m_metadata[s_index].get()); // resolve the metadata
  }

  /// @brief Allows getting currently made metadata.
  template <Phase P> inline constexpr const Metadata::Wrapper<P> *metadata() const {
    static constexpr auto s_index = static_cast<uint8_t>(P) - 1; // get the current index
    $_ASSERT(m_metadata[s_index] != nullptr, "Metadata for interface not yet created");
    return static_cast<const Metadata::Wrapper<P> *>(m_metadata[s_index].get());
  }

  /**
   * @brief Handles parsing the module.
   * @param services              Services container.
   */
  inline constexpr void parse(XI::Container *services) {
    $_UNUSED $_AUTO = $::Lock::guard(m_mutex);
    if (m_phase >= Phase::PARSED) return;
    m_parse(services), m_phase = Phase::PARSED;
    m_dump(Dump::SYNTAX, services); // print
  }

  /**
   * @brief Handles analyzing the module.
   * @param services              Services container.
   */
  inline constexpr void analyze(XI::Container *services) {
    $_UNUSED $_AUTO = $::Lock::guard(m_mutex);
    if (m_phase >= Phase::TYPED) return;
    m_analyze(services), m_phase = Phase::TYPED;
    m_dump(Dump::TYPEDEFS, services); // print
  }

  /**
   * @brief Handles compiling the module.
   * @param services              Services container.
   */
  inline constexpr void compile(XI::Container *services) {
    $_UNUSED $_AUTO = $::Lock::guard(m_mutex);
    if (m_phase >= Phase::COMPILED) return;
    m_compile(services), m_phase = Phase::COMPILED;
    m_dump(Dump::BYTECODE, services); // print
  }

  /**
   * @brief Handles interpreting a module.
   * @param isolate               Runtime isolate.
   * @param trace                 Resource trace.
   */
  inline constexpr Async::Thenable *interpret(Runtime::Isolate *isolate) {
    // prepare the available exports to be used
    auto *exports = metadata<Phase::EXPORTED>();

    // check if we can suitably interpret our details (otherwise return the promise)
    auto ready = m_phase.compare_exchange_strong(Phase::COMPILED, Phase::EXPORTED);

    // and attempt interpretting safely now
    return ready ? m_interpret(isolate, exports) : exports->thenable();
  }

protected:
  //  PRIVATE METHODS  //

  /// @brief Gets the underlying available dependencies.
  Syntax::Dependencies m_dependencies() const noexcept;

  /**
   * @brief Handles dumping outputs.
   * @param type                      Type of dump.
   * @param services                  Services container.
   */
  void m_dump(Dump type, XI::Container *services) const noexcept;

  /**
   * @brief Gets the document buffer for a module.
   * @param services                  Service container.
   * @param resource                  Resource to find.
   */
  $::Shared::Pointer<Document::Buffer> m_document(XI::Container *services) const noexcept;
  $::Shared::Pointer<Document::Buffer> m_document(XI::Container *services, const $::URI::View &resource) const noexcept;

  /**
   * @brief Handles parsing the module.
   * @param services              Services container.
   */
  virtual void m_parse(XI::Container *services);

  /**
   * @brief Handles analyzing the module.
   * @param services              Services container.
   */
  virtual void m_analyze(XI::Container *services);

  /**
   * @brief Handles compiling the module.
   * @param services              Services container.
   */
  virtual void m_compile(XI::Container *services);

  /**
   * @brief Handles interpreting a module.
   * @param isolate               Runtime isolate.
   * @param exports               Export metadata.
   */
  virtual Async::Thenable *m_interpret(Runtime::Isolate *isolate, Metadata::Wrapper<Phase::EXPORTED> *exports);
};

} // namespace Sabre::Module

#endif
