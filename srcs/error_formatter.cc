#include <sstream>

#include "error_formatter.h"

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
  std::string errMsg;

  stream << line << ":" << column << " error: " << message << std::endl;
  stream << mSplitSrc[line - 1] << std::endl;

  for (int c = 1; c < column; ++c) {
    if (mSplitSrc[line - 1][c - 1] == '\t') {
      stream << "\t";
    }
    else {
      stream << " ";
    }
  }

  stream << "^" << std::endl;
  stream >> errMsg;
  return errMsg;
}

bool ErrorFormatter::isActive() const {
  return mIsActive;
}
