#include "lexer.h"

Lexer::Exception::Exception() : message("lexer exception") {}

Lexer::Exception::Exception(const char * src) : message(src) {}

Lexer::Exception::Exception(const Lexer::Exception & exception) : message(exception.message) {}

Lexer::Exception & Lexer::Exception::operator=(const Lexer::Exception & rhs) {
  message = rhs.message;
  return *this;
}

const char * Lexer::Exception::what() const throw() {
  return this->message;
}

Lexer::Exception::~Exception() {}

Lexer::Lexer() {}

Lexer::Lexer(const std::string & src) : mSrc(src) {}

Lexer::Lexer(const Lexer & lexer) : mSrc(lexer.mSrc) {}

const Token Lexer::next() {
  while (mPos < mSrc.size() && (mSrc[mPos] == ' ' || mSrc[mPos] == '\t')) {
    ++mPos;
    ++mColumn;
  }

  if (mPos >= mSrc.size()) {
    return {TokenType::kEndOfInput};
  }
  else if (mSrc[mPos] == '\n') {
    ++mPos;
    ++mLine;
    mColumn = 1;
    return {TokenType::kNewLine, "\n"};
  }

  throw std::runtime_error("fuck\n");
}

void Lexer::setFormatter(const ErrorFormatter & formatter) {
  mFormatter = formatter;
}

const Token Lexer::numericToken(TokenType type, std::string value) {
  std::string numLiteral(value.begin() + value.find('('), value.end() - 1);
  std::regex numRegex("[0-9]+\\.?([0-9]+)?");

  Token token{type, numLiteral, mLine, mColumn};
  mColumn += value.size();

  if (std::regex_match(numLiteral, numRegex) == false) {
    error("Invalid syntax, expected integer or floating point literal '" + value + "'", mLine, mColumn + value.find('('));
  }

  return token;
}

void Lexer::error(const std::string & message, size_t line, size_t column) const {
  std::string err = message;

  if (mFormatter.isActive()) {
    err = mFormatter.format(message, line, column);
  }
  else {
    err.append("\n");
  }

  throw Lexer::Exception(err.c_str());
}

Lexer & Lexer::operator=(const Lexer & rhs) {
  mSrc = rhs.mSrc;
  mPos = rhs.mPos;
  return *this;
}

Lexer::~Lexer() {}
