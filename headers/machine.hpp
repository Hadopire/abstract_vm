#pragma once

#include <string>
#include <deque>
#include "lexer.hpp"
#include "eoperand_type.hpp"
#include "ioperand.hpp"
#include "error_formatter.hpp"

class Machine {
  public:
    Machine();
    Machine(const Machine & machine);

    void setFormatter(const ErrorFormatter & formatter);

    void Add(const Token & token);
    void Sub(const Token & token);
    void Mul(const Token & token);
    void Div(const Token & token);
    void Mod(const Token & token);
    void Push(const Token & token, eOperandType type, const std::string & value);
    void Pop(const Token & token);
    void Print(const Token & token);
    void Dump(const Token & token);
    void Assert(const Token & token, eOperandType type, const std::string & value);
    void Exit(const Token & token);

    Machine & operator=(const Machine & rhs);

    ~Machine();

    class Exception : public std::runtime_error {
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
    std::deque<const IOperand *> mStack;
    ErrorFormatter mFormatter;

    void error(const std::string & message, size_t line, size_t column) const;
};
