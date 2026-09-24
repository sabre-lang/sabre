/// Shell Includes
#include "shell/validators/reporter.hpp"

//  PRIVATE METHODS  //

$::String::Buffer Shell::Validator::Reporter::m_check($::String::Buffer &input) {
  /// TODO: implement other reporters to enable normal ensurer
  // if (XT::Reporter::Ensure(input)) return {};
  if (input == XT::Reporter::Label::TEXT.view()) return {};
  else throw CLI::ValidationError("Unknown reporter");
}
