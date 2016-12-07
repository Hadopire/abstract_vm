#include <iostream>
#include <fstream>
#include <string>

#include "ioperand_factory.h"
#include "ioperand.h"
#include "lexer.h"
#include "error_formatter.h"

int main(int ac, char **av) {

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
      return -1;
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

  std::cout << src << std::endl;

  Lexer lexer(src);
  ErrorFormatter formatter(src);

  lexer.setFormatter(formatter);

  return 0;
}
