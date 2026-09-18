/// Sabre Include
#include "sabre/format/pipeline.hpp"
#include "sabre/format/dispatch.hpp"

//  CONSTRUCTORS  //

Sabre::Format::Pipeline::Pipeline() : Pipeline($::Global::get<Format::Options>()) {}
Sabre::Format::Pipeline::Pipeline(const Options *options) : m_options(options) {}

//  PUBLIC METHODS  //

const Sabre::Format::Options *Sabre::Format::Pipeline::options() const noexcept { return m_options; }
Sabre::Format::Result Sabre::Format::Pipeline::process(const Lexer::Buffer *tokens) {
  // construct the reader and writer to be used
  auto writer = $::Unique::New<Writer>(m_options);
  auto reader = $::Unique::New<Reader>(tokens, m_options);

  // attempt reading the program as a collection of nodes
  auto *program = Dispatch::program(reader.get());
  if (program == nullptr) return std::nullopt;

  // finally attempt writing our output program now
  return program->render(writer.get()), writer->merge();
}
