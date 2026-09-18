/// Sabre Includes
#include "sabre/function/info.hpp"
#include "sabre/image/arena.hpp"

//  PRIVATE METHODS  //

void Sabre::Image::Arena::m_print(std::ostream &os, const Arena &self) {
  // print all the available functions and their instructions
  for (const auto &function : self.functions) os << *function << "\n\n";

  // print the incoming constants pool now
  os << "<Constant Pool (size: " << self.constants.size() << ")>";
  for (const auto &[ii, constant] : $::Ranges::Each(self.constants)) {
    $_UNUSED $_AUTO = $::Manip::Indent(os);
    os << "\n[" << ii << "]: " << constant;
  }

  if (self.constants.size()) os << '\n'; // print the intern pool
  os << "\n<Interns Pool (size: " << self.strings.size() << ")>";
  for (const auto &[ii, intern] : $::Ranges::Each(self.strings)) {
    $_UNUSED $_AUTO = $::Manip::Indent(os); // prepare indentation before printing
    os << "\n[" << ii << "]: " << $::Dye::green(*$::Serde::Escape(intern.view()));
  }
}
