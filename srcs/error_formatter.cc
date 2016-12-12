#include <sstream>
#include <cstring>
#include <unistd.h>

#include "error_formatter.hpp"

ErrorFormatter::ErrorFormatter() {}

ErrorFormatter::ErrorFormatter(const std::string & src) {
  setSource(src);
}

ErrorFormatter::ErrorFormatter(const ErrorFormatter & formatter) :
  mSplitSrc(formatter.mSplitSrc),
  mIsActive(formatter.mIsActive),
  mDefault(formatter.mDefault),
  mRedBold(formatter.mRedBold),
  mWhiteBold(formatter.mWhiteBold),
  mGreenBold(formatter.mGreenBold) {}

ErrorFormatter & ErrorFormatter::operator=(const ErrorFormatter & rhs) {
  mSplitSrc = rhs.mSplitSrc;
  mIsActive = rhs.mIsActive;
  mDefault = rhs.mDefault;
  mRedBold = rhs.mRedBold;
  mWhiteBold = rhs.mWhiteBold;
  mGreenBold = rhs.mGreenBold;

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

  if (isatty(1)) {
    mDefault = "\033[0m";
    mRedBold = "\033[1;31m";
    mWhiteBold = "\033[1;37m";
    mGreenBold = "\033[1;32m";
  }
}

const std::string ErrorFormatter::format(const std::string & message, size_t line, size_t column) const {
  if (!mIsActive) {
    return "";
  }

  std::stringstream stream;
  stream << mWhiteBold << line << ":" << column << mRedBold << " error: " << mWhiteBold << message << std::endl;
  stream << mDefault << mSplitSrc[line - 1] << std::endl;

  stream << mGreenBold;
  size_t c;
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

  stream << std::endl << mDefault;
  return stream.str();
}

const std::string ErrorFormatter::formatBasic(const std::string & message, size_t line, size_t column) const {
  if (!mIsActive) {
    return "";
  }

  std::stringstream stream;
  stream << mWhiteBold << line << ":" << column << mRedBold << " error: " << mWhiteBold << message << std::endl;
  stream << mDefault;

  return stream.str();
}

bool ErrorFormatter::isActive() const {
  return mIsActive;
}
