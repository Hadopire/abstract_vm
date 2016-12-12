#pragma once

#include <list>
#include <functional>
#include <vector>

#include "lexer.hpp"
#include "machine.hpp"

class Parser {
  public:
    Parser();
    Parser(const std::list<Token> & tokens);
    Parser(const Parser & parser);

    void setFormatter(const ErrorFormatter & formatter);
    bool isFinish();

    Parser & operator=(const Parser & rhs);

    std::function<void (Machine &)> next();

    ~Parser();

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
    std::list<Token> mTokens;
    std::list<Token>::iterator mPos;
    ErrorFormatter mFormatter;
    bool mExit{ false };

    std::function<void (Machine &)> pushExpr(std::vector<Token> & expr);
    std::function<void (Machine &)> assertExpr(std::vector<Token> & expr);
    std::function<void (Machine &)> basicExpr(std::vector<Token> & expr);
    std::function<void (Machine &)> endOfInputExpr(std::vector<Token> & expr);

    std::vector<Token> nextExpr();
    void error(const std::string & message, size_t line, size_t column) const;
};
