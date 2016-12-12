#include "parser.hpp"


Parser::Exception::Exception() : message("parser exception") {}

Parser::Exception::Exception(const std::string & src) : message(src) {}

Parser::Exception::Exception(const Parser::Exception & exception) : message(exception.message) {}

Parser::Exception & Parser::Exception::operator=(const Parser::Exception & rhs) {
  message = rhs.message;
  return *this;
}

const char * Parser::Exception::what() const throw() {
  return message.c_str();
}

Parser::Exception::~Exception() {}

Parser::Parser() {}

Parser::Parser(const std::list<Token> & tokens) : mTokens(tokens) {
  mPos = mTokens.begin();
}

Parser::Parser(const Parser & parser) : mTokens(parser.mTokens), mPos(parser.mPos), mFormatter(parser.mFormatter) {}

Parser & Parser::operator=(const Parser & rhs) {
  mTokens = rhs.mTokens;
  mPos = rhs.mPos;
  mFormatter = rhs.mFormatter;
  return *this;
}

Parser::~Parser() {}

std::function<void (Machine &)> Parser::next() {
  std::vector<Token> expr = nextExpr();

  if (expr.size() == 0) {
    throw Parser::Exception("No more tokens to parse");
  }

  switch (expr[0].type) {
    case TokenType::kPush:
      pushExpr(expr);
      break;
    case TokenType::kAssert:
      assertExpr(expr);
      break;
    case TokenType::kEndOfInput:
      endOfInputExpr(expr);
      break;
    default:
      basicExpr(expr);
      break;
  }

  return [](Machine & m) { return; };
}

std::function<void (Machine &)> Parser::pushExpr(std::vector<Token> & expr) {

  return [](Machine & m) { return; };
}

std::function<void (Machine &)> Parser::assertExpr(std::vector<Token> & expr) {

  return [](Machine & m) { return; };
}

std::function<void (Machine &)> Parser::basicExpr(std::vector<Token> & expr) {

  return [](Machine & m) { return; };
}

std::function<void (Machine &)> Parser::endOfInputExpr(std::vector<Token> & expr) {

  if (!mExit) {
    error("Missing 'exit' instruction", 0, 0);
  }

  return [](Machine & m) { return; };
}

void Parser::setFormatter(const ErrorFormatter & formatter) {
  mFormatter = formatter;
}

bool Parser::isFinish() {
  return mPos == mTokens.end();
}

std::vector<Token> Parser::nextExpr() {
  while (mPos != mTokens.end() && (mPos->type == TokenType::kNewLine ||
        mPos->type == TokenType::kComment)) {
    ++mPos;
  }

  std::vector<Token> tokens;

  while (mPos != mTokens.end() && (mPos->type != TokenType::kNewLine ||
        mPos->type != TokenType::kComment)) {
    if (mPos->type == TokenType::kExit) {
      mExit = true;
    }

    if (mPos->type == TokenType::kEndOfInput && tokens.size() > 0) {
      return tokens;
    }

    tokens.push_back(*mPos);
    ++mPos;
  }

  return tokens;
}

void Parser::error(const std::string & message, size_t line, size_t column) const {
  std::string err = message;

  if (mFormatter.isActive()) {
    if (line < 1 || column < 1) {
      err = mFormatter.formatBasic(message, line, column);
    }
    else {
      err = mFormatter.format(message, line, column);
    }
  }
  else {
    err.append("\n");
  }

  throw Parser::Exception(err);
}
