#pragma once

#include <string>
#include <functional>

#include "ioperand.h"
#include "eoperand_type.h"

class IOperandFactory {

  public:
    static IOperandFactory & getInstance( void );
    const IOperand * createOperand( eOperandType type, const std::string & value ) const;

  private:
    std::function<const IOperand *(const IOperandFactory &, const std::string &)> createOp[5] {
      &IOperandFactory::createInt8,
      &IOperandFactory::createInt16,
      &IOperandFactory::createInt32,
      &IOperandFactory::createFloat,
      &IOperandFactory::createDouble
    };

    IOperandFactory() {}

    const IOperand * createInt8( std::string const & value ) const;
    const IOperand * createInt16( std::string const & value ) const;
    const IOperand * createInt32( std::string const & value ) const;
    const IOperand * createFloat( std::string const & value ) const;
    const IOperand * createDouble( std::string const & value ) const;
};
