#ifndef _XTEST_HPP
#define _XTEST_HPP

/// Assert Includes
#include "xtest/assert/failure.hpp"
#include "xtest/assert/info.hpp"
#include "xtest/assert/reason.hpp"

/// Bench Includes
#include "xtest/bench/mark.hpp"
#include "xtest/bench/options.hpp"
#include "xtest/bench/result.hpp"
#include "xtest/bench/samples.hpp"

/// Bootstrap Includes
#include "xtest/bootstrap/launch.hpp"

/// Handle Includes
#include "xtest/handle/case.hpp"
#include "xtest/handle/group.hpp"
#include "xtest/handle/mixin.hpp"

/// Reporter Includes
#include "xtest/reporter/abstract.hpp"
#include "xtest/reporter/facade.hpp"
#include "xtest/reporter/options.hpp"
#include "xtest/reporter/proxy.hpp"
#include "xtest/reporter/stream.hpp"

/// Session Includes
#include "xtest/session/options.hpp"
#include "xtest/session/ordering.hpp"
#include "xtest/session/runner.hpp"
#include "xtest/session/storage.hpp"

/// Trivia Includes
#include "xtest/trivia/location.hpp"
#include "xtest/trivia/metadata.hpp"
#include "xtest/trivia/statistics.hpp"

#endif
