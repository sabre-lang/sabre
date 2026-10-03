/// Vendor Includes
#include <boost/interprocess/anonymous_shared_memory.hpp>
#include <boost/interprocess/file_mapping.hpp>
#include <boost/interprocess/mapped_region.hpp>

/// Library Includes
#include "xtdlib/filesystem/read.hpp"

/// Forward Definitions
$_FWD($::FS::Buffer, using Offset = boost::interprocess::offset_t)
$_FWD($::FS::Buffer, using Mapping = boost::interprocess::file_mapping)
$_FWD($::FS::Buffer, using Allocation = boost::interprocess::mapped_region)

/// Forward Declarations
$_FWD($::FS::Buffer, Offset Size(Mapping &, Offset = 0))
$_FWD($::FS::Buffer, Allocation Resolve(const String::View &))

//  TYPEDEFS  //

struct $::FS::Region::Wrapper {
  //  PROPERTIES  //

  /// @brief The internal region instance.
  Buffer::Allocation region = {};

  //  CONSTRUCTORS  //

  /// @brief Constructs a defaulted wrapper.
  explicit Wrapper() = default;

  /**
   * @brief Constructs a suitable internal region.
   * @param region            Region to contain.
   */
  explicit Wrapper(Buffer::Allocation &&region) : region(std::move(region)) {}
};

//  CONSTRUCTORS  //

$::FS::Region::Region() : m_internal($::Shared::New<Wrapper>()) {}
$::FS::Region::Region($::Shared::Pointer<Wrapper> &&internal) : m_internal(std::move(internal)) {}
$::FS::Region::Region(const String::View &content) {
  auto memory = boost::interprocess::anonymous_shared_memory(content.size());
  m_internal = $::Shared::New<Wrapper>(Buffer::Allocation(std::move(memory)));
  std::memcpy(m_internal->region.get_address(), content.data(), content.size());
}

//  PUBLIC METHODS  //

$::FS::Region $::FS::Read(const String::Buffer &buffer) { return Read(String::View(buffer)); }
$::FS::Region $::FS::Read(const FS::Path &file_path) { return Read(file_path.string()); }
$::FS::Region $::FS::Read(const String::View &file_path) {
  auto region = Buffer::Resolve(file_path); // resolve now for user
  return Region($::Shared::New<Region::Wrapper>(std::move(region)));
}

bool $::FS::Region::empty() const noexcept { return size() == 0; }
size_t $::FS::Region::size() const noexcept { return m_internal->region.get_size(); }
void *$::FS::Region::data() const noexcept { return m_internal->region.get_address(); }
$::String::View $::FS::Region::view() const noexcept { return {static_cast<const char *>(data()), size()}; }
std::span<uint8_t> $::FS::Region::span() const noexcept { return {static_cast<uint8_t *>(data()), size()}; }

//  PUBLIC METHODS  //

$::FS::Buffer::Allocation $::FS::Buffer::Resolve(const String::View &file_path) {
  try {
    static constexpr auto readonly = boost::interprocess::read_only;
    auto mapping = Mapping(file_path.data(), readonly); // attempt
    return Size(mapping) ? Allocation(mapping, readonly) : Allocation();
  } catch (...) {
    $_ERROR("FS::Buffer / Invalid file-mapping - {0}", file_path);
    std::rethrow_exception(std::current_exception()); // re-throw
  }
}

$::FS::Buffer::Offset $::FS::Buffer::Size(Mapping &mapping, Offset size) {
  return boost::interprocess::ipcdetail::get_file_size(mapping.get_mapping_handle().handle, size), size;
}
