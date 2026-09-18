#ifndef _SABRE_IMPORT_LOADER_HPP
#define _SABRE_IMPORT_LOADER_HPP

/// Sabre Includes
#include "sabre/module/abstract.hpp"
#include "sabre/resource/scheme.hpp"

namespace Sabre::Import {

/// @brief Import Loader Abstraction.
struct $_ABSTRACT Loader : public XI::Transient {
  //  TYPEDEFS  //

  /// @brief Simple Loader Definition.
  template <$::String::Literal S, class T, class B = Loader> struct Define : public B {
    //  TYPEDEFS  //

    /// @brief Ensure the definition is valid.
    static_assert(std::derived_from<B, Loader>);
    static_assert(std::derived_from<T, Module::Abstract>);

    //  PUBLIC METHODS  //

    /// @brief Gets the associated scheme.
    inline constexpr $::String::View scheme() const noexcept final { return S; }

    /**
     * @brief Handles fetching a module.
     * @param services                  Services container.
     * @param resource                  Resource to fetch.
     */
    $::Unique::Pointer<Module::Abstract> fetch(XI::Container *services, const $::URI::Buffer &resource) const final {
      return services->get<T>(resource);
    }
  };

  //  CONSTRUCTORS  //

  /// @brief Virtual abstract destructor.
  virtual ~Loader() = default;

  //  PUBLIC METHODS  //

  /// @brief The underlying loader scheme.
  virtual $::String::View scheme() const noexcept = 0;

  /**
   * @brief Handles resolving loader resources.
   * @param body                Resource body.
   * @param hint                Resource hint.
   */
  virtual Resource::Result resolve(const $::String::View &body, const $::FS::Path &) const noexcept {
    return $::URI::Buffer(scheme(), body);
  }

  /**
   * @brief Handles fetching a module.
   * @param services                  Services container.
   * @param resource                  Resource to fetch.
   */
  virtual $::Unique::Pointer<Module::Abstract> fetch(XI::Container *services, const $::URI::Buffer &resource) const = 0;
};

} // namespace Sabre::Import

#endif
