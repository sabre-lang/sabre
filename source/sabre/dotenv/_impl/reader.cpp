/// Sabre Includes
#include "sabre/dotenv/reader.hpp"

//  PUBLIC METHODS  //

Sabre::Dotenv::View &&Sabre::Dotenv::Reader::parse(const $::String::View &input, bool inherit) {
  return parse(input, inherit ? $::Environ::view() : View());
}

Sabre::Dotenv::View &&Sabre::Dotenv::Reader::parse(const $::String::View &input, View &&env) {
  // prepare the lines to be iterated over
  auto lines = std::views::split(input, '\n');

  // and attempt parsing each line individually as needed
  for (auto &&line : lines) m_parse(env, $::Trim::both($::String::View(line)));

  // return the resulting environment map
  return env;
}

//  PRIVATE METHODS  //

void Sabre::Dotenv::Reader::m_parse(View &env, const $::String::View &line) {
  // check for empty lines, or ones that will have no content
  if (line.empty() || line.starts_with('#')) return;

  // check for an equals sign (without one we ignore the line)
  auto equals = line.find_first_of('=');
  if (equals == $::String::Term) return;

  // also ensure that the hash comes after the equals sign
  if (line.find_first_of('#') < equals) return;

  // split the instance into a key and value pair
  auto key = $::Trim::trailing(line.substr(0, equals));
  auto value = $::Trim::leading(line.substr(equals + 1));

  // if the key is empty, then we ignore assigning
  if (key.empty()) return;

  // we now need to actually decipher the content now
  m_emplace(env, key, m_decipher(env, value));
}

$::String::Buffer Sabre::Dotenv::Reader::m_decipher(const View &, $::String::View &value) {
  // if the value is empty, then fast-path out
  if (value.empty()) return {};

  // get the current quotation details
  auto quotation = value.front();
  auto interpolate = quotation == '"';

  // if we have no valid quotation value
  if (!interpolate && quotation != '`' && quotation != '\'') {
    auto hash = value.find_first_of('#');
    value = value.substr(0, hash);
    value = $::Trim::trailing(value);
    return $::String::Buffer(value);
  }

  // remove the leading quotation mark now
  value = value.substr(1);

  // find the next quotation value now
  auto closing = value.find_first_of(quotation);

  // only handle if we do have a closing value
  if (closing != $::String::Term) value = value.substr(0, closing);

  /// TODO: use the "interpolate" result for formatting values with "env"

  // should safely be able to unescape the value now
  return $::Serde::Unescape(value).value_or("");
}

void Sabre::Dotenv::Reader::m_emplace(View &env, const $::String::View &key, const $::String::Buffer &value) {
  env.insert_or_assign(key, value);
}
