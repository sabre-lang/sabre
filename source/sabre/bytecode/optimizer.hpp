#ifndef _SABRE_BYTECODE_OPTIMIZER_HPP
#define _SABRE_BYTECODE_OPTIMIZER_HPP

/// Sabre Includes
#include "sabre/bytecode/binder.hpp"
#include "sabre/bytecode/routine.hpp"
#include "sabre/runtime/options.hpp"

namespace Sabre::Bytecode {

/// @brief Compilation Optimizer.
class Optimizer : public XI::Transient {
  //  PROPERTIES  //

  /// @brief Underlying label allocator.
  Binder *m_labels;

  /// @brief Attached runtime options.
  const Runtime::Options *m_options;

public:
  //  CONSTRUCTORS  //

  /// @brief Constructs a bytecode optimizier.
  explicit Optimizer(Binder *labels);
  explicit Optimizer(XI::Container *services, Binder *labels);
  explicit Optimizer(Binder *labels, const Runtime::Options *options);

  //  PUBLIC METHODS  //

  /**
   * @brief Handles optimizing a given callable unit.
   * @param routine                   Routine to optimize.
   */
  void process(Routine *routine) const;

private:
  //  PRIVATE METHODS  //

  void m_build_graph(Routine *routine) const; // Builds the active functions CFG.
  void m_dump_graph(Routine *routine) const;  // Attempts dumping the CFG.

  void m_remove_dead_ir(Routine *routine) const;     // Removes any dead-instructions.
  void m_remove_dead_blocks(Routine *routine) const; // Removes all dead-blocks.

  void m_remove_empty_blocks(Routine *routine) const;      // Removes all empty-blocks.
  void m_remove_useless_jumps(Routine *routine) const;     // Removes all useless jumps.
  void m_rewrite_chained_branches(Routine *routine) const; // Re-writes chained branches.
};

} // namespace Sabre::Bytecode

#endif
