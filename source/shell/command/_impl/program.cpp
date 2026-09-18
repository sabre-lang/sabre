/// Sabre Includes
#include <sabre/product/details.hpp>

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

//  CONSTRUCTORS  //

Shell::Command::Program::Program() : m_app($::Unique::New<CLI::App>()) {
  // set the application now
  m_app->name(SABRE_MM_IDENTIFIER);

  // ensure we receive a subcommand
  m_app->require_subcommand(0, 1);

  // set a desired formatter now
  m_app->formatter($::Shared::New<Formatter>(this));

  // set the underlying version handler now as well
  m_app->set_version_flag("-v,--version", "v" SABRE_MM_VERSION);
  m_app->add_flag_callback("--vendors", m_vendors)->configurable(false);
  m_app->add_flag_callback("--revision", m_revision)->configurable(false);

  // process global environment variables early
  if (auto level = $::Environ::get("SABRE_LOGGING_LEVEL")) $::Logger::Options::update(*level);
}

//  PUBLIC METHODS  //

int32_t Shell::Command::Program::launch(int32_t argc, char **argv) {
  // fast-path our "help" screen when no arguments are given
  if (argc < 2) return m_help(std::cout), EXIT_SUCCESS;

  // attempt parsing now
  CLI11_PARSE(*m_app, argc, argv);

  // if we reach the end, then declare a success
  return EXIT_SUCCESS;
}

void Shell::Command::Program::subscribe($::Unique::Pointer<Abstract> &&action) {
  // get the details about the action
  auto name = action->name();

  // prepare the subcommand that is available to be used
  auto *command = m_app->add_subcommand($::String::Buffer(name));
  command->callback([name] { $_ABORT("Action '{0}' not yet implemented", name); });

  // emplace the comand as necessary now onto the list
  action->m_subscribe(command), m_commands.emplace(name, std::move(action));
}

$::String::Buffer
Shell::Command::Program::Formatter::make_help(const CLI::App *app, $::String::Buffer, CLI::AppFormatMode) const {
  // get the underlying name of the application
  auto name = app->get_name();

  // compare against the program name for base details
  if (name == m_program->m_app->get_name()) return m_program->help();

  // and show the help details now for the incoming subcommand
  return m_program->m_commands.at(name)->help();
}

//  PRIVATE METHODS  //

void Shell::Command::Program::m_revision() { $::Debug::println("v" SABRE_MM_REVISION); }
void Shell::Command::Program::m_vendors() {
  // prints all the incoming vendor version available
  for (const auto &[name, version] : Sabre::Product::vendors()) $::Debug::println("{0} - {1}", name, version);
}

void Shell::Command::Program::m_help(std::ostream &os) const { os << g_descriptor.help(); }
