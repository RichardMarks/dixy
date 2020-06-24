//
// Created by Richard Marks on 6/24/20.
//

#ifndef DIXY_ASSEMBLER_H
#define DIXY_ASSEMBLER_H

#include <iostream>
#include <stdexcept>
#include <functional>
#include <vector>
#include <string>
#include <fstream>

#include "utils.h"

namespace dixy {
  class Assembler {
    bool shouldExplain{false};
    std::vector<unsigned char> bytes;
    std::vector<const char *> mnemonics{
      "IDP", "IVD", "OUT", "MVC", "MXD", "CDX", "CDY", "AXD",
      "SXD", "CYD", "CXD", "XDY", "MV3", "INP", "DVD", "DDP"
    };
    std::vector<const char *> descriptions{
      "[0x00] (IDP) - Increment data pointer",
      "[0x01] (IVD) - Increment value of byte at data pointer",
      "[0x02] (OUT) - Output byte at data pointer to standard output device",
      "[0x03] (MVC) - If byte at data pointer is zero, move instruction pointer to after next 'C' instruction",
      "[0x04] (MXD) - Multiply value of X storage by byte at data pointer, storing product in Y storage",
      "[0x05] (CDX) - Copy value of byte at data pointer into X storage",
      "[0x06] (CDY) - Copy value of byte at data pointer into Y storage",
      "[0x07] (AXD) - Add value of X storage and byte at data pointer, storing sum in Y storage",
      "[0x08] (SXD) - Subtract value of X from byte at data pointer, storing difference in Y storage",
      "[0x09] (CYD) - Copy value of Y storage into byte at data pointer",
      "[0x0A] (CXD) - Copy value of X storage into byte at data pointer",
      "[0x0B] (XDY) - Divide value of X storage by byte at data pointer, storing quotient in Y storage",
      "[0x0C] (MV3) - If byte at data pointer is not zero, move instruction pointer to after last '3' instruction",
      "[0x0D] (INP) - Accept byte of input from standard input device, storing value at data pointer",
      "[0x0E] (DVD) - Decrement value of byte at data pointer",
      "[0x0F] (DDP) - Decrement data pointer"
    };

    int getMnemonicIndex(const std::string &mnemonic);
  public:
    std::ostream& outputProgram(std::ostream& outStream, const std::string &outputFilename, const std::vector<unsigned char> &assembledBytes) const;
    void createFile(const std::string &outputFilename, const std::vector<unsigned char> &assembledBytes) const;
    void listSyntax();
    void disassemble(const std::string &sourceFile, const std::string &outputFile);
    void assemble(const std::vector<std::string>& sourceFiles, const std::string& outputFile);
    void assemble(const char *source, const std::function<void(std::vector<unsigned char>)> &onComplete);
    void explain();
  };
}

#endif // DIXY_ASSEMBLER_H
