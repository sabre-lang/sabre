#ifndef _SABRE_BYTECODE_BLOCK_HPP
#define _SABRE_BYTECODE_BLOCK_HPP

/// Sabre Includes
#include "sabre/bytecode/instruction.hpp"

namespace Sabre::Bytecode {

/// @brief Instruction Mappings.
class Mapping {
  //  PROPERTIES  //

  /// @brief Handles instruction storage.
  $::Map::Base<const Instruction *, XLSP::Position> m_positions = {};

public:
  //  CONSTRUCTORS  //

  /// @brief Constructs an instruction cache.
  constexpr Mapping() = default;

  //  PUBLIC METHODS  //

  /// @brief Handles clearing the current positions available.
  inline constexpr void clear() { m_positions.clear(); }

  /**
   * @brief Resolves a suitable position value.
   * @param instruction               Instruction to retrieve.
   */
  inline constexpr const XLSP::Position *get(const Instruction *instruction) const noexcept {
    return m_positions.contains(instruction) ? &m_positions.at(instruction) : nullptr;
  }

  /**
   * @brief Handles emplacing an instruction.
   * @param instruction               Instruction to emplace.
   * @param position                  Associated position.
   */
  inline constexpr void set(const Instruction *instruction, const XLSP::Position &position = {}) {
    m_positions.emplace(instruction, position);
  }
};

/// @brief Basic Block Structure.
class Block {
  //  PROPERTIES  //

  /// @brief Denotes if a block is reachable.
  bool m_reachable = false;

  /// @brief Bound block labels.
  $::Map::Set<Label> m_labels = {};

  /// @brief Incoming block references.
  $::Map::Set<Block *> m_incoming = {};

  /// @brief Outgoing block references.
  $::Map::Set<Block *> m_outgoing = {};

  /// @brief All block instructions available.
  std::vector<Boxed> m_instructions = {};

public:
  //  CONSTRUCTORS  //

  /// @brief Constructs an empty block instance.
  constexpr Block() = default;

  //  PUBLIC METHODS  //

  /// @brief Denotes if a block is actually reachable.
  inline constexpr bool &reachable() noexcept { return m_reachable; }
  inline constexpr bool reachable() const noexcept { return m_reachable; }

  /// @brief Denotes if the block is empty.
  inline constexpr bool empty() const noexcept { return m_instructions.empty(); }

  /// @brief Gets the size of the block.
  inline constexpr size_t size() const noexcept { return m_instructions.size(); }

  /// @brief Gets a blocks available labels.
  inline constexpr $::Map::Set<Label> &labels() noexcept { return m_labels; }
  inline constexpr const $::Map::Set<Label> &labels() const noexcept { return m_labels; }

  /// @brief Gets the available instructions.
  inline constexpr auto &instructions() noexcept { return m_instructions; }
  inline constexpr const auto &instructions() const noexcept { return m_instructions; }

  /// @brief Gets the incoming block set.
  inline constexpr const $::Map::Set<Block *> &incoming() const noexcept { return m_incoming; }

  /// @brief Gets the outgoing block set.
  inline constexpr const $::Map::Set<Block *> &outgoing() const noexcept { return m_outgoing; }

  /**
   * @brief Links a target block to this one.
   * @param target                Target to link.
   */
  inline void link(Block *target) { m_outgoing.insert(target), target->m_incoming.insert(this); }

  /// @brief Unlinks all references to this block.
  inline void unlink() {
    for (auto *block : m_incoming) block->m_outgoing.erase(this);
    for (auto *block : m_outgoing) block->m_incoming.erase(this);
  }

  /**
   * @brief Unlinks a target block to this one.
   * @param target                Target to unlink.
   */
  inline void unlink(Block *target) { m_outgoing.erase(target), target->m_incoming.erase(this); }

protected:
  //  PRIVATE METHODS  //

  /**
   * @brief Handles printing bytecode blocks.
   * @param os                    Output stream.
   * @param self                  Block instance.
   */
  static void m_print(std::ostream &os, const Block &self);
};

} // namespace Sabre::Bytecode

#endif
