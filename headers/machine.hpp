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

    void add(const Token & token);
    void sub(const Token & token);
    void mul(const Token & token);
    void div(const Token & token);
    void mod(const Token & token);
    void push(const Token & token, eOperandType type, const std::string & value);
    void pop(const Token & token);
    void print(const Token & token);
    void dump(const Token & token);
    void assert(const Token & token, eOperandType type, const std::string & value);
    void exit(const Token & token);

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
