/// Format Includes
#include "sabre/format/_inline/macros.ipp"

//  PUBLIC METHODS  //

SABRE_MM_FORMAT_INFIX(Cast, reader, value) {
  return m_binary(reader, value, [](Reader *reader) { return m_annotation(reader); });
}
