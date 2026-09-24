#ifndef _XTEST_HANDLE_MIXIN_HPP
#define _XTEST_HANDLE_MIXIN_HPP

/// Testing Includes
#include "xtest/forward/handle.hpp"
#include "xtest/forward/session.hpp"
#include "xtest/trivia/metadata.hpp"

namespace XT::Handle {

/// @brief Handle Callback.
using Callback = $::Unique::Functor<void(Session::Runner *) const>;

/// @brief Abstracted Test Handle.
class $_ABSTRACT Base {
  //  PROPERTIES  //

  /// @brief Associated metadata.
  $::Unique::Pointer<Trivia::Metadata> m_trivia = nullptr;

public:
  //  CONSTRUCTORS  //

  /**
   * @brief Constructs a test abstraction.
   * @param trivia              Metadata to bind.
   */
  constexpr Base($::Unique::Pointer<Trivia::Metadata> &&trivia) : m_trivia(std::move(trivia)) {}

  /// @brief Virtual abstract destructor.
  virtual ~Base() = default;

  //  PUBLIC METHODS  //

  /// @brief Gets the total count of tests included.
  inline constexpr size_t count() const noexcept { return m_count(); }

  /// @brief Gets the title of the test.
  inline constexpr $::String::View title() const noexcept { return m_trivia->title; }

  /// @brief Gets the trivia metadata.
  inline constexpr Trivia::Metadata *trivia() noexcept { return m_trivia.get(); }
  inline constexpr const Trivia::Metadata *trivia() const noexcept { return m_trivia.get(); }

  /// @brief Gets the current todo state.
  inline constexpr bool todo() const noexcept { return m_trivia->todo; }

  /// @brief Gets the current skip state.
  inline constexpr bool skip() const noexcept { return m_trivia->skip; }

  /**
   * @brief Handles executing a test.
   * @param runner              Session runner.
   */
  inline void execute(Session::Runner *runner) const {
    if (!m_bailout(runner)) m_execute(runner);
  }

protected:
  //  PRIVATE METHODS  //

  /// @brief Gets the total count of tests included.
  virtual size_t m_count() const noexcept = 0;

  /**
   * @brief Handles executing a test.
   * @param runner              Session runner.
   */
  virtual void m_execute(Session::Runner *runner) const = 0;

  /**
   * @brief Checks the current bailout conditions.
   * @param runner              Session runner.
   */
  virtual bool m_bailout(Session::Runner *runner) const noexcept;
};

/// @brief Allows introspection of a test abstraction.
template <class T> struct $_ABSTRACT Mixin : public Base {
  //  CONSTRUCTORS  //

  /**
   * @brief Constructs a test abstraction.
   * @param title               Title of test.
   * @param location            Test location.
   */
  constexpr Mixin(const $::String::Buffer &title, const Trivia::Location &location = {}) :
      Base($::Unique::From(new Trivia::Metadata({.location = location, .title = title}))) {}

  //  PUBLIC METHODS  //

  inline constexpr T *todo(bool state) noexcept { return trivia()->todo = state, static_cast<T *>(this); }
  inline constexpr T *skip(bool state) noexcept { return trivia()->skip = state, static_cast<T *>(this); }
};

} // namespace XT::Handle

#endif
