/// XJCT Modules
#include "xjct/archive/binary.hpp"
#include "xjct/forward/vendors.hpp"

//  PRIVATE METHODS  //

XJCT::Archive::Format XJCT::Archive::Binary::m_deduce(const std::span<const uint8_t> &span) noexcept {
  auto stream = $::Unique::New<LIEF::SpanStream>(span);
  if (LIEF::ELF::is_elf(*stream.get())) return Format::LINUX;
  else if (LIEF::MachO::is_macho(*stream.get())) return Format::DARWIN;
  return LIEF::PE::is_pe(*stream.get()) ? Format::WINDOWS : Format::UNKNOWN;
}
