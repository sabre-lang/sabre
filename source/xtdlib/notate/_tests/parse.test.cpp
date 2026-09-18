/// Vendor Includes
#include <tests/tests.hpp>

/// Forward Declarations
$_FWD(Mock, $::String::Buffer merge($::String::Buffer, const $::String::Buffer &, const $::String::Buffer &))

//  MACROS  //

#define MM_HEADER                                          \
  "This is a header section with\n"                        \
  "an additional fenced description.\n\n"                  \
  "```sabre\nDebug.println::async(\"Hello, World!\")\n```"

//  X-MACROS  //

#define XX_SECTIONS(X)                             \
  X("", MM_HEADER, "")                             \
  X("brief", "This is the leading brief text", "") \
  X("param", "First named parameter", "alpha")     \
  X("param", "Second named parameter", "beta")     \
  X("returns", "A suitable return value", "")      \
  X("empty", "", "")

//  MOCK METHODS  //

$::String::Buffer
Mock::merge($::String::Buffer output, const $::String::Buffer &description, const $::String::Buffer &name) {
  if (output.size()) output = "@" + output;
  output += (output.size() ? " " : "") + name;
  output += (output.size() ? " " : "") + description;
  return output; // return the resulting output now here
}

//  TEST CASES  //

TEST_CALLBACK(parse, runner) {
// merge all out items together now
#define X(L, D, N, ...) Mock::merge(L, D, N) + "\n" +
  auto comments = XX_SECTIONS(X) "";
  auto trimmed = $::Trim::both(comments);
#undef X

  // attempt parsing the available blocks now
  auto block = $::Notate::Parse(trimmed, {.named = {"param"}});

  // check that we have the correct number of sections
  runner->asserts()->equal(block.size(), 6);

  // prepare an index for iterating our sections
  size_t index = 0;

// check some invariants for the resulting section labels
#define X(L, D, N, ...)                                 \
  {                                                     \
    const auto &section = block.at(index++);            \
    runner->asserts()->equal(section.label(), L);       \
    runner->asserts()->equal(section.parameter(), N);   \
    runner->asserts()->equal(section.description(), D); \
  }

  XX_SECTIONS(X)
#undef X
};

//  TEST SECTIONS  //

TEST_SUITE("xtdlib")->emplace({
    TEST_GROUP("Notate/Parse", {TEST_CASE(parse)}),
});
