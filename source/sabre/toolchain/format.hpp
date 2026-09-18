#ifndef _SABRE_TOOLCHAIN_FORMAT_HPP
#define _SABRE_TOOLCHAIN_FORMAT_HPP

/// Sabre Includes
#include "sabre/document/buffer.hpp"
#include "sabre/format/options.hpp"

namespace Sabre::Toolchain {

/**
 * @brief Handles formatting source-code.
 * @param buffer                Buffer to format.
 * @param options               Format options.
 */
Format::Result format(const $::String::View &buffer, const Format::Options &options = {});
Format::Result format(const Document::Buffer &document, const Format::Options &options = {});

} // namespace Sabre::Toolchain

#endif
