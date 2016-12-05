#include <cstdint>
#include "operand.h"

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
