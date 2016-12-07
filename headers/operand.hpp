#pragma once

#include <string>
#include <sstream>
#include <cmath>
#include <algorithm>

#include "eoperand_type.hpp"
#include "ioperand.hpp"
#include "ioperand_factory.hpp"

template<typename T>
class Operand : public IOperand {

  public:
    Operand( T & value ) : value(value) {
      std::stringstream strStream;
      strStream << value;
      strStream >> this->strValue;
    }

    virtual int getPrecision( void ) const {
      return static_cast<int>(this->getType());
    }

    virtual eOperandType getType( void ) const;

    virtual const IOperand * operator+( const IOperand & rhs ) const {
      eOperandType returnType = static_cast<eOperandType>(std::max<int>(rhs.getPrecision(), this->getPrecision()));

      std::string resultStr;
      std::stringstream strStream;
      if (returnType < eOperandType::kFloat) {
        long value = static_cast<long>(this->value) + std::stol(rhs.toString());
        strStream << value;
      }
      else {
        double value = static_cast<double>(this->value) + std::stod(rhs.toString());
        strStream << value;
      }
      strStream >> resultStr;

      IOperandFactory factory = IOperandFactory::getInstance();
      return factory.createOperand(returnType, resultStr);
    }

    virtual const IOperand * operator-( const IOperand & rhs ) const {
      eOperandType returnType = static_cast<eOperandType>(std::max<int>(rhs.getPrecision(), this->getPrecision()));

      std::string resultStr;
      std::stringstream strStream;
      if (returnType < eOperandType::kFloat) {
        long value = static_cast<long>(this->value) - std::stol(rhs.toString());
        strStream << value;
      }
      else {
        double value = static_cast<double>(this->value) - std::stod(rhs.toString());
        strStream << value;
      }
      strStream >> resultStr;

      IOperandFactory factory = IOperandFactory::getInstance();
      return factory.createOperand(returnType, resultStr);
    }

    virtual const IOperand * operator*( const IOperand & rhs ) const {
      eOperandType returnType = static_cast<eOperandType>(std::max<int>(rhs.getPrecision(), this->getPrecision()));

      std::string resultStr;
      std::stringstream strStream;
      if (returnType < eOperandType::kFloat) {
        long value = static_cast<long>(this->value) * std::stol(rhs.toString());
        strStream << value;
      }
      else {
        double value = static_cast<double>(this->value) * std::stod(rhs.toString());
        strStream << value;
      }
      strStream >> resultStr;

      IOperandFactory factory = IOperandFactory::getInstance();
      return factory.createOperand(returnType, resultStr);
    }

    virtual const IOperand * operator/( const IOperand & rhs ) const {
      eOperandType returnType = static_cast<eOperandType>(std::max<int>(rhs.getPrecision(), this->getPrecision()));

      std::string resultStr;
      std::stringstream strStream;
      if (returnType < eOperandType::kFloat) {
        long value = static_cast<long>(this->value) / std::stol(rhs.toString());
        strStream << value;
      }
      else {
        double value = static_cast<double>(this->value) / std::stod(rhs.toString());
        strStream << value;
      }
      strStream >> resultStr;

      IOperandFactory factory = IOperandFactory::getInstance();
      return factory.createOperand(returnType, resultStr);
    }

    virtual const IOperand * operator%( const IOperand & rhs ) const {
      eOperandType returnType = static_cast<eOperandType>(std::max<int>(rhs.getPrecision(), this->getPrecision()));

      std::string resultStr;
      std::stringstream strStream;
      if (returnType < eOperandType::kFloat) {
        long value = static_cast<long>(this->value) % std::stol(rhs.toString());
        strStream << value;
      }
      else {
        double value = std::fmod(static_cast<double>(this->value),std::stod(rhs.toString()));
        strStream << value;
      }
      strStream >> resultStr;

      IOperandFactory factory = IOperandFactory::getInstance();
      return factory.createOperand(returnType, resultStr);
    }

    virtual const std::string & toString( void ) const {
      return this->strValue;;
    }

    virtual ~Operand( void ) {}

  private:
    T value;
    std::string strValue;
};
