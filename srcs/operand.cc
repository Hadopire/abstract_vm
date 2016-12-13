#include <cstdint>
#include "operand.hpp"

template<>
Operand<int8_t>::Operand(int8_t & value) : value(value) {
  std::stringstream strStream;
  strStream << static_cast<int>(value);
  strStream >> this->strValue;
}

template<>
eOperandType Operand<int8_t>::getType() const {
  return eOperandType::kInt8;
}

template<>
eOperandType Operand<int16_t>::getType() const {
  return eOperandType::kInt16;
}

template<>
eOperandType Operand<int32_t>::getType() const {
  return eOperandType::kInt32;
}

template<>
eOperandType Operand<float>::getType() const {
  return eOperandType::kFloat;
}

template<>
eOperandType Operand<double>::getType() const {
  return eOperandType::kDouble;
}
