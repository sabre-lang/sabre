/// Shell Includes
#include "shell/command/program.hpp"

//  PRIVATE METHODS  //

void Shell::Command::Descriptor::m_help(std::ostream &os) const {
  // print our base usage details now
  os << $::Dye::bold("Usage") << ": " << $::Dye::red(SABRE_MM_IDENTIFIER).bold();
  if (m_title.size()) os << $::Dye::blue(" {0}", m_title).bold(); // show item

  // and append the usage details based on size of commands and options
  if (m_commands.size()) os << $::Dye::bold(" <command>");
  if (m_options.size()) os << $::Dye::bold(" [options...]");

  // show any potential positionals now as well
  for (const auto &entry : m_positionals) m_help(os, entry);

  // always post-emplace a new-line now
  os << '\n';

  // and print the descriptor details now
  m_help(os, "Commands", m_commands);
  m_help(os, "Options", m_options);
  m_help(os, "Environment", m_environment);
}

void Shell::Command::Descriptor::m_help(std::ostream &os, const Positional &entry) const {
  // destructure the incoming positional now
  auto [positional, required] = entry;

  auto open = required ? '<' : '[';  // prepare the open ...
  auto close = required ? '>' : ']'; // ... and the close tags

  // and print the desired positional now
  os << $::Dye::dim(" {0}{1}{2}", open, positional, close).bold();
}

void Shell::Command::Descriptor::m_help(std::ostream &os, const $::String::View &title, const List &list) const {
  // ignore if already empty
  if (list.empty()) return;

  // allow setting all our details now
  os << '\n' << $::Dye::bold(title) << ":\n";

  // get the underlying padding to be used
  auto padding = m_padding(list);

  // indent our details as necessary now
  $_UNUSED $_AUTO = $::Manip::Indent(os);

  // print all the available items now
  for (const auto &[name, description] : list) {
    auto size = padding - name.value().size(); // prepare padding
    auto styled = name.empty() ? $::Dye::cyan(name.value()) : name;
    os << styled << $::String::Buffer(size, ' ') << description;
    os << (description.size() ? "." : "") << '\n'; // show closing
  }
}
