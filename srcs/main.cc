#include <iostream>
#include <fstream>
#include <string>
#include <list>

#include "ioperand_factory.hpp"
#include "ioperand.hpp"
#include "lexer.hpp"
#include "error_formatter.hpp"
#include "parser.hpp"
#include "machine.hpp"

const std::string readSrc(int ac, char **av) {
  std::string src;

  if (ac > 1) {
    try {
      std::ifstream in(av[1], std::ios::in);

      in.seekg(0, std::ios::end);
      src.resize(static_cast<size_t>(in.tellg()));
      in.seekg(0, std::ios::beg);

      in.read(&src[0], src.size());
      in.close();
    }
    catch (std::exception & e) {
      std::cerr << "failed to open " << av[1] << std::endl;
      exit(-1);
    }
  }
  else {
    bool finish = false;

    while (!finish) {
      std::string buffer;

      std::getline(std::cin, buffer);
      if (std::cin.eof() == 1 ) {
        std::cin.clear();
        std::cin.ignore();
      }
      else {
        buffer.append("\n");
      }

      if (buffer.compare(";;\n") == 0) {
        finish = true;
      }
      else {
        src.append(buffer);
      }
    }
  }
  return src;
}

int main(int ac, char **av) {
  const std::string src = readSrc(ac, av);
  ErrorFormatter formatter(src);

  size_t errorCount = 0;

  Lexer lexer(src);
  lexer.setFormatter(formatter);

  std::list<Token> tokens;
  Token token{TokenType::kNewLine, "", 0, 0};
  while (token.type != TokenType::kEndOfInput) {
    try {
      token = lexer.next();
      tokens.push_back(token);
    }
    catch (Lexer::Exception & e) {
      std::cerr << e.what();
      ++errorCount;
    }
  }

  if (errorCount > 0) {
    std::cerr << "Total lexer error: " << errorCount << std::endl;
    return -1;
  }

  Parser parser(tokens);
  parser.setFormatter(formatter);

  std::function<void (Machine &)> instr;
  std::list<std::function<void (Machine &)>> instructions;
  while (parser.isFinish() == false) {
    try {
      instr = parser.next();
      instructions.push_back(instr);
    }
    catch (Parser::Exception & e) {
      std::cerr << e.what();
      ++errorCount;
    }
  }

  if (errorCount > 0) {
    std::cerr << "Total parser error: " << errorCount << std::endl;
    return -1;
  }

  Machine machine;
  machine.setFormatter(formatter);
  for (auto & func : instructions) {
    try {
      func(machine);
    }
    catch (Machine::Exception & e) {
      std::cerr << e.what();
      return -1;
    }
  }

  return 0;
}
