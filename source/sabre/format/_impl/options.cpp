/// Sabre Includes
#include "sabre/format/options.hpp"

//  PRIVATE METHODS  //

Sabre::Format::Options Sabre::Format::Options::m_decode(const $::Serde::Value &value) {
  return {
      .term_insert = value.at<$::Serde::Boolean>("terminator.insert", true),
      .newline_final = value.at<$::Serde::Boolean>("newline.final", true),
      .indent_spaces = value.at<$::Serde::Boolean>("indent.spaces", true),
      .arrows_align = value.at<$::Serde::Boolean>("arrows.align", true),
      .commas_trailing = value.at<$::Serde::Boolean>("commas.trailing", true),

      .braces_expand = value.at<$::Serde::Boolean>("braces.expand", true),
      .braces_preserve = value.at<$::Serde::Boolean>("braces.preserve", true),

      .line_width = value.at<size_t>("line.width", 120),
      .indent_width = value.at<size_t>("indent.width", 4),
  };
}
