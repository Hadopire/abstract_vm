#include <algorithm>
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
      return pushExpr(expr);
      break;
    case TokenType::kAssert:
      return assertExpr(expr);
      break;
    case TokenType::kEndOfInput:
      return endOfInputExpr(expr);
      break;
    default:
      return basicExpr(expr);
      break;
  }

  return [](Machine & m) { };
}

std::function<void (Machine &)> Parser::pushExpr(std::vector<Token> & expr) {

  if (expr.size() > 2) {
    error("Expected newline", expr[2].line, expr[2].column);
  }
  else if (expr.size() == 1) {
    error("Missing value to push", expr[0].line, expr[0].column);
  }

  std::string value;

  if (std::find(mIntegerTypes.begin(), mIntegerTypes.end(), expr[1].type) != mIntegerTypes.end()) {
    if (std::find(expr[1].value.begin(), expr[1].value.end(), '.') != expr[1].value.end()) {
      error("Expected integer value", expr[1].line, expr[1].column);
    }
    value = expr[1].value;
  }
  else if (std::find(mFloatTypes.begin(), mFloatTypes.end(), expr[1].type) != mFloatTypes.end()) {
    value = expr[1].value;
  }
  else {
    error("'" + expr[1].value + "' is not a correct value to push", expr[1].line, expr[1].column);
  }

  return [expr, value](Machine & m) { m.push(expr[0], static_cast<eOperandType>(expr[1].type), value); };
}

std::function<void (Machine &)> Parser::assertExpr(std::vector<Token> & expr) {

  if (expr.size() > 2) {
    error("Expected newline", expr[2].line, expr[2].column);
  }
  else if (expr.size() == 1) {
    error("Missing value to assert", expr[0].line, expr[0].column);
  }

  std::string value;

  if (std::find(mIntegerTypes.begin(), mIntegerTypes.end(), expr[1].type) != mIntegerTypes.end()) {
    if (std::find(expr[1].value.begin(), expr[1].value.end(), '.') != expr[1].value.end()) {
      error("Expected integer value", expr[1].line, expr[1].column);
    }
    value = expr[1].value;
  }
  else if (std::find(mFloatTypes.begin(), mFloatTypes.end(), expr[1].type) != mFloatTypes.end()) {
    value = expr[1].value;
  }
  else {
    error("'" + expr[1].value + "' is not a correct value to assert", expr[1].line, expr[1].column);
  }

  return [expr, value](Machine & m) { m.assert(expr[0], static_cast<eOperandType>(expr[1].type), value); };
}

std::function<void (Machine &)> Parser::basicExpr(std::vector<Token> & expr) {

  if (expr.size() > 1) {
    error("Expected newline", expr[1].line, expr[1].column);
  }

  std::function<void (Machine &, const Token &)> instruction = mTokenToInst[expr[0].type];
  return [instruction, expr](Machine & m) { instruction(m, expr[0]); };
}

std::function<void (Machine &)> Parser::endOfInputExpr(std::vector<Token> & expr) {

  if (!mExit) {
    error("Missing 'exit' instruction", 0, 0);
  }

  return [](Machine & m) { };
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
  while (mPos != mTokens.end() && mPos->type != TokenType::kNewLine &&
        mPos->type != TokenType::kComment) {
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
