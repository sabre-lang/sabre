/// Sabre Includes
#include "sabre/relint/context.hpp"
#include "sabre/runtime/container.hpp"

//  CONSTRUCTORS  //

Sabre::Relint::Context::Context() : Context($::Global::get<Runtime::Container>()) {}
Sabre::Relint::Context::Context(XI::Container *services) : m_services(services) {}
