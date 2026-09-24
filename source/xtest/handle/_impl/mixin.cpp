/// Testing Includes
#include "xtest/handle/mixin.hpp"
#include "xtest/session/runner.hpp"

//  PRIVATE METHODS  //

bool XT::Handle::Base::m_bailout(Session::Runner *runner) const noexcept { return runner->bailout(); }
