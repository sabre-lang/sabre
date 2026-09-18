#ifndef _SABRE_FORWARD_DIAGNOSTIC_HPP
#define _SABRE_FORWARD_DIAGNOSTIC_HPP

/// Vendor Includes
#include <xinv/xinv.hpp>
#include <xlsp/xlsp.hpp>

/// Forward Declarations
$_FWD(Sabre::Diagnostic, class View)
$_FWD(Sabre::Diagnostic, class Scope)
$_FWD(Sabre::Diagnostic, class Reporter)
$_FWD(Sabre::Diagnostic, struct Options)

/// Forward Definitions
$_FWD(Sabre::Diagnostic, using Code = XLSP::Diagnostic::Code)
$_FWD(Sabre::Diagnostic, using Severity = XLSP::Diagnostic::Severity)
$_FWD(Sabre::Diagnostic, using Tags = $::Enum::Flags<XLSP::Diagnostic::Tag>)

$_FWD(Sabre::Diagnostic, static constexpr auto Deprecated = XLSP::Diagnostic::Tag::DEPRECATED)
$_FWD(Sabre::Diagnostic, static constexpr auto Unnecessary = XLSP::Diagnostic::Tag::UNNECESSARY)

#endif
