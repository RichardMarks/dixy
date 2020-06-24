//
// Created by Richard Marks on 6/24/20.
//

#ifndef DIXY_REFERENCE_RUNTIME_H
#define DIXY_REFERENCE_RUNTIME_H

#include "dixy-cli.h"

#include "dixy-assembler.h"

namespace cli::rt {
  class ReferenceRuntime : public Runtime {
  public:
    ReferenceRuntime(std::string bin, std::vector<std::string> args) : Runtime(std::move(bin), std::move(args)) {
      std::cout
        << "DIXY Syntax Reference\n"
        << COPYRIGHT_AND_URL
        << std::endl;
      dixy::Assembler assembler;
      assembler.listSyntax();
    }
  };
}

#endif // DIXY_REFERENCE_RUNTIME_H
