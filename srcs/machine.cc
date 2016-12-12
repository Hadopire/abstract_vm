#include <cstdlib>
#include <iostream>

#include "machine.hpp"
#include "ioperand_factory.hpp"
#include "operand.hpp"

Machine::Exception::Exception() : runtime_error(""), message("machine exception") {}

Machine::Exception::Exception(const std::string & src) : runtime_error(src), message(src) {}

Machine::Exception::Exception(const Machine::Exception & exception) : runtime_error(exception.message), message(exception.message) {}

Machine::Exception & Machine::Exception::operator=(const Machine::Exception & rhs) {
  message = rhs.message;
  return *this;
}

const char * Machine::Exception::what() const throw() {
  return message.c_str();
}

Machine::Exception::~Exception() {}

Machine::Machine() {}

Machine::Machine(const Machine & machine) : mStack(machine.mStack), mFormatter(machine.mFormatter) {}

Machine & Machine::operator=(const Machine & rhs) {
  mStack = rhs.mStack;
  mFormatter = rhs.mFormatter;
  return *this;
}

Machine::~Machine() {}

void Machine::add(const Token & token) {
  if (mStack.size() < 2) {
    error("'add' on stack with less than two operands", token.line, token.column);
  }

  const IOperand *v1 = mStack.front();
  mStack.pop_front();
  const IOperand *v2 = mStack.front();
  mStack.pop_front();

  try {
    mStack.push_front(*v1 + *v2);
  }
  catch (IOperandFactory::Exception & e) {
    error(e.what(), token.line, token.column);
  }

  delete v1;
  delete v2;
}

void Machine::sub(const Token & token) {
  if (mStack.size() < 2) {
    error("'sub' on stack with less than two operands", token.line, token.column);
  }

  const IOperand *v1 = mStack.front();
  mStack.pop_front();
  const IOperand *v2 = mStack.front();
  mStack.pop_front();

  try {
    mStack.push_front(*v1 - *v2);
  }
  catch (IOperandFactory::Exception & e) {
    error(e.what(), token.line, token.column);
  }

  delete v1;
  delete v2;
}

void Machine::mul(const Token & token) {
  if (mStack.size() < 2) {
    error("'mul' on stack with less than two operands", token.line, token.column);
  }

  const IOperand *v1 = mStack.front();
  mStack.pop_front();
  const IOperand *v2 = mStack.front();
  mStack.pop_front();

  try {
    mStack.push_front(*v1 * *v2);
  }
  catch (IOperandFactory::Exception & e) {
    error(e.what(), token.line, token.column);
  }

  delete v1;
  delete v2;
}

void Machine::div(const Token & token) {
  if (mStack.size() < 2) {
    error("'div' on stack with less than two operands", token.line, token.column);
  }

  const IOperand *v1 = mStack.front();
  mStack.pop_front();
  const IOperand *v2 = mStack.front();
  mStack.pop_front();

  try {
    mStack.push_front(*v1 / *v2);
  }
  catch (IOperandFactory::Exception & e) {
    error(e.what(), token.line, token.column);
  }

  delete v1;
  delete v2;
}

void Machine::mod(const Token & token) {
  if (mStack.size() < 2) {
    error("'mod' on stack with less than two operands", token.line, token.column);
  }

  const IOperand *v1 = mStack.front();
  mStack.pop_front();
  const IOperand *v2 = mStack.front();
  mStack.pop_front();

  try {
    mStack.push_front(*v1 % *v2);
  }
  catch (IOperandFactory::Exception & e) {
    error(e.what(), token.line, token.column);
  }

  delete v1;
  delete v2;
}

void Machine::push(const Token & token, eOperandType type, const std::string & value) {
  IOperandFactory factory = IOperandFactory::getInstance();
  const IOperand * operand;

  try {
    operand = factory.createOperand(type, value);
  }
  catch (IOperandFactory::Exception & e) {
    error(e.what(), token.line, token.column);
  }

  mStack.push_front(operand);
}

void Machine::pop(const Token & token) {
  if (mStack.size() < 1) {
    error("'pop' on empty stack", token.line, token.column);
  }

  delete mStack.front();
  mStack.pop_front();
}

void Machine::print(const Token & token) {
  if (mStack.size() < 1) {
    error("'print' on empty stack", token.line, token.column);
  }

  try {
    const IOperand * iOperand = mStack.front();
    const Operand<int8_t> * operand = dynamic_cast<const Operand<int8_t> *>(iOperand);

    std::cout << operand->toString();
  }
  catch (const std::bad_cast & e) {
    error("value at the top of the stack not a 8bit integer", token.line, token.column);
  }
}

void Machine::dump(const Token & token) {
  for (auto i = mStack.rbegin(); i != mStack.rend(); ++i) {
    std::cout << (*i)->toString() << std::endl;
  }
}

void Machine::assert(const Token & token, eOperandType type, const std::string & value) {
  if (mStack.size() < 1) {
    error("'assert' on empty stacl", token.line, token.column);
  }

  const IOperand * front = mStack.front();
  std::string frontStackValue = front->toString();
  if (frontStackValue.compare(value) != 0) {
    error("assert fail", token.line, token.column);
  }
}

void Machine::exit(const Token & token) {
  std::exit(0);
}

void Machine::error(const std::string & message, size_t line, size_t column) const {
  std::string err = message;

  if (mFormatter.isActive()) {
    err = mFormatter.format(message, line, column);
  }
  else {
    err.append("\n");
  }

  throw Machine::Exception(err);
}

void Machine::setFormatter(const ErrorFormatter & formatter) {
  mFormatter = formatter;
}
