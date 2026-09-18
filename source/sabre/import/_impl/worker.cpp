/// Sabre Includes
#include "sabre/import/worker.hpp"
#include "sabre/import/service.hpp"
#include "sabre/runtime/container.hpp"
#include "sabre/runtime/isolate.hpp"

//  CONSTRUCTORS  //

Sabre::Import::Worker::Worker(Result *deferred, const $::URI::Buffer &resource) :
    Worker($::Global::get<Runtime::Container>(), deferred, resource) {}

Sabre::Import::Worker::Worker(XI::Container *services, Result *deferred, const $::URI::Buffer &resource) :
    m_resource(resource), m_modules(*services), m_deferred(deferred) {}

//  PRIVATE METHODS  //

void Sabre::Import::Worker::m_execute() { m_deferred->resolve(m_thread, m_modules->fetch(m_resource)); }
