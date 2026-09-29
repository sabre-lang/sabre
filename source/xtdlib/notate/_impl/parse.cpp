/// Library Includes
#include "xtdlib/notate/parse.hpp"
#include "xtdlib/string/trim.hpp"
#include "xtdlib/utility/static.hpp"

//  TYPEDEFS  //

/// @brief Section Dispatch Handler.
struct $::Notate::Section::Dispatch : private $::Ensure::Static {
  //  PUBLIC METHODS  //

  /**
   * @brief Handles pre-processing segments.
   * @param segments                  Content to process.
   * @param options                   Parsing options.
   */
  static inline Block process(const Segments &segments, const Options &options) {
    // prepare the current fenced status
    auto fenced = false;

    // prepare the processed set of segments
    auto processed = Block({Segments()});

    // iterate over the available items
    for (const auto &segment : segments) {
      // determine if the current section is tagged
      auto tagged = !fenced && segment.starts_with('@');

      // depending on the tag either emplace a section or segment
      if (tagged) processed.emplace_back(Section({segment}));
      else processed.back().m_segments.emplace_back(segment);

      // post-condition we want to update the current fencing
      if (segment.starts_with(options.fence)) fenced = !fenced;
    }

    // remove any empty sections now
    std::erase_if(processed, [](const Section &section) { return section.empty(); });

    // return the resulting processed items now
    return processed;
  }

  /**
   * @brief Handles classifying notation comments.
   * @param block                 Block to classify.
   * @param options               Parsing options.
   */
  static inline void classify(Block &block, const Options &options) {
    for (auto &section : block) classify(section, options);
  }

  /**
   * @brief Handles classifying notation comments.
   * @param section               Section to classify.
   * @param options               Parsing options.
   */
  static inline void classify(Section &section, const Options &options) {
    // ignore when sections are empty
    if (section.m_segments.empty()) return;

    // get the leading segment to classify
    auto segment = section.m_segments.front();
    auto distance = segment.size(); // sizing

    // check if we have a leading "@" character
    if (!segment.starts_with('@')) return;

    // trim the leading item now
    segment = Trim::leading(segment.substr(1));

    // attempt resolving the current label
    auto [ls, le] = m_identifier(segment, distance);
    segment = Trim::leading(segment.substr(le - ls));
    section.m_label = ((ls & 0xFF) << 8) | (le & 0xFF);

    // check if parameterization is actually allowed
    if (options.named.contains(section.label())) {
      auto [ps, pe] = m_identifier(segment, distance);
      segment = Trim::leading(segment.substr(pe - ps));
      section.m_param = ((ps & 0xFF) << 8) | (pe & 0xFF);
    }

    // lastly note the current description offset now
    section.m_describe = distance - segment.size();
  }

private:
  //  PRIVATE METHODS  //

  /**
   * @brief Resolves identifier offsets.
   * @param segment                 Segment to resolve.
   * @param distance                Original distances.
   */
  static inline constexpr std::pair<uint8_t, uint8_t> m_identifier(const String::View &segment, size_t distance) {
    // note the starting position to use now
    uint8_t start = distance - segment.size(), ending = distance;

    // check if we find a suitable ending character now
    auto whitespace = segment.find_first_of(' ');

    // prepare the ending value now and ensure we fix empty items
    if (whitespace != String::Term) ending = start + whitespace;

    // and return the resulting details to be used
    return {start, ending};
  }
};

//  PUBLIC METHODS  //

$::Notate::Block $::Notate::Parse(const String::View &block, const Options &options) {
  // determine the total lines
  auto count = std::ranges::count(block, '\n');

  // prepare a set of lines to be split into
  auto segments = Segments();
  segments.reserve(count);

  // and split into a desired set of segments now
  for (auto &&view : std::views::split(block, '\n')) {
    String::View line = {view.begin(), view.end()};
    segments.emplace_back(Trim::both(line)); // emplace
  }

  // call the segmented parser now
  return Parse(segments, options);
}

$::Notate::Block $::Notate::Parse(const Segments &segments, const Options &options) {
  // start by pre-processing incoming markdown that is typically fenced
  auto sections = Section::Dispatch::process(segments, options);

  // once processed, we need to classify each section
  return Section::Dispatch::classify(sections, options), sections;
}

$::Notate::Block $::Notate::Parse(const std::vector<String::Buffer> &buffers, const Options &options) {
  // prepare a set of segments to be used
  auto segments = Segments(buffers.size());

  // bind all the segments now
  for (size_t ii = 0; ii < buffers.size(); ++ii) segments[ii] = buffers[ii];

  // and attempt parsing with the view-based model
  return Parse(segments, options);
}
