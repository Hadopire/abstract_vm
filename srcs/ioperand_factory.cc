#include <cstdint>
#include <iostream>

#include "operand.h"
#include "ioperand_factory.h"

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
    /* TODO check overflow */
    std::cerr << e.what() << std::endl;
  }

  if (lValue < INT8_MIN || lValue > INT8_MAX) {
    /* TODO check overflow */
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
    /* TODO check overflow */
    std::cerr << e.what() << std::endl;
  }

  if (lValue < INT16_MIN || lValue > INT16_MAX) {
    /* TODO check overflow */
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
    /* TODO check overflow */
    std::cerr << e.what() << std::endl;
  }

  if (lValue < INT32_MIN || lValue > INT32_MAX) {
    /* TODO check overflow */
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
    /* TODO check overflow */
    std::cerr << e.what() << std::endl;
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
    /* TODO check overflow */
    std::cerr << e.what() << std::endl;
  }

  double opValue = static_cast<double>(dValue);
  return (new Operand<double>(opValue));
}
