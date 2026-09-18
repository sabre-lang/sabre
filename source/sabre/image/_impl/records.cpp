/// Sabre Includes
#include "sabre/image/records.hpp"

//  PRIVATE METHODS  //

Sabre::Image::Records::View Sabre::Image::Records::m_scan(size_t offset) const noexcept {
  // prepare the output of this instance
  View view = {.before = nullptr, .after = nullptr};
  if (m_partitions.empty()) return view; // stop if empty

  // get some valid iterators to be used
  size_t ii = 0, size = m_partitions.size();

  // attempt scanning for a suitable position to be at
  for (; ii < size && offset != m_partitions.at(ii).offset; ++ii);

  // update the after value now
  if (ii < size) view.before = &m_partitions.at(ii);
  if (++ii < size) view.after = &m_partitions.at(ii);

  // return the current result now
  return view;
}
