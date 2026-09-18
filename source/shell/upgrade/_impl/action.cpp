/// Vendor Includes
#include <xpc/xpc.hpp>

/// Sabre Includes
#include <sabre/product/details.hpp>

/// Shell Includes
#include "shell/command/macros.hpp"
#include "shell/upgrade/action.hpp"

//  X-MACROS  //

#define XX_OPTIONS_LIST(X)                                             \
  X("--dry", "Perform all upgrade checks without actions")             \
  X("--tag", "The version tag to upgrade/downgrade (default: latest)") \
  X("--force", "Forcefully replace the current executable")            \
  X("--quiet", "Hide all verbose diagnostics whilst upgrading")        \
  X("--remove", "Removes the Sabre installation instance")             \
  X("", "")                                                            \
  SHELL_XX_OPTIONS_COMMON(X)

//  CONSTRUCTORS  //

Shell::Upgrade::Action::Action() : Abstract("upgrade") {
#define X(N, D, ...) {$::Color::ANSI(N), D},
  m_descriptor.options({XX_OPTIONS_LIST(X)});
#undef X
}

//  PRIVATE METHODS  //

$::FS::Path Shell::Upgrade::Action::m_installation() const {
  return $::Path::canonical("..", $::Executable::dirname());
}

void Shell::Upgrade::Action::m_execute() {
  // prepare the script to be used for re-installation now
  auto script = Sabre::Product::scripts() / "install.sh";
  if ($_PLATFORM_WINDOWS) script.replace_extension(".ps1");

  // define the necessary binary and arguments based on the platform
#if $_PLATFORM_WINDOWS
  auto binary = "powershell"; // define a suitable shell to be used
  XPC::Shell::Args argv = {"-File", script.string(), "-Tag", m_options.tag};
#else
  auto binary = "bash"; // define a suitable shell to be used
  XPC::Shell::Args argv = {script.string(), "--tag", m_options.tag};
#endif

  // update some options if necessary
  if (m_options.dry) argv.emplace_back($_PLATFORM_WINDOWS ? "-Dry" : "--dry");
  if (m_options.force) argv.emplace_back($_PLATFORM_WINDOWS ? "-Force" : "--force");

  // prepare the required verbose details as well
  auto redirect = m_options.verbose ? XPC::Pipe::Direct::INHERIT : XPC::Pipe::Direct::IGNORE;

  // construct the builder now to be used
  auto builder = XPC::Shell::Builder(binary).argv(argv).cin(redirect).cout(redirect).cerr(redirect);

  // and attempt running the installer now
  auto errc = XPC::Process::Child(builder).wait();
  if (errc != 0) throw CLI::RuntimeError(errc);
}

void Shell::Upgrade::Action::m_subscribe(CLI::App *command) {
  // prepare all the incoming details
  command->add_flag("--dry", m_options.dry);
  command->add_flag("--force", m_options.force);

  // prepare the options now
  command->add_option("--tag", m_options.tag);
  command->add_flag_callback("--quiet", [&] { m_options.verbose = false; });

  // prepare the removal handler as well
  command->add_flag_callback("--remove", [&] {
    // request the removal handler (platform based)
    m_remove();

    std::cout << $::Dye::green("Removing") << ": "; // show success result
    std::cout << $::Dye::dim("Successfully uninstalled Sabre!") << std::endl;

    // ensure we exit the current upgrade sequence
    throw CLI::Success();
  });

  // and set the callback to be used
  command->callback(std::bind(&Action::m_execute, this));
}
