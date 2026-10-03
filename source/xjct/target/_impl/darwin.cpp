/// XJCT Modules
#include "xjct/target/darwin.hpp"
#include "xjct/forward/vendors.hpp"

//  PRIVATE METHODS  //

bool XJCT::Target::Darwin::m_imbue(Archive::Binary &binary, const Imbue::Options &options) const noexcept {
  // construct a stream to be used instead of the buffer
  auto stream = $::Unique::New<LIEF::SpanStream>(binary.data(), binary.size());

  // prepare the parser to be used for modification
  auto parser = LIEF::MachO::Parser::parse(std::move(stream));
  if (parser == nullptr) return false; // failed to create

  // pre-build the necessary content now
  Blob::Bytes content = {options.blob.begin(), options.blob.end()};

  // iterate over the available Mach-O binaries
  for (auto &fat : *parser) {
    // attempt adding our section (ensuring it doesn't exist)
    if (fat.has_section(options.name)) return false;

    // prepare the section to be added now (LIEF sillily creates unique_ptr)
    auto section = LIEF::MachO::Section::create(options.name, content);

    // add the content as a section now (but then expects a const reference?)
    fat.add_section(*section);

    // remove code-signature as well if necessary (required to rebuild)
    if (fat.has_code_signature()) fat.remove_signature();
  }

  // update our executable and return now
  return binary.update(parser->raw());
}
