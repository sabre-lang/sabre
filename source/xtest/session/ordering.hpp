#ifndef _XTEST_SESSION_ORDERING_HPP
#define _XTEST_SESSION_ORDERING_HPP

/// Testing Includes
#include "xtest/forward/session.hpp"
#include "xtest/handle/mixin.hpp"

namespace XT::Session {

/// @brief Gets a suitable input seed value.
static inline constexpr uint32_t Seed() { return $::Random::Device::Standard()(); }

/// @brief Defines Input Ordering.
struct Ordering {
  //  PROPERTIES  //

  /// @brief Denotes if randomized testing.
  bool randomize = false;

  /// @brief Given seed for randomization.
  uint32_t seed = Seed();
};

/// @brief Randomness Generator Instance.
class Generator {
  //  PROPERTIES  //

  /// @brief Baseline generator options.
  const Ordering *m_ordering = nullptr;

  /// @brief Encapsulated randomness engine.
  $::Random::Engine::Twister m_engine = {};

public:
  //  CONSTRUCTORS  //

  /**
   * @brief Constructs a randomness generator.
   * @param ordering            Ordering to enable.
   */
  constexpr Generator(const Ordering *ordering = nullptr) : m_ordering(ordering) {
    if (m_ordering) m_engine.seed(m_ordering->seed);
  }

  //  PUBLIC METHODS  //

  /// @brief Denotes if ordering is randomized.
  inline constexpr bool randomize() const noexcept { return m_ordering ? m_ordering->randomize : false; }

  /**
   * @brief Handles shuffling inputs.
   * @param input               Input to shuffle.
   */
  template <std::derived_from<Handle::Base> T>
  inline constexpr std::vector<T *> shuffle(std::vector<T *> input, bool lexical = false) {
    if (randomize()) return std::ranges::shuffle(input, m_engine), std::move(input);
    else if (lexical) return std::ranges::sort(input, m_sort), std::move(input);
    else return std::move(input); // otherwise we chose not to shuffle our values
  }

private:
  //  PRIVATE METHODS  //

  /**
   * @brief Sorts inputs by lexical ordering.
   * @param left                Left most input.
   * @param right               Right most input.
   */
  static inline constexpr bool m_sort(const Handle::Base *left, const Handle::Base *right) {
    return left->title() < right->title();
  }
};

} // namespace XT::Session

#endif
