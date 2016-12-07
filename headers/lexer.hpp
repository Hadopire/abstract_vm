#pragma once

#include <string>
#include <cctype>
#include <sstream>
#include <regex>
#include <functional>
#include <utility>

#include "error_formatter.hpp"

enum class TokenType {
  kInt8,
  kInt16,
  kInt32,
  kFloat,
  kDouble,
  kAdd,
  kSub,
  kMul,
  kDiv,
  kMod,
  kPush,
  kPop,
  kDump,
  kPrint,
  kAssert,
  kExit,
  kComment,
  kNewLine,
  kEndOfInput
};

struct Token {
  TokenType type;
  std::string value;
  size_t line;
  size_t column;
};

class Lexer {

  public:
    Lexer();
    Lexer(const std::string & src);
    Lexer(const Lexer & lexer);

    const Token next();
    void setFormatter(const ErrorFormatter & formatter);

    Lexer & operator=(const Lexer & rhs);

    ~Lexer();

    class Exception : public std::exception {
      public:
        Exception();
        Exception(const std::string & err);
        Exception(const Exception & exception);

        Exception & operator=(const Exception & rhs);

        const char * what() const throw();

        ~Exception();

      private:
        std::string message;
    };

  private:
    std::string mSrc;
    std::vector<std::string> mSplitSrc;

    size_t mPos{0};
    size_t mLine{1};
    size_t mColumn{1};

    ErrorFormatter mFormatter;

    struct TokenHandler {
      TokenType type;
      std::function<const Token (Lexer &, TokenType, std::string)> handlerFunc;
    };

    const std::pair<std::string, TokenHandler> mRegToHandler[17]{
      {"int8\\(.+\\)",   {TokenType::kInt8, &Lexer::numericToken}},
      {"int16\\(.+\\)",  {TokenType::kInt16, &Lexer::numericToken}},
      {"int32\\(.+\\)",  {TokenType::kInt32, &Lexer::numericToken}},
      {"float\\(.+\\)",  {TokenType::kFloat, &Lexer::numericToken}},
      {"double\\(.+\\)", {TokenType::kDouble, &Lexer::numericToken}},
      {"add",                             {TokenType::kAdd, &Lexer::basicToken}},
      {"sub",                             {TokenType::kSub, &Lexer::basicToken}},
      {"mul",                             {TokenType::kMul, &Lexer::basicToken}},
      {"div",                             {TokenType::kDiv, &Lexer::basicToken}},
      {"mod",                             {TokenType::kMod, &Lexer::basicToken}},
      {"push",                            {TokenType::kPush, &Lexer::basicToken}},
      {"pop",                             {TokenType::kPop, &Lexer::basicToken}},
      {"dump",                            {TokenType::kDump, &Lexer::basicToken}},
      {"print",                           {TokenType::kPrint, &Lexer::basicToken}},
      {"assert",                          {TokenType::kAssert, &Lexer::basicToken}},
      {"exit",                            {TokenType::kExit, &Lexer::basicToken}},
      {";(.+)?",                          {TokenType::kComment, &Lexer::commentToken}}
    };

    const Token numericToken(TokenType type, std::string value);
    const Token basicToken(TokenType type, std::string value);
    const Token commentToken(TokenType type, std::string value);

    std::string extractTokenString();

    void error(const std::string & message, size_t line, size_t column) const;
};
