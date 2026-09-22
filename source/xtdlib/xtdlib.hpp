#ifndef _XTDLIB_HPP
#define _XTDLIB_HPP

/// Async Includes
#include "xtdlib/async/atomic.hpp"
#include "xtdlib/async/signaller.hpp"

/// Chrono Includes
#include "xtdlib/chrono/clock.hpp"
#include "xtdlib/chrono/duration.hpp"
#include "xtdlib/chrono/measure.hpp"
#include "xtdlib/chrono/point.hpp"

/// Color Includes
#include "xtdlib/color/ansi.hpp"
#include "xtdlib/color/enabled.hpp"

/// Container Includes
#include "xtdlib/container/flags.hpp"
#include "xtdlib/container/map.hpp"
#include "xtdlib/container/ranges.hpp"

/// Debug Includes
#include "xtdlib/debug/assert.hpp"
#include "xtdlib/debug/location.hpp"
#include "xtdlib/debug/logger.hpp"
#include "xtdlib/debug/output.hpp"
#include "xtdlib/debug/printable.hpp"

/// Encoding Includes
#include "xtdlib/encoding/ascii.hpp"
#include "xtdlib/encoding/hex.hpp"
#include "xtdlib/encoding/utf8.hpp"

/// Enum Includes
#include "xtdlib/enum/class.hpp"
#include "xtdlib/enum/flags.hpp"

/// Filesystem Includes
#include "xtdlib/filesystem/chmod.hpp"
#include "xtdlib/filesystem/compare.hpp"
#include "xtdlib/filesystem/path.hpp"
#include "xtdlib/filesystem/read.hpp"
#include "xtdlib/filesystem/unique.hpp"
#include "xtdlib/filesystem/write.hpp"

/// Glob Includes
#include "xtdlib/glob/match.hpp"
#include "xtdlib/glob/pattern.hpp"

/// Lambda Includes
#include "xtdlib/lambda/defer.hpp"
#include "xtdlib/lambda/shared.hpp"
#include "xtdlib/lambda/signature.hpp"
#include "xtdlib/lambda/unique.hpp"
#include "xtdlib/lambda/visitor.hpp"

/// Macro Includes
#include "xtdlib/macros/attributes.hpp"
#include "xtdlib/macros/forward.hpp"
#include "xtdlib/macros/processor.hpp"

/// Manip Includes
#include "xtdlib/manip/indent.hpp"

/// Math Includes
#include "xtdlib/math/saturating.hpp"

/// Memory Includes
#include "xtdlib/memory/constants.hpp"
#include "xtdlib/memory/mapping.hpp"

/// Mutex Includes
#include "xtdlib/mutex/guard.hpp"
#include "xtdlib/mutex/light.hpp"
#include "xtdlib/mutex/lock.hpp"
#include "xtdlib/mutex/types.hpp"

/// Notate Includes
#include "xtdlib/notate/options.hpp"
#include "xtdlib/notate/parse.hpp"
#include "xtdlib/notate/section.hpp"

/// Pointer Includes
#include "xtdlib/pointer/shared.hpp"
#include "xtdlib/pointer/unique.hpp"

/// Random Includes
#include "xtdlib/random/engine.hpp"
#include "xtdlib/random/interface.hpp"
#include "xtdlib/random/simple.hpp"
#include "xtdlib/random/standard.hpp"

/// RTTI Includes
#include "xtdlib/rtti/hash.hpp"
#include "xtdlib/rtti/mixin.hpp"
#include "xtdlib/rtti/name.hpp"

/// Serde Includes
#include "xtdlib/serde/codec.hpp"
#include "xtdlib/serde/escape.hpp"
#include "xtdlib/serde/json.hpp"
#include "xtdlib/serde/toml.hpp"
#include "xtdlib/serde/value.hpp"
#include "xtdlib/serde/yaml.hpp"

/// Signal Includes
#include "xtdlib/signal/code.hpp"
#include "xtdlib/signal/handler.hpp"

/// Spinner Includes
#include "xtdlib/spinner/abstract.hpp"
#include "xtdlib/spinner/cursor.hpp"
#include "xtdlib/spinner/enabled.hpp"
#include "xtdlib/spinner/frames.hpp"
#include "xtdlib/spinner/mode.hpp"
#include "xtdlib/spinner/options.hpp"
#include "xtdlib/spinner/suffix.hpp"

/// String Includes
#include "xtdlib/string/buffer.hpp"
#include "xtdlib/string/convert.hpp"
#include "xtdlib/string/literal.hpp"
#include "xtdlib/string/trim.hpp"
#include "xtdlib/string/view.hpp"

/// System Includes
#include "xtdlib/system/abort.hpp"
#include "xtdlib/system/architecture.hpp"
#include "xtdlib/system/debugbreak.hpp"
#include "xtdlib/system/environment.hpp"
#include "xtdlib/system/executable.hpp"
#include "xtdlib/system/information.hpp"
#include "xtdlib/system/platform.hpp"
#include "xtdlib/system/process.hpp"
#include "xtdlib/system/switch.hpp"
#include "xtdlib/system/unreachable.hpp"

/// Terminal Includes
#include "xtdlib/terminal/a11y.hpp"
#include "xtdlib/terminal/core.hpp"

/// Unit Includes
#include "xtdlib/unit/strong.hpp"
#include "xtdlib/unit/ternary.hpp"

/// URI Includes
#include "xtdlib/uri/alias.hpp"
#include "xtdlib/uri/buffer.hpp"
#include "xtdlib/uri/parse.hpp"
#include "xtdlib/uri/scheme.hpp"
#include "xtdlib/uri/view.hpp"

/// Utility Includes
#include "xtdlib/utility/global.hpp"
#include "xtdlib/utility/property.hpp"
#include "xtdlib/utility/singleton.hpp"
#include "xtdlib/utility/static.hpp"

#endif
