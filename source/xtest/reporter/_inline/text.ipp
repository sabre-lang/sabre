#ifndef _XTEST_REPORTER_TEXT_IPP
#define _XTEST_REPORTER_TEXT_IPP

/// Testing Includes
#include "xtest/reporter/stream.hpp"

namespace XT::Reporter {

/// @brief Text Reporter Implementation.
template <> class Proxy<Label::TEXT> : public Stream {
  //  TYPEDEFS  //

  /// @brief Benchmark record.
  using Record = std::pair<const Bench::Mark *, Bench::Result>;

  //  PROPERTIES  //

  /// @brief Current flushing index.
  size_t m_fidx = 0;

  /// @brief Indentation size.
  size_t m_indent = 2;

  /// @brief All recorded benchmark results.
  std::vector<Record> m_records = {};

  /// @brief The current available spinner.
  $::Spinner::Pointer m_spinner = nullptr;

public:
  //  CONSTRUCTORS  //

  /// @brief Inherit the base constructor.
  using Stream::Stream;

  //  PUBLIC METHODS  //

  /**
   * @brief After running hook.
   * @param sections          Total sections.
   * @param elapsed           Elapsed duration.
   */
  inline void after_running(size_t sections, const $::Chrono::Duration &elapsed) final {
    for (const auto &[test, benchmark] : m_records) { // show all the benchmark results now
      m_stream << $::Dye::bold(test->trivia()->title) << ": " << $::Dye::dim("{0} runs", benchmark.runs);
      m_stream << '\n', m_time(benchmark), m_stream << '\n', m_range(benchmark), m_stream << "\n\n";
    }

    // destructure some items
    const auto &stats = m_stats();
    const auto &ordering = m_runner->options()->ordering;

    // get the final totals to be used
    auto total = stats.total();
    auto failed = stats.failed();
    auto real = $::Chrono::Duration(stats.elapsed());

    // show the immediate results now if there was a total
    if (total) {
      m_stream << ' ' << stats.passed() << " passed\n";
      m_stream << ' ' << failed << " failed\n";
      m_stream << ' ' << stats.skipped() << " skipped\n\n";
    }

    // prepare a function to pluralize values
    static auto s_pad = [](const $::String::View &input, size_t size) { return fmt::format("{0:{1}}", input, size); };
    static auto s_plural = [](size_t value, const $::String::View &input = "") -> $::String::Buffer {
      return fmt::to_string(value) + input + (value == 1 ? "" : "s");
    };

    // get the expected formatting padding to be used
    auto seed = ordering.randomize ? fmt::to_string(ordering.seed) : "unset";
    auto randomize = fmt::format("Randomization: {0}", (ordering.randomize ? "Enabled " : "Disabled"));
    auto header = fmt::format("Ran {0} across {1} ", total, s_plural(total, " test"), s_plural(sections, " section"));
    auto padding = std::max(header.size(), randomize.size()); // determine a suitable padding to be used

    // show bailout statistics if necessary to be shown
    if (m_runner->bailout()) {
      m_stream << $::Dye::red("{0:{1}}", "Bailed out!", padding);
      m_stream << $::Dye::dim("[After {0}]", s_plural(failed, " failure")) << '\n';
    }

    // show the final result as necessary now
    m_stream << s_pad(randomize, padding) << $::Dye::dim("[Input Seed: {0}]", seed);
    m_stream << std::endl << s_pad(header, padding) << $::Dye::dim("[R: {0}, T: {1}]", real, elapsed) << std::endl;
  }

  /**
   * @brief Group opening hook.
   * @param group             Group starting.
   */
  inline void group_opened(const Handle::Group *group) final {
    // get the traits of the group
    auto *trivia = group->trivia();

    // show the padding to be used
    m_stream << m_before();

    // handle based on the current group state
    if (trivia->skip) m_stream << $::Dye::yellow("Skipping");
    else if (trivia->todo) m_stream << $::Dye::magenta("Unimplemented");
    else m_stream << $::Dye::cyan("Testing"); // otherwise valid

    // prepare the location to be used and whether we need to show it
    auto location = fmt::to_string(trivia->location);
    auto pinpoint = trivia->title != location && !trivia->location.anonymous();

    m_stream << ": " << $::Dye::italic(trivia->title); // and show now
    if (pinpoint) m_stream << $::Dye::dim(" -> {0}", trivia->location);
    m_stream << std::endl; // and close the location now
  }

  /**
   * @brief Group closing hook.
   * @param group             Group ending.
   */
  inline void group_closed(const Handle::Group *) final {
    if (m_depth > 1) return; // ignore flushing
    if (m_pending().size()) m_stream << '\n', m_flush();
    m_stream << '\n'; // and show some final depth
  }

  /**
   * @brief Group failure hook.
   * @param group             Group failed.
   * @param reason            Failure reason.
   */
  inline void group_failure(const Handle::Group *, const $::String::Buffer &) final {}

  /**
   * @brief Test starting hook.
   * @param test              Test started.
   */
  inline void test_starting(const Handle::Base *test) final {
    // ensure that there are no active tests
    $_ASSERT(m_spinner == nullptr, "Expected no active tests");

    // get the incoming runner options to be used
    auto *options = m_runner->options();

    // determine if we have a suitable output to show spinner details to
    if (!$::Spinner::Enabled(*options->reporter.output)) return;

    // prepare a suffix and prefix to be used
    auto prefix = m_before(), suffix = fmt::to_string($::Dye::dim(test->trivia()->title));

    // construct the current spinner instance
    m_spinner = options->reporter.spinner(
        $::Spinner::Options{
            .prefix = prefix,
            .suffix = suffix,
            .output = *options->reporter.output,
        }
    );
  }

  /**
   * @brief Test teardown hook.
   * @param test              Test finished.
   */
  inline void test_teardown(const Handle::Base *) final { m_spinner = nullptr; }

  /**
   * @brief Test successful hook.
   * @param test              Test finished.
   * @param duration          Test duration.
   */
  inline void test_success(const Handle::Base *test, const $::Chrono::Duration &duration) final {
    m_finalize(test, $::Dye::green("✓"), fmt::to_string(duration));
  }

  /**
   * @brief Test failed hook.
   * @param test              Test finished.
   * @param reason            Failure reason.
   */
  inline void test_failure(const Handle::Base *test, const $::String::Buffer &reason) final {
    m_finalize(test, $::Dye::red("✗"), reason);
  }

  /**
   * @brief Test timeout hook.
   * @param test              Test finished.
   * @param timeout           Timeout duration.
   */
  inline void test_timeout(const Handle::Base *test, const $::Chrono::Duration &timeout) final {
    m_finalize(test, $::Dye::red("!"), fmt::format("Exceeded timeout - {0}", timeout));
  }

  /**
   * @brief Test skipped result.
   * @param test              Test skipped.
   */
  inline void test_skipped(const Handle::Base *test) final { m_finalize(test, $::Dye::yellow("»")); }

  /**
   * @brief Test partial result.
   * @param test              Test unimplemented.
   */
  inline void test_unimplemented(const Handle::Base *test) final { m_finalize(test, $::Dye::magenta("-")); }

  /**
   * @brief Test progress hook.
   * @param test              Test progressing.
   * @param progress          Progress update.
   */
  inline void test_progress(const Handle::Base *test, const Trivia::Progress &progress) final {
    if (m_spinner == nullptr) return; // ensure existence before showing progress
    auto percentage = 100 * progress.iter / static_cast<double>(progress.total);
    m_spinner->suffix($::Dye::dim("{0} ({1}) - {2:.2}%", test->trivia()->title, progress.label, percentage));
  }

  /**
   * @brief Benchmark finished hook.
   * @param bench             Benchmark finished.
   * @param result            Result of benchmark.
   */
  inline void bench_finished(const Bench::Mark *bench, const Bench::Result &result) final {
    auto details = fmt::format("{0} over {1} runs", $::Chrono::Duration(result.mean), result.runs);
    m_finalize(bench, $::Dye::blue("⏺"), details), m_records.emplace_back(bench, result);
  }

private:
  //  PRIVATE METHODS  //

  /// @brief Constructs the current padding.
  inline $::String::Buffer m_before() const { return $::String::Buffer(m_depth * m_indent, ' '); }

  /// @brief Handles flushing all pending failures.
  inline void m_flush() {
    const auto &pending = m_pending(); // get the underlying pending failures
    for (size_t ii = m_fidx; ii < pending.size(); ii++) m_flush(pending.at(ii));
    m_fidx = pending.size(); // update the flush index to be used
  }

  /**
   * @brief Handles flushing all pending failures.
   * @param failure           Failure to flush.
   */
  inline void m_flush(const Assert::Failure &failure) const {
    m_stream << $::Dye::red("Failed Test") << ' '; // prepare details now
    m_stream << $::Dye::italic(failure.title) << ": " << failure.reason << '\n';

    // prepare a cache for the previous failure location (we don't want duplicates)
    auto previous = $::String::View();

    // and iteratively flush our locations in reverse
    for (const auto &location : std::views::reverse(failure.trace)) {
      // ignore empty locations here
      if (location.empty()) continue;

      // ignore if the previous was the same
      if (location == previous) continue;

      m_stream << $::Dye::dim("| {0}", location) << '\n';
      previous = location; // cache the previous location
    }
  }

  /**
   * @brief Handles showing completion items.
   * @param test              Test instance.
   * @param status            Status text.
   * @param suffix            Additional suffix text.
   */
  inline void m_finalize(const Handle::Base *test, const $::Color::ANSI &status, const $::String::View &suffix = "") {
    if (m_spinner != nullptr) m_spinner->dismiss(); // dismiss the spinner instance
    m_stream << m_before() << status << ' ' << $::Dye::dim(test->trivia()->title);
    if (suffix.size()) m_stream << ' ' << $::Dye::dim("[{0}]", suffix);
    m_stream << std::endl; // and show the suffix as well
  }

  /**
   * @brief Formats benchmark times.
   * @param record            Record to format.
   */
  inline void m_time(const Bench::Result &record) {
    m_stream << "  Time (" << $::Dye::green("mean").bold();
    m_stream << " ± " << $::Dye::green("σ").dim() << ")";

    m_stream << " - " << $::Dye::green($::Chrono::Duration(record.mean)).bold();
    m_stream << " ± " << $::Dye::green($::Chrono::Duration(record.stddev)).dim();
  }

  /**
   * @brief Formats benchmark ranges.
   * @param record            Record to format.
   */
  inline void m_range(const Bench::Result &record) {
    m_stream << "  Range (" << $::Dye::cyan("min").bold();
    m_stream << " … " << $::Dye::magenta("max") << ")";

    m_stream << " - " << $::Dye::cyan($::Chrono::Duration(record.minimum)).bold();
    m_stream << " … " << $::Dye::magenta($::Chrono::Duration(record.maximum));
  }
};

} // namespace XT::Reporter

#endif
