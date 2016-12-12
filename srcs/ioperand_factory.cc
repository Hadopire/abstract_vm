#include <cstdint>
#include <iostream>

#include "operand.hpp"
#include "ioperand_factory.hpp"

IOperandFactory::Exception::Exception() : out_of_range("IOperandFactory exception"), message("IOperandFactory exception") {}

IOperandFactory::Exception::Exception(const std::string & src) : out_of_range(src), message(src) {}

IOperandFactory::Exception::Exception(const IOperandFactory::Exception & exception) : out_of_range(exception.message), message(exception.message) {}

IOperandFactory::Exception & IOperandFactory::Exception::operator=(const IOperandFactory::Exception & rhs) {
  message = rhs.message;
  return *this;
}

const char * IOperandFactory::Exception::what() const throw() {
  return message.c_str();
}

IOperandFactory::Exception::~Exception() {}

IOperandFactory & IOperandFactory::getInstance() {
  static IOperandFactory factory;

  return factory;
}

const IOperand * IOperandFactory::createOperand(eOperandType type, std::string const & value) const {
  return createOp[static_cast<int>(type)](*this, value);
}

const IOperand * IOperandFactory::createInt8(std::string const & value) const {
  long lValue;

  try {
    lValue = std::stol(value);
  }
  catch(std::out_of_range & e) {
    throw IOperandFactory::Exception("Int8 overflow on value '" + value + "'");
  }

  if (lValue < INT8_MIN || lValue > INT8_MAX) {
    throw IOperandFactory::Exception("Int8 overflow on value '" + value + "'");
  }

  int8_t opValue = static_cast<int8_t>(lValue);
  return (new Operand<int8_t>(opValue));
}

const IOperand * IOperandFactory::createInt16(std::string const & value) const {
  long lValue;

  try {
    lValue = std::stol(value);
  }
  catch(std::out_of_range & e) {
    throw IOperandFactory::Exception("Int16 overflow on value '" + value + "'");
  }

  if (lValue < INT16_MIN || lValue > INT16_MAX) {
    throw IOperandFactory::Exception("Int16 overflow on value '" + value + "'");
  }

  int16_t opValue = static_cast<int16_t>(lValue);
  return (new Operand<int16_t>(opValue));
}

const IOperand * IOperandFactory::createInt32(std::string const & value) const {
  long lValue;

  try {
    lValue = std::stol(value);
  }
  catch(std::out_of_range & e) {
    throw IOperandFactory::Exception("Int32 overflow on value '" + value + "'");
  }

  if (lValue < INT32_MIN || lValue > INT32_MAX) {
    throw IOperandFactory::Exception("Int32 overflow on value '" + value + "'");
  }

  int32_t opValue = static_cast<int32_t>(lValue);
  return (new Operand<int32_t>(opValue));
}

const IOperand * IOperandFactory::createFloat(std::string const & value) const {
  float fValue;

  try {
    fValue = std::stof(value);
  }
  catch(std::out_of_range & e) {
    throw IOperandFactory::Exception("Float overflow on value '" + value + "'");
  }

  float opValue = static_cast<float>(fValue);
  return (new Operand<float>(opValue));
}

const IOperand * IOperandFactory::createDouble(std::string const & value) const {
  double dValue;

  try {
    dValue = std::stof(value);
  }
  catch(std::out_of_range & e) {
    throw IOperandFactory::Exception("Double overflow on value '" + value + "'");
  }

  double opValue = static_cast<double>(dValue);
  return (new Operand<double>(opValue));
}
