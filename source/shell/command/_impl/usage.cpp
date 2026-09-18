/// Shell Includes
#include "shell/command/program.hpp"

//  X-MACROS  //

#define XX_COMMANDS_LIST(X)                                                        \
  X("run", "Execute a script/project with Sabre", .magenta().bold())               \
  X("task", "Runs a pre-defined crate configuration task", .magenta().bold())      \
  X("test", "Runs all available unit tests with Sabre", .magenta().bold())         \
  X("", "")                                                                        \
  X("bundle", "Bundles scripts/projects into executables", .cyan().bold())         \
  X("format", "Formats scripts/projects with the builtin styler", .cyan().bold())  \
  X("lint", "Lints scripts/projects for enhanced static analysis", .cyan().bold()) \
  X("serve", "Starts a Sabre language-server instance", .cyan().bold())            \
  X("", "")                                                                        \
  X("upgrade", "Upgrades to the latest version of Sabre", .blue().bold())          \
  X("<command> --help", "Print help text for a given command", .dim())

#define XX_OPTIONS_LIST(X)                                   \
  X("-h, --help", "Display global help information")         \
  X("-v, --version", "Show the current executable version")  \
  X("    --vendors", "Show all the vendors that Sabre uses") \
  X("    --revision", "Show the version with revision hash")

#define XX_ENVIRONMENT_LIST(X)                                                                  \
  X("SABRE_LOGGING_LEVEL", "Sets the debug logging level", .cyan().italic())                    \
  X("SABRE_HARDWARE_VPROCS", "Maximum virtual processors count", .cyan().italic())              \
  X("SABRE_STACK_SIZE", "The size used by thread stacks", .cyan().italic())                     \
  X("SABRE_STACK_LIMIT", "The overflow limit for thread stacks", .cyan().italic())              \
  X("", "")                                                                                     \
  X("NO_COLOR", "Disables the use of terminal colors", .cyan().italic())                        \
  X("NO_PROGRESS", "Disables the use of terminal spinners and progress bars", .cyan().italic())

//  PROPERTIES  //

#define X(N, D, ...) {$::Color::ANSI(N) __VA_ARGS__, D},
static auto g_descriptor = Shell::Command::Descriptor()
                               .commands({XX_COMMANDS_LIST(X)})
                               .options({XX_OPTIONS_LIST(X)})
                               .environment({XX_ENVIRONMENT_LIST(X)});
#undef X

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
