/// Sabre Includes
#include "sabre/bundle/codec.hpp"
#include "sabre/resource/scheme.hpp"

//  PRIVATE METHODS  //

void Sabre::Bundle::Codec::m_encode(const Filesystem &fs, XJCT::Blob::Bytes &sink) const noexcept {
  // write the size of the incoming scripts vector
  m_write<size_t>(fs.scripts.size(), sink);

  // and write each of the script values now
  for (const auto &script : fs.scripts) m_encode(*script, sink);
}

void Sabre::Bundle::Codec::m_encode(const Image::Arena &arena, XJCT::Blob::Bytes &sink) const noexcept {
  // encode the leading resource now
  m_encode(arena.resource, sink);

  // write the bytecode out to be encapsulated now
  m_write(arena.binary.buffer(), sink);

  m_write(arena.constants.size(), sink); // write out the constants now
  for (const auto &constant : arena.constants) m_encode(constant, sink);

  m_write(arena.strings.size(), sink); // write the strings now
  for (const auto &intern : arena.strings) m_write(intern.view(), sink), m_encode(intern.symbol(), sink);

  m_write(arena.functions.size(), sink); // finally write the function information
  for (const auto &function : arena.functions) m_encode(*function, sink);
}

void Sabre::Bundle::Codec::m_encode(const $::URI::View &resource, XJCT::Blob::Bytes &sink) const noexcept {
  m_write($::URI::Buffer(Resource::Scheme::ARCH, resource.relative().string()).view(), sink);
}

void Sabre::Bundle::Codec::m_encode(const Function::Info &info, XJCT::Blob::Bytes &sink) const noexcept {
  // write the current buffer details
  auto buffer = info.bytecode();
  m_write(buffer.size(), sink);
  m_write(buffer.offset(), sink);

  // write the shared information now
  auto *shared = info.shared();
  m_write(shared->arity, sink);
  m_write(shared->adicity, sink);
  m_write(shared->locals, sink);
  m_write(shared->leaked, sink);
  m_write(shared->vargs, sink);
}

void Sabre::Bundle::Codec::m_encode(const Value::Any &value, XJCT::Blob::Bytes &sink) const noexcept {
  m_write(Pointer::Underlying(value.pointer()), sink);
}

Sabre::Bundle::Filesystem Sabre::Bundle::Codec::m_decode(XJCT::Blob::View source) const noexcept {
  // construct the baseline filesystem instance now
  auto filesystem = Filesystem();

  // prepare the size of the incoming scripts now
  auto scripts = m_read<size_t>(source);

  // pre-fill the resources and scripts size
  filesystem.scripts.reserve(scripts);
  filesystem.resources.reserve(scripts);

  // and attempt decoding as necessary into the arenas
  for (size_t ii = 0; ii < scripts; ++ii) {
    auto buffer = $::URI::Parse(m_view(source)); // parse the resource
    $::URI::View resource = filesystem.resources.emplace_back(buffer);
    auto &arena = filesystem.scripts.emplace_back(nullptr);
    m_decode(arena, source), arena->resource = resource;
  }

  // return the resulting filesystem now
  return filesystem;
}

void Sabre::Bundle::Codec::m_decode($::Unique::Pointer<Image::Arena> &arena, XJCT::Blob::View &source) const noexcept {
  // ensure we reconstruct the arena now
  arena = $::Unique::New<Image::Arena>();

  // update the bytecode block to be used
  arena->binary = {m_span<uint8_t>(source)};

  auto constants = m_span<Pointer::Underlying>(source); // read the constants
  for (const auto &pointer : constants) arena->constants.emplace_back(pointer);

  // prepare the strings base to be read from
  auto strings = m_read<size_t>(source);
  arena->strings.reserve(strings);

  // prepare the strings to be read now
  for (size_t ii = 0; ii < strings; ++ii) {
    auto buffer = $::String::Buffer(m_view(source));
    auto value = Value::Any(m_read<Pointer::Underlying>(source));
    arena->strings.emplace_back(buffer, value.as<Value::Symbol>());
  }

  // prepare the functions size to be read from
  auto functions = m_read<size_t>(source);
  arena->functions.reserve(functions);

  // finally attempt writing all out all the functions now
  for (size_t ii = 0; ii < functions; ++ii) {
    auto info = $::Unique::New<Function::Info>(arena.get()); // rebuild
    m_decode(arena->functions.emplace_back(std::move(info)), source);
  }
}

void Sabre::Bundle::Codec::m_decode($::Unique::Pointer<Function::Info> &info, XJCT::Blob::View &source) const noexcept {
  // get the underlying buffer details
  auto size = m_read<size_t>(source), offset = m_read<size_t>(source);
  info->bytecode() = info->arena()->binary.slice(offset, size);

  // read the shared information now
  auto *shared = info->shared();
  m_read(&shared->arity, 1, source);
  m_read(&shared->adicity, 1, source);
  m_read(&shared->locals, 1, source);
  m_read(&shared->leaked, 1, source);
  m_read(&shared->vargs, 1, source);
}
