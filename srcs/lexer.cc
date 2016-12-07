#include "lexer.hpp"
#include <iostream>

Lexer::Exception::Exception() : message("lexer exception") {}

Lexer::Exception::Exception(const std::string & src) : message(src) {}

Lexer::Exception::Exception(const Lexer::Exception & exception) : message(exception.message) {}

Lexer::Exception & Lexer::Exception::operator=(const Lexer::Exception & rhs) {
  message = rhs.message;
  return *this;
}

const char * Lexer::Exception::what() const throw() {
  return message.c_str();
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
    return {TokenType::kNewLine, "<NewLine>"};
  }

  std::string tokenString = extractTokenString();
  for (auto & handler : mRegToHandler) {
    std::regex reg(handler.first);

    if (std::regex_match(tokenString, reg) == true) {
      return handler.second.handlerFunc(*this, handler.second.type, tokenString);
    }
  }

  mColumn += tokenString.size();
  mPos += tokenString.size();
  error("Unknown token '" + tokenString + "'", mLine, mColumn - tokenString.size());

  return {};
}

void Lexer::setFormatter(const ErrorFormatter & formatter) {
  mFormatter = formatter;
}

const Token Lexer::numericToken(TokenType type, std::string value) {
  std::string numLiteral(value.begin() + value.find('(') + 1, value.end() - 1);
  std::regex numRegex("[0-9]+\\.?([0-9]+)?");

  Token token{type, numLiteral, mLine, mColumn};
  mColumn += value.size();
  mPos += value.size();

  if (std::regex_match(numLiteral, numRegex) == false) {
    error("Invalid syntax, expected integer or floating point literal '" + value + "'", mLine, mColumn - value.size() + value.find('(') + 1);
  }

  return token;
}

const Token Lexer::basicToken(TokenType type, std::string value) {
  Token token{type, value, mLine, mColumn};
  mColumn += value.size();
  mPos += value.size();

  return token;
}

const Token Lexer::commentToken(TokenType type, std::string value) {
  Token token{type, "<Comment>", mLine, mColumn};
  while (mPos < mSrc.size() && mSrc[mPos] != '\n') {
    ++mPos;
    ++mColumn;
  }

  return token;
}

std::string Lexer::extractTokenString() {
	size_t nPos = mPos + 1;

	while (nPos < mSrc.size() && !isspace(mSrc[nPos]) && mSrc[nPos] != ';') {
		++nPos;
	}

	return std::string(mSrc.begin() + mPos, mSrc.begin() + nPos);
}

void Lexer::error(const std::string & message, size_t line, size_t column) const {
  std::string err = message;

  if (mFormatter.isActive()) {
    err = mFormatter.format(message, line, column);
  }
  else {
    err.append("\n");
  }

  throw Lexer::Exception(err);
}

Lexer & Lexer::operator=(const Lexer & rhs) {
  mSrc = rhs.mSrc;
  mPos = rhs.mPos;
  return *this;
}

Lexer::~Lexer() {}
