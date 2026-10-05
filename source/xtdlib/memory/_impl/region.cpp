/// Vendor Includes
#include <boost/interprocess/anonymous_shared_memory.hpp>
#include <boost/interprocess/file_mapping.hpp>
#include <boost/interprocess/mapped_region.hpp>

/// Library Includes
#include "xtdlib/filesystem/read.hpp"
#include "xtdlib/memory/region.hpp"

/// Forward Definitions
$_FWD($::Memory, using Offset = boost::interprocess::offset_t)
$_FWD($::Memory, using Mapping = boost::interprocess::file_mapping)
$_FWD($::Memory, using Allocation = boost::interprocess::mapped_region)
$_FWD($::Memory, namespace IPC = boost::interprocess::ipcdetail)

/// Forward Declarations
$_FWD($::Memory, Allocation Anonymous(size_t))
$_FWD($::Memory, Allocation Read(const String::View &, Offset = 0))

//  TYPEDEFS  //

struct $::Memory::Region::Wrapper {
  //  PROPERTIES  //

  /// @brief The internal region instance.
  Allocation region = {};

  //  CONSTRUCTORS  //

  /// @brief Constructs a defaulted wrapper.
  explicit Wrapper() = default;

  /**
   * @brief Constructs a suitable internal region.
   * @param region            Region to contain.
   */
  explicit Wrapper(Allocation &&region) : region(std::move(region)) {}
};

//  CONSTRUCTORS  //

$::Memory::Region::Region() : m_internal(Shared::New<Wrapper>()) {}
$::Memory::Region::Region(size_t capacity) : m_internal(Shared::New<Wrapper>(Anonymous(capacity))) {}
$::Memory::Region::Region(Shared::Pointer<Wrapper> &&internal) : m_internal(std::move(internal)) {}
$::Memory::Region::Region(const String::View &content) : Region(content.data(), content.size()) {}
$::Memory::Region::Region(const std::span<const uint8_t> &buffer) : Region(buffer.data(), buffer.size()) {}
$::Memory::Region::Region(const void *data, size_t size) : Region(size) {
  std::memcpy(m_internal->region.get_address(), data, size);
}

//  PUBLIC METHODS  //

$::Memory::Region $::FS::Read(const String::Buffer &buffer) { return Read(String::View(buffer)); }
$::Memory::Region $::FS::Read(const FS::Path &file_path) { return Read(file_path.string()); }
$::Memory::Region $::FS::Read(const String::View &file_path) {
  auto region = Memory::Read(file_path); // resolve now for user
  return Memory::Region(Shared::New<Memory::Region::Wrapper>(std::move(region)));
}

bool $::Memory::Region::empty() const noexcept { return size() == 0; }
size_t $::Memory::Region::size() const noexcept { return m_internal->region.get_size(); }
void *$::Memory::Region::data() const noexcept { return m_internal->region.get_address(); }
$::String::View $::Memory::Region::view() const noexcept { return {static_cast<const char *>(data()), size()}; }
std::span<uint8_t> $::Memory::Region::span() const noexcept { return {static_cast<uint8_t *>(data()), size()}; }

//  PUBLIC METHODS  //

$::Memory::Allocation $::Memory::Anonymous(size_t size) {
  return size ? boost::interprocess::anonymous_shared_memory(size) : Allocation();
}

$::Memory::Allocation $::Memory::Read(const String::View &file_path, Offset size) {
  try {
    static constexpr auto readonly = boost::interprocess::read_only;
    auto mapping = Mapping(file_path.data(), readonly); // attempt
    IPC::get_file_size(mapping.get_mapping_handle().handle, size);
    return size ? Allocation(mapping, readonly) : Allocation();
  } catch (...) {
    $_ERROR("Memory::Read / Invalid Region Mapping - '{0}'", file_path);
    std::rethrow_exception(std::current_exception()); // re-throw error
  }
}
