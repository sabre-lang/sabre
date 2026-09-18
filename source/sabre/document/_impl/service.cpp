/// Sabre Includes
#include "sabre/document/service.hpp"
#include "sabre/runtime/container.hpp"

//  CONSTRUCTORS  //

Sabre::Document::Service::Service() : Service($::Global::get<Runtime::Container>()) {}
Sabre::Document::Service::Service(XI::Container *services) : m_publisher(*services) {}
