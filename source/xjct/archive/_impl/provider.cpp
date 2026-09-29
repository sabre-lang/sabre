/// XJCT Modules
#include "xjct/archive/provider.hpp"
#include "xjct/forward/vendors.hpp"
#include "xjct/target/darwin.hpp"
#include "xjct/target/linux.hpp"
#include "xjct/target/windows.hpp"

//  PROPERTIES  //

static auto g_target_linux = $::Unique::New<XJCT::Target::Linux>();
static auto g_target_darwin = $::Unique::New<XJCT::Target::Darwin>();
static auto g_target_windows = $::Unique::New<XJCT::Target::Windows>();

//  PRIVATE METHODS  //

XJCT::Target::Abstract *XJCT::Archive::Provider::m_resolve() const noexcept {
#if $_PLATFORM_LINUX
  return g_target_linux.get();
#elif $_PLATFORM_DARWIN
  return g_target_darwin.get();
#elif $_PLATFORM_WINDOWS
  return g_target_windows.get();
#else
  return nullptr;
#endif
}

XJCT::Target::Abstract *XJCT::Archive::Provider::m_resolve(const Binary &binary) const noexcept {
  switch (binary.format()) {
  case Format::LINUX: return g_target_linux.get();
  case Format::DARWIN: return g_target_darwin.get();
  case Format::WINDOWS: return g_target_windows.get();
  default: return nullptr; // ignore bad options now
  }
}

bool XJCT::Archive::Provider::m_toggle(Binary &binary, const $::String::View &prefix, bool state) const noexcept {
  // prepare the binaries blob representation
  auto blob = binary.blob();
  auto span = binary.span();

  // resolve the first/last instances to be used now
  auto first = blob.find(prefix), last = blob.find(prefix, first + 1);

  // ensure that the details are correct now
  if (first == $::String::Term) return false;
  else if (last != $::String::Term) return false;

  // prepare the colon and resource details now
  auto colon = first + prefix.size(), resource = colon + 1;

  // should be able to imbue as necessary now
  if (span[colon] != ':') return false;

  // update the fuse resource now
  return (span[resource] = state + '0'), true;
}
