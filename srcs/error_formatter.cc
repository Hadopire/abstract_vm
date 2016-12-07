#include <sstream>
#include <cstring>

#include "error_formatter.hpp"

ErrorFormatter::ErrorFormatter() {}

ErrorFormatter::ErrorFormatter(const std::string & src) {
  setSource(src);
}

ErrorFormatter::ErrorFormatter(const ErrorFormatter & formatter) :
  mSplitSrc(formatter.mSplitSrc),
  mIsActive(formatter.mIsActive) {}

ErrorFormatter & ErrorFormatter::operator=(const ErrorFormatter & rhs) {
  mSplitSrc = rhs.mSplitSrc;
  mIsActive = rhs.mIsActive;

  return *this;
}

ErrorFormatter::~ErrorFormatter() {}

void ErrorFormatter::setSource(const std::string & src) {
  std::stringstream stream;
  stream.str(src);

  std::string item;
  while (std::getline(stream, item)) {
    mSplitSrc.push_back(item);
  }

  mIsActive = true;
}

const std::string ErrorFormatter::format(const std::string & message, size_t line, size_t column) const {
  std::stringstream stream;

  stream << line << ":" << column << " error: " << message << std::endl;
  stream << mSplitSrc[line - 1] << std::endl;

  int c;
  for (c = 1; c < column; ++c) {
    if (mSplitSrc[line - 1][c - 1] == '\t') {
      stream << "\t";
    }
    else {
      stream << " ";
    }
  }

  stream << "^";
  for (++c; c < mSplitSrc[line - 1].size(); ++c) {
    if (isspace(mSplitSrc[line - 1][c - 1]) || mSplitSrc[line - 1][c - 1] == ';') {
      break;
    }
    stream << "~";
  }

  stream << std::endl;
  return stream.str();
}

bool ErrorFormatter::isActive() const {
  return mIsActive;
}
