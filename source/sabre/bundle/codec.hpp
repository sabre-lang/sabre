#ifndef _SABRE_BUNDLE_CODEC_HPP
#define _SABRE_BUNDLE_CODEC_HPP

/// Sabre Includes
#include "sabre/forward/bundle.hpp"
#include "sabre/function/info.hpp"
#include "sabre/image/arena.hpp"

namespace Sabre::Bundle {

/// @brief Encapsulates a bundled filesystem.
struct Filesystem {
  //  PROPERTIES  //

  /// @brief The script resources.
  std::vector<$::URI::Buffer> resources = {};

  /// @brief All the available scripts.
  std::vector<$::Unique::Pointer<Image::Arena>> scripts = {};

  //  CONSTRUCTORS  //

  /**
   * @brief Constructs a filesystem.
   * @param arenas                Arenas to bind.
   */
  explicit Filesystem(const std::vector<Image::Arena *> &arenas = {});
};

/// @brief Handles encoding bundles.
struct Codec : public XJCT::Blob::Encoder<Filesystem>, public XJCT::Blob::Decoder<Filesystem> {
  //  CONSTRUCTORS  //

  /// @brief Constructs a defaulted codec.
  constexpr Codec() = default;

protected:
  //  PRIVATE METHODS  //

  /**
   * @brief Handles encoding an archive.
   * @param fs                    Filesystem to encode.
   * @param sink                  Blob sink to use.
   */
  void m_encode(const Filesystem &fs, XJCT::Blob::Bytes &sink) const noexcept final;

  /**
   * @brief Handles encoding an arena.
   * @param arena                 Arena to encode.
   * @param sink                  Blob sink to use.
   */
  void m_encode(const Image::Arena &arena, XJCT::Blob::Bytes &sink) const noexcept;

  /**
   * @brief Handles encoding a resource.
   * @param resource              Resource to encode.
   * @param sink                  Blob sink to use.
   */
  void m_encode(const $::URI::View &resource, XJCT::Blob::Bytes &sink) const noexcept;

  /**
   * @brief Handles encoding a function.
   * @param info                  Info to encode.
   * @param sink                  Blob sink to use.
   */
  void m_encode(const Function::Info &info, XJCT::Blob::Bytes &sink) const noexcept;

  /**
   * @brief Handles encoding a value.
   * @param value                 Value to encode.
   * @param sink                  Blob sink to use.
   */
  void m_encode(const Value::Any &value, XJCT::Blob::Bytes &sink) const noexcept;

  /**
   * @brief Handles reading the file-system.
   * @param source                Blob source state.
   */
  Filesystem m_decode(XJCT::Blob::View source) const noexcept final;

  /**
   * @brief Handles decoding an arena.
   * @param arena                 Arena to decode.
   * @param source                Blob source state.
   */
  void m_decode($::Unique::Pointer<Image::Arena> &arena, XJCT::Blob::View &source) const noexcept;

  /**
   * @brief Handles decoding a function.
   * @param info                  Function information.
   * @param source                Blob source state.
   */
  void m_decode($::Unique::Pointer<Function::Info> &info, XJCT::Blob::View &source) const noexcept;
};

} // namespace Sabre::Bundle

#endif
