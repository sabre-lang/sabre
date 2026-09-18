#ifndef _SABRE_ARCHIVE_RECORDS_HPP
#define _SABRE_ARCHIVE_RECORDS_HPP

/// Sabre Includes
#include "sabre/forward/image.hpp"

namespace Sabre::Image {

/// @brief Records Partition Value.
struct Partition {
  size_t offset;           // Mapped offset.
  XLSP::Position position; // Position value.
};

/// @brief Image Position Records.
class Records {
  //  TYPEDEFS  //

  /// @brief Scanning Result.
  struct View {
    const Partition *before;
    const Partition *after;
  };

  //  PROPERTIES  //

  /// @brief Available record entries.
  std::vector<Partition> m_partitions = {};

public:
  //  CONSTRUCTORS  //

  /// @brief Default constructor.
  constexpr Records() = default;

  //  PUBLIC METHODS  //

  inline constexpr bool empty() const noexcept { return m_partitions.empty(); }
  inline constexpr size_t size() const noexcept { return m_partitions.size(); }

  /// @brief Gets the maximum partition widths.
  inline constexpr size_t width() const noexcept {
    if (m_partitions.empty()) return 0; // return early when there are no partitions available
    static auto eval = [](const auto &partition) { return fmt::to_string(partition.position).size(); };
    static auto comp = [](const auto &a, const auto &b) { return eval(a) < eval(b); };
    return fmt::to_string(std::ranges::max_element(m_partitions, comp)->position).size();
  }

  inline constexpr const Partition *before(size_t offset) const noexcept { return m_scan(offset).before; }
  inline constexpr const Partition *after(size_t offset) const noexcept { return m_scan(offset).after; }

  /**
   * @brief Constructs a suitable position.
   * @param offset            Bytecode offset.
   * @param position          Position to bind.
   */
  inline constexpr const Partition &append(size_t offset, const XLSP::Position &position) {
    return m_partitions.emplace_back(Partition{.offset = offset, .position = position});
  }

private:
  //  PRIVATE METHODS  //

  /**
   * @brief Scans for a suitable view.
   * @param offset            Bytecode offset.
   */
  View m_scan(size_t offset) const noexcept;
};

} // namespace Sabre::Image

#endif
