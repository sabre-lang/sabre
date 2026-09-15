#ifndef _TALOS_RUNTIME_ISOLATE_HPP
#define _TALOS_RUNTIME_ISOLATE_HPP

/// Talos Includes
#include "talos/async/thenable.hpp"
#include "talos/diagnostic/inspect.hpp"
#include "talos/engine/frame.hpp"
#include "talos/function/args.hpp"
#include "talos/handle/scope.hpp"
#include "talos/object/exception.hpp"
#include "talos/resource/scheme.hpp"
#include "talos/runtime/allocator.hpp"
#include "talos/runtime/options.hpp"
#include "talos/string/common.hpp"

namespace Talos::Runtime {

/// @brief Runtime Thread Isolate.
class $_ABSTRACT Isolate {
  //  TYPEDEFS  //

  /// @brief Allow the allocator internal access.
  friend class Allocator;

  /// @brief Allow interpreter frames internal access.
  friend struct Engine::Frame;

  /// @brief Allow handles internal access.
  template <std::derived_from<Value::Any> T> friend class Handle::Local;

protected:
  //  PROPERTIES  //

  /// @brief Services container.
  XI::Container *m_services = nullptr;

  /// @brief Bound data value to assign.
  Value::Any m_data = {};

  /// @brief Currently bound exception value.
  Value::Any m_exception = {};

  /// @brief Currently attached value handles.
  Handle::Stack m_handles = {};

  /// @brief Currently bound interpreter frames.
  std::vector<Engine::Frame *> m_frames = {};

public:
  //  CONSTRUCTORS  //

  /**
   * @brief Constructs a runtime isolate.
   * @param services              Services container.
   * @param data                  Bound data value.
   */
  explicit Isolate();
  explicit Isolate(XI::Container *services);
  explicit Isolate(XI::Container *services, const Value::Any &data);

  /// @brief Virtual abstract destructor.
  virtual ~Isolate() = default;

  //  PUBLIC METHODS  //

  /// @brief Gets the current interpreter frame.
  inline constexpr Engine::Frame *frame() const noexcept { return m_frames.empty() ? nullptr : m_frames.back(); }
  inline constexpr Engine::Frame *frame(size_t depth) const noexcept {
    if (m_frames.size() < depth) return nullptr;
    return m_frames.at(m_frames.size() - depth - 1);
  }

  /// @brief Gets the underlying thread instance (allows mocking).
  virtual inline constexpr Async::Thread *thread() const noexcept { return nullptr; }

  /// @brief Current exception value (usually "void").
  inline constexpr Value::Any exception() const noexcept { return m_exception; }

  /// @brief Constructs a local handle scoping.
  inline constexpr Handle::Scope scope() noexcept { return Handle::Scope(this); }
  inline constexpr Handle::Scope scope(Engine::Exports *exports) noexcept { return m_scope(exports); }

  /// @brief Gets the underlying runtime options.
  inline constexpr const Options *options() const noexcept { return *m_services; }

  /// @brief Gets a formatted backtrace value.
  inline constexpr std::vector<Resource::Trace> backtrace() { return m_backtrace(); }

  /// @brief Gets the disposable lifetimes container.
  inline constexpr Garbage::Lifetimes *lifetimes() const noexcept { return m_lifetimes(); }

  /// @brief Gets a service from the runtime.
  template <std::derived_from<XI::Singleton> T> inline constexpr T *service() const noexcept { return *m_services; }

  /// @brief Gets a unique service from the runtime.
  template <std::derived_from<XI::Transient> T> inline constexpr auto service() const noexcept { return *m_services; }

  /// @brief Handles getting the baseline services container.
  inline constexpr XI::Container *service() const noexcept { return service<XI::Container>(); }
  template <std::derived_from<XI::Container> T> inline constexpr T *service() const noexcept {
    return static_cast<T *>(m_services);
  }

  /// @brief Forcibly interrupts the current frame.
  inline constexpr void interrupt() noexcept {
    if (m_frames.empty()) return;
    m_frames.back()->interrupt();
  }

  /**
   * @brief Handles yielding isolate roots.
   * @param yield                     Yield handler.
   */
  void roots(Globals::Each &yield);

  /**
   * @brief Handles retrieving global values.
   * @param symbol                    Symbol to retrieve.
   */
  Value::Any global(const Value::Symbol &symbol);

  /**
   * @brief Gets the intern associated with a symbol.
   * @param symbol                    Symbol to retrieve.
   */
  const String::Intern *intern(const Value::Symbol &symbol);

  /**
   * @brief Handles binding function receivers.
   * @param target                    Function to bind.
   * @param receiver                  Callback receiver.
   */
  Function::Any bind(const Function::Any &target, const Value::Any &receiver);

  /**
   * @brief Handles calling functions.
   * @param target                    Target to call.
   * @param args                      Arguments to bind.
   */
  Value::Any invoke(const Value::Any &target, const Function::Args &args = {});

  /**
   * @brief Handles spawning callbacks/futures.
   * @param target                    Target to spawn.
   * @param args                      Arguments to bind.
   * @param trace                     Optional trace.
   */
  Async::Result spawn(const Value::Any &target, const Resource::Trace &trace);
  Async::Result spawn(const Value::Any &target, Function::Args &&args = {}, const Resource::Trace &trace = {});

  /**
   * @brief Handles resolving runtime paths.
   * @param script                    Script value.
   * @param hint                      Directory hint.
   */
  Resource::Result resolve(const $::String::View &script, const $::FS::Path &hint = $::System::cwd()) const;

  /**
   * @brief Handles importing resources (blocking).
   * @param resource                  Resource to import.
   * @param trace                     Optional resource trace.
   */
  Value::Any import(const $::URI::Buffer &resource, const Resource::Trace &trace = {});
  Value::Any import(const $::String::View &script, const Resource::Trace &trace = {});
  Value::Any import(const $::String::View &script, const $::FS::Path &hint, const Resource::Trace &trace = {});

  /**
   * @brief Gets the available exports.
   * @param script                    Script value.
   * @param hint                      Directory hint.
   */
  Engine::Exports *exports(const $::URI::Buffer &resource);
  Engine::Exports *exports(const $::String::View &script, const $::FS::Path &hint = $::System::cwd());

  /**
   * @brief Handles throw "todo" exceptions.
   * @param reason                    Todo reasoning.
   */
  inline Value::Any todo(const String::Any &reason) { return todo(reason.view()); }
  inline Value::Any todo(const $::String::View &reason) { return panic(6000900, reason); }
  inline Value::Any todo() { return todo("The following code has not been implemented yet"); }

  /**
   * @brief Handles throwing an exception.
   * @param exception                 Exception to throw.
   */
  inline Value::Any panic(const Object::Exception &exception) { return m_panic(exception); }

  /**
   * @brief Handles throwing reasoned exceptions.
   * @param reason                    Reason to throw.
   */
  inline Value::Any panic(const Value::Any &reason) {
    if (reason.is<Value::Void>()) return panic(6000001); // the baseline exception
    else if (reason.is<String::Any>()) return panic(create<Object::Exception>(reason.as<String::Any>()));
    else if (reason.is<Object::Exception>()) return m_panic(reason.as<Object::Exception>());
    else return panic(fmt::to_string(reason)); // otherwise stringify as necessary now
  }

  /**
   * @brief Handles throwing an exception.
   * @param message                   Message to format.
   * @param args                      Exception arguments.
   */
  template <class... As> inline Value::Any panic(const $::String::View &message, As &&...args) {
    return m_panic(create<Object::Exception>(fmt::format(fmt::runtime(message), std::forward<As>(args)...)));
  }

  /**
   * @brief Handles throwing an exception.
   * @param code                      Diagnostic code.
   * @param args                      Exception arguments.
   */
  template <class... As> inline Value::Any panic(Diagnostic::Code code, As &&...args) {
    auto category = Diagnostic::Inspect::category(code);
    auto message = Diagnostic::Inspect::format(code, std::forward<As>(args)...);
    return m_panic(create<Object::Exception>(category, message));
  }

  /**
   * @brief Handles allocating a runtime stack.
   * @param size                      Size of stack.
   */
  inline Stack allocate(size_t size) { return Allocator::stack(this, size); }

  /**
   * @brief Handles constructing heap allocated values.
   * @param args                      Arguments to bind.
   */
  template <std::derived_from<Value::Any> T, class... As> inline T create(As &&...args) {
    return Allocator::create<T, As...>(this, std::forward<As>(args)...);
  }

private:
  //  PRIVATE METHODS  //

  /// @brief Gets a formatted backtrace value.
  std::vector<Resource::Trace> m_backtrace();

  /// @brief Gets the disposable lifetimes container.
  Garbage::Lifetimes *m_lifetimes() const noexcept;

  /**
   * @brief Handles scoping engine exports.
   * @param exports             Module exports to scope.
   */
  Handle::Scope m_scope(Engine::Exports *exports) noexcept;

  /**
   * @brief Binds an exception to the isolate.
   * @param exception         Exception to bind.
   */
  Value::Any m_panic(const Object::Exception &exception) noexcept;
};

} // namespace Talos::Runtime

//  SPECIALIZATIONS  //

template <std::derived_from<Talos::Value::Any> T>
Talos::Handle::Local<T>::Local(Scope *scope) :
    m_isolate(scope->m_isolate), m_next(m_isolate->m_handles.push(m_reference())) {}

template <std::derived_from<Talos::Value::Any> T>
Talos::Handle::Local<T>::Local(Scope *scope, const Value::Any &value) :
    m_value(value), m_isolate(scope->m_isolate), m_next(m_isolate->m_handles.push(m_reference())) {
  if constexpr (!std::same_as<T, Value::Any>) $_ASSERT(value.is<T>());
}

template <std::derived_from<Talos::Value::Any> T> Talos::Handle::Local<T>::~Local() {
  $_ASSERT(m_isolate->m_handles.head() == m_reference(), "Mismatched locals head");
  m_isolate->m_handles.pop(m_next); // pop the top-most head now as necessary
}

#endif
