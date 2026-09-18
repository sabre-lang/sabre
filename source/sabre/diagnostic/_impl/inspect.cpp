/// Sabre Includes
#include "sabre/diagnostic/inspect.hpp"

/// Forward Declarations
$_FWD(Sabre::Diagnostic::Inspect, template <Code C, Severity S> bool overridable())

//  PUBLIC METHODS  //

template <Sabre::Diagnostic::Code C, Sabre::Diagnostic::Severity S> bool Sabre::Diagnostic::Inspect::overridable() {
  return C < 9000000 && S != Severity::ERROR; // we only allow overriding codes that are non-errors
}

bool Sabre::Diagnostic::Inspect::overridable(Code code) {
  // attempt checking if the value fits an overridable one
  switch (code) {
#define SABRE_XX_DIAGNOSTIC_CODE(_, N, S, ...)    \
  case (N): return overridable<N, Severity::S>();
#include "sabre/diagnostic/_defines/codes.def"
  default: return false;
  }
}

Sabre::Diagnostic::Tags Sabre::Diagnostic::Inspect::tags(Code code) {
  switch (code) {
#define SABRE_XX_DIAGNOSTIC_CODE(_, N, __, ___, ...) \
  case N: return Tags(__VA_ARGS__);
#include "sabre/diagnostic/_defines/codes.def"
  default: return Tags();
  }
}

Sabre::Diagnostic::Severity Sabre::Diagnostic::Inspect::severity(Code code) {
  switch (code) {
#define SABRE_XX_DIAGNOSTIC_CODE(_, N, S, ...) \
  case N: return Severity::S;
#include "sabre/diagnostic/_defines/codes.def"
  default: return Severity::NONE;
  }
}

$::String::View Sabre::Diagnostic::Inspect::message(Code code) {
  switch (code) {
#define SABRE_XX_DIAGNOSTIC_CODE(_, N, __, M, ...) \
  case N: return M;
#include "sabre/diagnostic/_defines/codes.def"
  default: $_ABORT("Unknown diagnostic code: {0}", code);
  }
}

$::String::View Sabre::Diagnostic::Inspect::category(Code code) {
  switch (code) {
#define SABRE_XX_DIAGNOSTIC_CODE(C, N, S, ...) \
  case N: return C;
#include "sabre/diagnostic/_defines/codes.def"
  default: return "Unknown";
  }
}
