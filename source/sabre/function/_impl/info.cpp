/// Sabre Includes
#include "sabre/function/info.hpp"

//  PRIVATE METHODS  //

void Sabre::Function::Info::m_print(std::ostream &os, const Info &self) {
  // show the leading details to be used now
  os << "<Function Info (detail: " << (self.main() ? "'main'" : fmt::to_string(fmt::ptr(&self))) << ")>";

  // prepare the indentation to be used now
  $_UNUSED $_AUTO = $::Manip::Indent(os);

  // prepare some details about the instructions
  auto size = self.m_bytecode.size();
  auto address = self.m_bytecode.address();
  auto base = self.m_arena->binary.address();
  auto records = self.m_arena->binary.records();

  // show an empty result if necessary to do so
  if (size == 0) os << "\n(none)";

  // get the formatting to use for the sources now
  auto padding = records.width();

  // attempt decoding all the available instructions now
  for (auto offset = address; offset < address + size; offset += Bytecode::Width) {
    auto *entry = records.before(offset - base); // resolve where
    auto position = entry ? fmt::to_string(entry->position) : "";
    auto *decoded = std::bit_cast<Bytecode::Instruction *>(offset);
    auto start = reinterpret_cast<const uint8_t *>(decoded);
    auto slice = std::vector<uint8_t>(start, start + Bytecode::Width);

    // and print the necessary details now
    os << fmt::format("\n| {0:08X}", offset - address);
    os << fmt::format(" / {0:<{1}}", position, padding);
    os << fmt::format(" |  {0:02X}", fmt::join(slice, " "));
    os << fmt::format("    {0}", *decoded);
  }
}
