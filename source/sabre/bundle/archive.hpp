#ifndef _SABRE_BUNDLE_ARCHIVE_HPP
#define _SABRE_BUNDLE_ARCHIVE_HPP

/// Sabre Includes
#include "sabre/bundle/codec.hpp"
#include "sabre/forward/import.hpp"

namespace Sabre::Bundle {

/// @brief Bundled Archive Service.
class Archive : public XI::Transient {
  //  PROPERTIES  //

  /// @brief Internal codec implementation.
  Codec m_codec = {};

  /// @brief Internal provider for archives.
  Provider m_provider = {};

  /// @brief Services container (if nullptr then running headless).
  XI::Container *m_services = nullptr;

public:
  //  CONSTRUCTORS  //

  /// @brief Constructs a headless archive.
  constexpr Archive() = default;

  /**
   * @brief Constructs a serviced archive.
   * @param services              Services container.
   */
  constexpr Archive(XI::Container *services) : m_services(services) {}

  //  PUBLIC METHODS  //

  /// @brief Denotes if currently headless.
  inline constexpr bool headless() const noexcept { return m_services == nullptr; }

  /// @brief Encapsulated provider instance.
  inline constexpr const Provider *provider() const noexcept { return &m_provider; }

  /**
   * @brief Handles decoding an archive.
   * @param blob                  Blob to decode.
   */
  inline constexpr Filesystem decode(const XJCT::Blob::View &blob) const noexcept { return m_codec.read(blob); }

  /**
   * @brief Handles encoding an archive.
   * @param fuse                  Archive fuse.
   */
  inline constexpr Filesystem decode(const XJCT::Sentinel::View &fuse) const noexcept {
    return decode(m_provider.resource(fuse));
  }

  /**
   * @brief Handles encoding an archive.
   * @param fs                    Filesystem to encode.
   */
  inline constexpr XJCT::Blob::Bytes encode(const Filesystem &fs) const noexcept { return m_codec.write(fs); }

  /**
   * @brief Handles encoding an archive.
   * @param modules               Modules to encode.
   */
  inline constexpr XJCT::Blob::Bytes encode(Import::Service *modules) const noexcept { return m_encode(modules); }

private:
  //  PRIVATE METHODS  //

  /**
   * @brief Handles compiling modules.
   * @param modules               Modules service.
   */
  XJCT::Blob::Bytes m_encode(Import::Service *modules) const noexcept;

  /**
   * @brief Handles compiling a module.
   * @param module                Module to compile.
   */
  Image::Arena *m_compile(Module::Abstract *module) const noexcept;

  /**
   * @brief Handles requesting a compilation.
   * @param module                Module to request.
   */
  std::future<Image::Arena *> m_request(Module::Abstract *module) const noexcept;
};

} // namespace Sabre::Bundle

#endif
