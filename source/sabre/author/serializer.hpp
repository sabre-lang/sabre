#ifndef _SABRE_AUTHOR_SERIALIZER_HPP
#define _SABRE_AUTHOR_SERIALIZER_HPP

/// Sabre Includes
#include "sabre/author/docify.hpp"

namespace Sabre::Author {

/// @brief Serialization Bypass.
class Serializer : public XI::Transient {
  //  TYPEDEFS  //

  /// @brief Serialization Abstraction.
  class $_ABSTRACT Abstract {
    //  PROPERTIES  //

    /// @brief Authoring options.
    const Options *m_options = nullptr;

    /// @brief Project instance to serialize.
    const Project *m_project = nullptr;

  public:
    //  CONSTRUCTORS  //

    /// @brief Do not allow default serialization.
    constexpr Abstract() = delete;

    /**
     * @brief Constructs a serializer instance.
     * @param internal          Docification instance.
     */
    constexpr Abstract(const Docify *self) : m_options(&self->options()), m_project(self->project()) {}

    /// @brief Virtual abstract destructor.
    virtual ~Abstract() = default;

    //  PUBLIC METHODS  //

    /// @brief Gets the project options.
    inline constexpr const Options *options() const noexcept { return m_options; }

    /// @brief Gets the project instance.
    inline constexpr const Project *project() const noexcept { return m_project; }

    /// @brief Coordinates rendering the result.
    inline constexpr $::String::View label() const noexcept { return m_label(); }

    /// @brief Gets the output file to write to.
    inline constexpr $::String::View extension() const noexcept { return m_extension(); }

    /// @brief Coordinates rendering the result.
    inline constexpr $::String::Buffer render() noexcept { return m_render(); }

  protected:
    //  PRIVATE METHODS  //

    /// @brief Handles rendering the final output.
    virtual $::String::Buffer m_render() noexcept = 0;

    /// @brief Handles resolving a suitable proxy label.
    virtual $::String::View m_label() const noexcept = 0;

    /// @brief Handles resolving a suitable output file.
    virtual $::String::View m_extension() const noexcept = 0;
  };

  /// @brief Proxy for Serialization.
  template <Format F> class Proxy;

  //  PROPERTIES  //

  /// @brief Services container.
  XI::Container *m_services = nullptr;

  /// @brief Verbose spinner details.
  $::Spinner::Pointer m_spinner = nullptr;

  /// @brief Internal serializer instance.
  $::Unique::Pointer<Abstract> m_proxy = nullptr;

public:
  //  CONSTRUCTORS  //

  /// @brief Do not allow default serialization.
  constexpr Serializer() = delete;

  /**
   * @brief Constructs a serializer instance.
   * @param self            Docification instance.
   */
  Serializer(const Docify *self);
  Serializer(XI::Container *services, const Docify *self);

  //  PUBLIC METHODS  //

  /// @brief Handles writing the output as necessary.
  void write();

private:
  //  PRIVATE METHODS  //

  /// @brief Resolves a suitable output file.
  $::String::Buffer m_outfile() const noexcept;
};

} // namespace Sabre::Author

#endif
