#ifndef _SABRE_AUTHOR_DOCIFY_HPP
#define _SABRE_AUTHOR_DOCIFY_HPP

/// Sabre Includes
#include "sabre/author/options.hpp"
#include "sabre/author/project.hpp"
#include "sabre/forward/import.hpp"

namespace Sabre::Author {

/**
 * @brief Handles resolving output files.
 * @param path                  Given output path.
 * @param hint                  Hint to fallback.
 * @param extension             Optional extension.
 */
static inline constexpr $::FS::Path
Outfile(const $::String::View &path, const $::URI::View &original, const $::String::View &extension = {}) {
  auto output = path.size() ? $::Path::canonical(path) : $::FS::Path(original.body()).replace_extension();
  return extension.size() && output.extension() != extension ? output += extension : output;
}

/// @brief Handles Docification of Modules.
class Docify : public XI::Transient {
  //  PROPERTIES  //

  /// @brief Docification options.
  Options m_options = {};

  /// @brief Services container.
  XI::Container *m_services = nullptr;

  /// @brief Modules storage.
  Import::Storage *m_modules = nullptr;

  /// @brief The current project instance.
  $::Unique::Pointer<Project> m_project = nullptr;

public:
  //  CONSTRUCTORS  //

  /**
   * @brief Constructs a docification handler.
   * @param services                Services container.
   * @param options                 Docification options.
   */
  explicit Docify(const Options &options = {});
  explicit Docify(XI::Container *services, const Options &options = {});

  //  PUBLIC METHODS  //

  /// @brief Gets the underlying options.
  inline constexpr const Options &options() const noexcept { return m_options; }

  /// @brief Gets the project instance.
  inline constexpr const Project *project() const noexcept { return m_project.get(); }

  /**
   * @brief Handles processing docification output.
   * @param resource                Input resource.
   */
  inline constexpr int32_t process(const $::URI::View &resource) {
    return m_initialize(resource), m_finalize(resource), EXIT_SUCCESS;
  }

private:
  //  PRIVATE METHODS  //

  /**
   * @brief Handles initializing the output to be emitted.
   * @param resource                Input resource.
   */
  void m_initialize(const $::URI::View &resource);

  /**
   * @brief Handles finalizing the output to be emitted.
   * @param resource                Input resource.
   */
  int32_t m_finalize(const $::URI::View &resource);

  /**
   * @brief Handles processing incoming modules.
   * @param resource                Resource to process.
   */
  void m_process(const $::URI::View &resource);
  void m_process(const Module::Abstract *module);

  /**
   * @brief Handles processing incoming references.
   * @param mirror                  Mirror reference.
   */
  void m_reference(const Relint::Mirror *mirror);
};

} // namespace Sabre::Author

#endif
