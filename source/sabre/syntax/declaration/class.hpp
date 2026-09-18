#ifndef _SABRE_DECLARATION_CLASS_HPP
#define _SABRE_DECLARATION_CLASS_HPP

/// Syntax Includes
#include "sabre/syntax/annotation/constructor.hpp"
#include "sabre/syntax/declaration/preamble.hpp"
#include "sabre/syntax/expression/call.hpp"
#include "sabre/syntax/statement/block.hpp"

namespace Sabre::Syntax {

/// @brief Object Fields Typing.
using Fields = std::vector<Declaration *>;

/// @brief Class Header Node.
class Header : public Mixin<Header> {
  //  PROPERTIES  //

  /// @brief The super-class expression.
  Call *m_super = nullptr;

  /// @brief The limited constructor signature.
  Constructor *m_constructor = nullptr;

  /// @brief Allowed interface implementations.
  Specialization m_implements = {};

public:
  //  CONSTRUCTORS  //

  /**
   * @brief Constructs a class header.
   * @param constructor             Constructor value.
   * @param super                   Super-class expression.
   * @param implements              Implementation interfaces.
   */
  explicit Header(Constructor *constructor = nullptr, Call *super = nullptr, const Specialization &implements = {})
      : m_super(super), m_constructor(constructor), m_implements(implements) {}

  //  PUBLIC METHODS  //

  inline constexpr const Call *super() const noexcept { return m_super; }
  inline constexpr const Expression *base() const noexcept { return m_super ? m_super->callee() : nullptr; }

  inline constexpr const Constructor *constructor() const noexcept { return m_constructor; }
  inline constexpr const Specialization &implements() const noexcept { return m_implements; }
};

/// @brief Class Syntax Node.
class Class : public Mixin<Class, Preamble> {
  //  PROPERTIES  //

  /// @brief Containing fields block.
  Block *m_block = nullptr;

  /// @brief The baseline class-header.
  Header *m_header = nullptr;

  /// @brief Attached property values.
  Fields m_fields = {};

public:
  //  CONSTRUCTORS  //

  /**
   * @brief Constructs a class node.
   * @param token                 Target name.
   */
  explicit Class(const Lexer::Token *token) : Mixin(token) {}
  explicit Class(const $::String::View &name) : Mixin(name) {}

  /**
   * @brief Constructs a class node.
   * @param name                    Target name.
   * @param header                  Class header.
   * @param fields                  Declaration fields.
   * @param block                   Declaration block.
   */
  explicit Class(const $::String::View &name, Header *header, const Fields &fields, Block *block)
      : Mixin(name), m_block(block), m_header(header), m_fields(fields) {}

  /**
   * @brief Constructs a class node.
   * @param token                   Target name.
   * @param header                  Class header.
   * @param fields                  Declaration fields.
   * @param block                   Declaration block.
   */
  explicit Class(const Lexer::Token *token, Header *header, const Fields &fields, Block *block)
      : Mixin(token), m_block(block), m_header(header), m_fields(fields) {}

  //  PUBLIC METHODS  //

  inline constexpr const Block *block() const noexcept { return m_block; }
  inline constexpr const Fields &fields() const noexcept { return m_fields; }
  inline constexpr const Header *header() const noexcept { return m_header; }
  inline constexpr const Call *super() const noexcept { return m_header->super(); }
  inline constexpr const Expression *base() const noexcept { return m_header->base(); }
  inline constexpr const Constructor *constructor() const noexcept { return m_header->constructor(); }
  inline constexpr const Specialization &implements() const noexcept { return m_header->implements(); }
};

} // namespace Sabre::Syntax

#endif
