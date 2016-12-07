#include <iostream>
#include <fstream>
#include <string>

#include "ioperand_factory.hpp"
#include "ioperand.hpp"
#include "lexer.hpp"
#include "error_formatter.hpp"

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
      if (std::cin.eof() == 1) {
        std::cin.clear();
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

  Lexer lexer(src);
  Token token{TokenType::kNewLine};
  ErrorFormatter formatter(src);
  size_t errorCount = 0;

  lexer.setFormatter(formatter);

  while (token.type != TokenType::kEndOfInput) {
    try {
      token = lexer.next();
    }
    catch (Lexer::Exception & e) {
      std::cerr << e.what();
      ++errorCount;
    }
  }

  if (errorCount > 0) {
    std::cerr << "Total lexer errors: " << errorCount << std::endl;
    return -1;
  }

  return 0;
}
