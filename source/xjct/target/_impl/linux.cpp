/// XJCT Modules
#include "xjct/target/linux.hpp"
#include "xjct/forward/vendors.hpp"

/// Forward Declarations
$_FWD(XJCT::ELF, struct Note)

//  TYPEDEFS  //

/// @brief Allows constructing notes manually.
struct XJCT::ELF::Note : public LIEF::ELF::Note {
  template <class... As> constexpr Note(As &&...args) : LIEF::ELF::Note(std::forward<As>(args)...) {}
};

//  PRIVATE METHODS  //

bool XJCT::Target::Linux::m_imbue(Archive::Binary &binary, const Imbue::Options &options) const noexcept {
  // construct a stream to be used instead of the buffer
  auto stream = $::Unique::New<LIEF::SpanStream>(binary.data(), binary.size());

  auto parser = LIEF::ELF::Parser::parse(std::move(stream));
  if (parser == nullptr) return false; // failed building

  // fail if any of the notes have the same name
  auto predicate = [options](const auto &note) { return note.name() == options.name; };
  if (std::any_of(parser->notes().begin(), parser->notes().end(), predicate)) return false;

  // prepare some constants to be used
  static constexpr auto s_section = ".note.custom";
  static constexpr auto s_type = LIEF::ELF::Note::TYPE::UNKNOWN;

  // prepare the archive content to be used
  Blob::Bytes content = {options.blob.begin(), options.blob.end()};

  // prepare the note (need to have a temporary value since its a unique_ptr)
  auto note = $::Unique::New<ELF::Note>(options.name, s_type, 0, content, s_section);

  // bind the note we require to our parser
  parser->add(*note);

  // manually build our instance (as we require note building)
  LIEF::ELF::Builder builder = {*parser, {.notes = true}};

  // finally rebuild the output as necessary now (this currently fails due to LIEF being bad)
  return builder.build(), binary.update(builder.get_build());
}
