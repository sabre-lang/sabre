#ifndef _SABRE_AUTHOR_DOCIFY_HPP
#define _SABRE_AUTHOR_DOCIFY_HPP

/// Sabre Includes
#include "sabre/author/options.hpp"
#include "sabre/forward/relint.hpp"

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

  /// @brief Services container.
  XI::Container *m_services = nullptr;

public:
  //  CONSTRUCTORS  //

  /**
   * @brief Constructs a docification handler.
   * @param services                Services container.
   */
  explicit Docify();
  explicit Docify(XI::Container *services);

  //  PUBLIC METHODS  //

  /**
   * @brief Handles processing docification output.
   * @param resource                Resource to process.
   */
  int32_t process(const $::URI::View &resource, const Options &options = {});
};

} // namespace Sabre::Author

#endif
