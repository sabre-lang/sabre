#ifndef _SABRE_MACHINE_MACROS_IPP
#define _SABRE_MACHINE_MACROS_IPP

/// Sabre Includes
#include "sabre/machine/visitor.hpp"

/// Machine Includes
#include "sabre/machine/_inline/glue.ipp"

//  MACROS  //

/// @brief We expose our compiler methods with a specialized prefix.
#define __cc__ builder->compiler->

/// @brief We expose our emitter methods with a specialized prefix.
#define __ee__ builder->emitter->

#define SABRE_MM_MACHINE_EMIT(N, B, I, ...)                         \
  template <>                                                       \
  void Sabre::Machine::Visitor::accept<Sabre::Bytecode::Glyph::N>(  \
      Builder * B, const Bytecode::Qualified<Bytecode::Glyph::N> *I \
  )

#endif
