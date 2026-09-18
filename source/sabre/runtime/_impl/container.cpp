/// Sabre Includes
#include "sabre/runtime/container.hpp"
#include "sabre/runtime/options.hpp"
#include "sabre/type/world.hpp"

/// Service Includes
#include "sabre/async/service.hpp"
#include "sabre/crate/service.hpp"
#include "sabre/document/service.hpp"
#include "sabre/garbage/service.hpp"
#include "sabre/globals/service.hpp"
#include "sabre/heap/service.hpp"
#include "sabre/import/service.hpp"
#include "sabre/lifecycle/service.hpp"
#include "sabre/locale/service.hpp"
#include "sabre/machine/service.hpp"
#include "sabre/relint/service.hpp"
#include "sabre/shape/service.hpp"
#include "sabre/signal/service.hpp"

//  CONSTRUCTORS  //

Sabre::Runtime::Container::Container() : Container({}) {}
Sabre::Runtime::Container::Container(const Options &options) {
  // bind the options to the service
  bind<Options>(options);

  // bind all the common services for the container
  bind<Async::Service>();
  bind<Locale::Service>();
  bind<Signal::Service>();
  bind<Import::Service>();
  bind<Crate::Service>();
  bind<Shape::Service>();
  bind<Relint::Service>();
  bind<Document::Service>();
  bind<Lifecycle::Service>();

  // post-bind the runtime specialized services
  bind<Heap::Service>();
  bind<Garbage::Service>();
  bind<Globals::Service>();
  bind<Machine::Service>();
}
