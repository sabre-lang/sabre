/// Shell Includes
#include "shell/format/action.hpp"
#include "shell/command/macros.hpp"
#include "shell/format/path.hpp"

//  X-MACROS  //

#define XX_OPTIONS_LIST(X)                                                 \
  X("--write", "Overwrite all input targets with the formatted result")    \
  X("--config", "Path to a common formatting configuration manifest file") \
  X("", "")                                                                \
  SHELL_XX_OPTIONS_COMMON(X)

//  CONSTRUCTORS  //

Shell::Format::Action::Action() : Abstract("format") {
#define X(N, D, ...) {$::Color::ANSI(N), D},
  m_descriptor.positionals({{"targets...", false}}).options({XX_OPTIONS_LIST(X)});
#undef X
}

//  PRIVATE METHODS  //

void Shell::Format::Action::m_execute() {
  // prepare all the scripts to be formatted now
  auto scripts = std::vector<$::URI::Buffer>();
  auto invalid = std::vector<$::String::View>();

  // attempt scanning for valid targets now
  for (const auto &target : m_targets) {
    // attempt finding all the targets to format
    auto found = Path(target);

    // update the desired sections now
    if (found.empty()) invalid.emplace_back(target);
    else $::Ranges::Append(scripts, found);
  }

  // fail if there were any invalid items
  if (invalid.size()) return m_fail("Format targets given are invalid");

  // stop if there were no scripts actually given
  if (scripts.empty()) return m_fail("Expected at least one format target");

  // ensure we have only one-script when not writing
  if (!m_write && scripts.size() > 1) m_fail("Dry mode expects only one script to format");

  auto results = std::vector<Result>(); // prepare
  auto futures = std::vector<std::future<Result>>();

  for (const auto &script : scripts) futures.emplace_back(std::async(&Action::m_worker, this, script));
  for (auto &future : futures) results.emplace_back(future.get()); // accumulate the results now

  // handle outputing results as necessary
  for (size_t ii = 0; ii < results.size(); ++ii) {
    auto result = results.at(ii);
    auto resource = scripts.at(ii);

    // handle the result as necessary
    m_write ? m_overwrite(resource, result) : m_output(resource, result);
  }
}

void Shell::Format::Action::m_subscribe(CLI::App *command) {
  // we allow all positionals to get passed through
  command->positionals_at_end(true);

  // prepare the available flags and options now
  command->add_flag("--write", m_write);

  // prepare the positionals that will be available now
  command->add_option("targets", m_targets);

  // allow overwriting the desired configuration options
  command->add_option_function<$::String::Buffer>("--config", [&](const $::String::Buffer &config) {
    m_options = m_resolve(config);
  });

  // set the necessary callback to run the instance now
  command->callback(std::bind(&Action::m_execute, this));
}
