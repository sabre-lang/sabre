#ifndef _SABRE_MEMBER_STORAGE_HPP
#define _SABRE_MEMBER_STORAGE_HPP

/// Sabre Includes
#include "sabre/function/native.hpp"
#include "sabre/member/property.hpp"
#include "sabre/member/reference.hpp"
#include "sabre/runtime/isolate.hpp"
#include "sabre/value/symbol.hpp"

namespace Sabre::Member {

/// @brief Property Storage Container.
struct Storage {
  //  TYPEDEFS  //

  /// @brief Entry typing to cccept.
  using Entry = std::pair<$::String::View, Function::Callback>;

  /// @brief Initializer fields typing.
  using Initializer = std::vector<Entry>;

private:
  //  PROPERTIES  //

  /// @brief Cached properties that were used as factories for others.
  $::Map::Base<Value::Symbol, $::Unique::Pointer<Descriptor>> m_cache = {};

  /// @brief All available properties that have been constructed.
  $::Map::Base<Value::Symbol, $::Unique::Pointer<Descriptor>> m_members = {};

public:
  //  CONSTRUCTORS  //

  /// @brief Constructs an empty storage instance.
  explicit constexpr Storage() = default;

  /**
   * @brief Constructs a storage instance.
   * @param name                  Base storage name.
   * @param fields                Fields to be bound.
   */
  explicit constexpr Storage(const $::String::View &name, const Initializer &fields) : m_members(fields.size()) {
    for (const auto &[field, callback] : fields) m_members.emplace(m_factory(name, field, callback));
  }

  //  PUBLIC METHODS  //

  /**
   * @brief Checks if a symbol exists.
   * @param symbol                Field symbol.
   */
  inline constexpr bool contains(const Value::Symbol &symbol) const noexcept { return m_members.contains(symbol); }

  /**
   * @brief Handles looking up a storage field.
   * @param symbol                Symbol to resolve.
   */
  inline constexpr View retrieve(const Value::Symbol &symbol) noexcept {
    auto iter = m_members.find(symbol); // attempt resolving
    return iter == m_members.cend() ? nullptr : iter->second.get();
  }

private:
  //  PRIVATE METHODS  //

  /**
   * @brief Handles constructing properties.
   * @param name                  Storage name.
   * @param field                 Field to bind.
   * @param callback              Native callback.
   */
  inline constexpr std::pair<Value::Symbol, $::Unique::Pointer<Property>>
  m_factory(const $::String::View &name, const $::String::View &field, Function::Callback callback) noexcept {
    // prepare the symbol to be used
    auto symbol = Value::Symbol(field);

    // prepare the property getter to be used now
    auto getter = Getter([=, this](Runtime::Isolate *isolate, auto) -> Value::Any {
      Value::Any value = isolate->create<Function::Native>(callback, name, field);
      m_cache.emplace(symbol, std::move(m_members.at(symbol))); // cache old property
      return (m_members[symbol] = $::Unique::New<Reference>(field, value), value);
    });

    // and construct the resulting entry now
    return {symbol, $::Unique::New<Property>(std::move(getter))};
  };
};

} // namespace Sabre::Member

#endif
