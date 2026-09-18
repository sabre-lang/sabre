#ifndef _SABRE_FORWARD_SYNTAX_HPP
#define _SABRE_FORWARD_SYNTAX_HPP

/// Vendor Includes
#include <xlsp/xlsp.hpp>

/// Forward Declarations
$_FWD(Sabre::Syntax, class Node)
$_FWD(Sabre::Syntax, class Tree)
$_FWD(Sabre::Syntax, class Bounds)
$_FWD(Sabre::Syntax, class Trivia)
$_FWD(Sabre::Syntax, class Storage)
$_FWD(Sabre::Syntax, struct Generator)

$_FWD(Sabre::Syntax, struct Annotation)
$_FWD(Sabre::Syntax, class Signature)
$_FWD(Sabre::Syntax, struct Parameters)
$_FWD(Sabre::Syntax, class Constructor)
$_FWD(Sabre::Syntax, class Placeholder)

$_FWD(Sabre::Syntax, class Preamble)
$_FWD(Sabre::Syntax, class Declaration)
$_FWD(Sabre::Syntax, struct Variable)
$_FWD(Sabre::Syntax, class Attribute)
$_FWD(Sabre::Syntax, class Decorator)

$_FWD(Sabre::Syntax, struct Expression)
$_FWD(Sabre::Syntax, class Identifier)

$_FWD(Sabre::Syntax, struct Statement)
$_FWD(Sabre::Syntax, struct Unterminated{})

/// Forward Definitions
$_FWD(Sabre::Syntax, using Template = std::vector<Placeholder *>)
$_FWD(Sabre::Syntax, using Specialization = std::vector<Annotation *>)
$_FWD(Sabre::Syntax, using Dependencies = std::vector<$::String::View>)

#endif
