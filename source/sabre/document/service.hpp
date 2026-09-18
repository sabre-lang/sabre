#ifndef _SABRE_DOCUMENT_SERVICE_HPP
#define _SABRE_DOCUMENT_SERVICE_HPP

/// Sabre Includes
#include "sabre/document/buffer.hpp"
#include "sabre/document/publisher.hpp"
#include "sabre/value/symbol.hpp"

namespace Sabre::Document {

/// @brief Document Service.
class Service : public XI::Singleton {
  //  TYPEDEFS  //

  /// @brief Key Storage Typing.
  using Key = Value::Symbol;

  /// @brief Buffer Value Typing.
  using Value = $::Shared::Pointer<Buffer>;

  //  PROPERTIES  //

  /// @brief Documents mutex value.
  mutable $::Mutex::Auto m_mutex;

  /// @brief Publisher instance.
  $::Unique::Pointer<Publisher> m_publisher;

  /// @brief Encapsulates document buffers.
  $::Map::Base<Key, Value> m_buffers = {};

public:
  //  CONSTRUCTORS  //

  /**
   * @brief Constructs a document service.
   * @param services                  Services container.
   */
  explicit Service();
  explicit Service(XI::Container *services);

  //  PUBLIC METHODS  //

  /// @brief Removes all available buffers.
  inline constexpr void clear() { m_buffers.clear(); }

  /// @brief Gets the publisher instance.
  inline constexpr Publisher *publisher() const noexcept { return m_publisher.get(); }

  /// @brief Gets a view of the underlying resources.
  inline constexpr std::vector<$::URI::Buffer> opened() const noexcept {
    // get the cached result of buffers
    auto result = std::vector<$::URI::Buffer>();

    // emplace all the resources available now
    for (const auto &buffer : m_buffers | std::views::values) {
      if (buffer->anonymous()) continue; // ignore
      result.emplace_back(buffer->resource().buffer());
    }

    // return the resulting resources now
    return result;
  }

  /**
   * @brief Checks if a resource exists.
   * @param resource                  Document resource.
   */
  inline constexpr bool contains(const $::URI::View &resource) const { return m_buffers.contains(resource); }

  /**
   * @brief Handles resolving a document buffer.
   * @param resource                  Document resource.
   */
  inline constexpr $::Shared::Pointer<Buffer> resolve(const $::URI::View &resource) const {
    $_UNUSED $_AUTO = $::Lock::guard(m_mutex); // prepare mutex
    if (m_buffers.contains(resource)) return m_buffers.at(resource);
    return $::Shared::New<Buffer>(resource); // non-cached buffer
  }

  /**
   * @brief Handles updating a document buffer.
   * @param resource                  Document resource.
   * @param content                   Content to bind.
   */
  inline constexpr void update(const $::URI::View &resource, const $::String::Buffer &content) {
    $_UNUSED $_AUTO = $::Lock::guard(m_mutex); // lock mutex here
    m_buffers[resource] = $::Shared::New<Buffer>(content, resource);
    m_publisher->refresh(resource); // and force a publisher update
  }

  /**
   * @brief Handles removing a resource.
   * @param resource                  Document resource.
   */
  inline constexpr void remove(const $::URI::View &resource) {
    $_UNUSED $_AUTO = $::Lock::guard(m_mutex); // prepare
    m_buffers.erase(resource), m_publisher->clear(resource);
  }
};

} // namespace Sabre::Document

#endif
