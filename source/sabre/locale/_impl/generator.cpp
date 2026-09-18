/// Vendor Includes
#include <boost/locale.hpp>

/// Sabre Includes
#include "sabre/locale/generator.hpp"

//  CONSTRUCTORS  //

Sabre::Locale::Generator::Generator() : m_context(new boost::locale::generator()) {}
Sabre::Locale::Generator::~Generator() { delete static_cast<boost::locale::generator *>(m_context); }

//  PRIVATE METHODS  //

std::locale Sabre::Locale::Generator::m_bind(const std::locale &locale, const $::String::Buffer &id) const noexcept {
  return static_cast<boost::locale::generator *>(m_context)->generate(locale, id);
}
