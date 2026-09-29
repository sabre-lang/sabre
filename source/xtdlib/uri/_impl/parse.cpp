/// Library Includes
#include "xtdlib/uri/parse.hpp"

//  PUBLIC METHODS  //

$::URI::Buffer $::URI::Parse(const String::View &view) {
  // prepare some details about the details
  auto offset = view.find(':');
  auto buffer = Buffer::m_normalize(view);

  // attempt checking for file paths now
  if (offset == String::Term || ($_PLATFORM_WINDOWS && offset == 1)) {
    if (FS::Path(view).is_absolute()) return Buffer(Path::canonical(buffer).string());
    return Buffer(Scheme::HREF, buffer.starts_with("./") ? buffer.substr(2) : buffer);
  }

  // pull out the current scheme value now
  auto scheme = buffer.substr(0, offset);
  auto remaining = buffer.substr(offset + 1);

  // ignore parsing an authority if there is none
  if (!remaining.starts_with("//")) return Buffer(scheme, remaining);

  // attempt finding the necessary authority now
  remaining = remaining.substr(2);
  offset = remaining.find('/');

  auto authority = remaining.substr(0, offset); // resolve our values now
  auto body = offset == String::Term ? "" : remaining.substr(offset);

  // and reconstruct the resulting URI again
  return Buffer(scheme, authority, body);
}
