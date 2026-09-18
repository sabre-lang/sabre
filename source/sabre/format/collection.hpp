#ifndef _SABRE_FORMAT_COLLECTION_HPP
#define _SABRE_FORMAT_COLLECTION_HPP

/// Sabre Includes
#include "sabre/format/storage.hpp"

namespace Sabre::Format {

/// @brief Piece Collection.
class Collection {
  //  TYPEDEFS  //

  /// @brief Allow storage internal access.
  friend class Storage;

  /// @brief Prepare the whitespace typing now.
  using Whitespace = $::Map::Base<size_t, Buffer>;

  //  PROPERTIES  //

  /// @brief Separator to be used.
  Piece *m_separator = nullptr;

  /// @brief The current segments available.
  Buffer m_buffer = {};

  /// @brief Currently available leading comments.
  Whitespace m_leading = {};

  /// @brief Currently available trailing comments.
  $::Map::Base<size_t, Piece *> m_trailing = {};

public:
  //  CONSTRUCTORS  //

  /**
   * @brief Constructs a delimited collection.
   * @param separator             Separator to bind.
   */
  explicit Collection(Piece *separator = nullptr) : m_separator(separator) {}

  //  PUBLIC METHODS  //

  /// @brief Denotes if collections are trimmable at all.
  inline constexpr bool trimmable() const noexcept { return m_separator != nullptr; }

  /// @brief Optional collection separator.
  inline constexpr Piece *&separator() noexcept { return m_separator; }
  inline constexpr Piece *separator() const noexcept { return m_separator; }

  /**
   * @brief Handles appending whitespace.
   * @param piece                  Piece instance.
   */
  void whitespace(Piece *piece) noexcept;

  /**
   * @brief Handles appending a collection item.
   * @param item                  Item instance.
   * @param trailing              Trailing comment.
   */
  bool append(Piece *item, Piece *trailing = nullptr) noexcept;

  /**
   * @brief Handles merging the collection together.
   * @param storage               Storage instance.
   * @param trailing              Allow trailing separators.
   */
  Piece::List *merge(Storage *storage, bool trailing = false) const noexcept;

private:
  //  PRIVATE METHODS  //

  /**
   * @brief Handles trimming leading whitespace.
   * @param whitespace            Whitespace to trim.
   */
  Whitespace m_trim(Whitespace whitespace) const noexcept;

  /**
   * @brief Handles flushing delimiters.
   * @param storage               Storage instance.
   * @param index                 Index of piece.
   * @param trailing              Allow trailing separators.
   */
  Piece *m_flush(Storage *storage, size_t index, bool trailing = true) const noexcept;

  /**
   * @brief Handles constructing suffixes.
   * @param storage               Storage instance.
   * @param index                 Index of piece.
   * @param trailing              Allow trailing separators.
   */
  Piece *m_suffix(Storage *storage, size_t index, bool trailing = true) const noexcept;
};

} // namespace Sabre::Format

#endif
