/// Sabre Includes
#include "sabre/format/collection.hpp"

//  PUBLIC METHODS  //

bool Sabre::Format::Collection::append(Piece *item, Piece *trailing) noexcept {
  // check if the item to append failed to do so
  if (item == nullptr) return false;

  // emplace the trailing item at this location
  if (trailing) m_trailing.emplace(m_buffer.size(), trailing);

  // otherwise append this item now as needed
  return m_buffer.emplace_back(item), true;
}

void Sabre::Format::Collection::whitespace(Piece *piece) noexcept {
  m_leading.try_emplace(m_buffer.size(), Buffer()).first->second.emplace_back(piece);
}

Sabre::Format::Piece::List *Sabre::Format::Collection::merge(Storage *storage, bool trailing) const noexcept {
  // prepare the baseline output to be used now
  auto output = storage->list();

  // get the total size of the output pieces
  auto size = m_buffer.size();

  // prepare a copy of the leading whitespace
  auto whitespace = m_trim(m_leading);

  // modify the leading and trailing whitespace values
  if (auto iter = whitespace.find(0); iter != whitespace.cend()) {}

  // check if there are any comments available
  auto comments = whitespace.size() || m_trailing.size();

  // append an initial piece to force wrapping for comments
  if (comments) storage->append(output, storage->expand());

  // prepare a spacer between pieces as well to be used
  auto *spacer = (comments || m_separator == nullptr) ? storage->line().hard : storage->line().soft;

  // iterate over the available pieces now
  for (const auto &[ii, piece] : $::Ranges::Each(m_buffer)) {
    // ensure we output the current separator and trailing comment
    if (ii > 0) storage->append(output, m_flush(storage, ii - 1, trailing), spacer);

    // get the leading whitespace available now to be emplaced onto the buffer
    auto leading = whitespace.contains(ii) ? whitespace.at(ii) : Buffer();
    for (auto *comment : leading) storage->append(output, comment, storage->line().hard);

    // append the piece instance now
    storage->append(output, piece);
  }

  // flush any trailing comments for the last piece (if there was one)
  if (m_buffer.size()) storage->append(output, m_flush(storage, size - 1, trailing));

  // and emplace the final leading whitespace that is available
  for (const auto &comment : whitespace.contains(size) ? whitespace.at(size) : Buffer()) {
    if (output->pieces().size() > 1) storage->append(output, storage->line().hard);
    storage->append(output, comment); // and append the comment here now after
  }

  // should be able to return the final result now
  return storage->group(output);
}

//  PRIVATE METHODS  //

Sabre::Format::Collection::Whitespace Sabre::Format::Collection::m_trim(Whitespace whitespace) const noexcept {
  auto iter = whitespace.find(0); // get the leading buffer value
  auto *buffer = iter != whitespace.cend() ? &iter->second : nullptr;

  // attempt trimming the leading whitespace buffer now
  while (buffer && buffer->size() && buffer->front()->is<Piece::Empty>()) buffer->erase(buffer->cbegin());

  // update the position if the final item is empty at all
  if (buffer && buffer->empty()) whitespace.erase(iter);

  iter = whitespace.find(m_buffer.size()); // get the last item
  buffer = iter != whitespace.cend() ? &iter->second : nullptr;

  // attempt trimming the trailing whitespace buffer now
  while (buffer && buffer->size() && buffer->back()->is<Piece::Empty>()) buffer->pop_back();

  // erase if necessary to do so
  if (buffer && buffer->empty()) whitespace.erase(iter);

  // return the resulting whitespace now
  return whitespace;
}

Sabre::Format::Piece *Sabre::Format::Collection::m_flush(Storage *storage, size_t index, bool trailing) const noexcept {
  // ignore appending trailing details if necessary
  if (!m_trailing.contains(index)) return m_suffix(storage, index, trailing);

  // otherwise append a trailing piece to be used
  return storage->list(m_suffix(storage, index, trailing), storage->space().hard, m_trailing.at(index));
}

Sabre::Format::Piece *
Sabre::Format::Collection::m_suffix(Storage *storage, size_t index, bool trailing) const noexcept {
  // check if the separator should be ignored
  if (m_separator == nullptr) return storage->empty();

  // check if using the last element
  if (index == m_buffer.size() - 1) return trailing ? storage->decide(m_separator) : storage->empty();

  // otherwise should be a normal delimited value now
  return storage->list(m_separator, storage->space().soft);
}
