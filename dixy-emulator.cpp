//
// Created by Richard Marks on 6/24/20.
//

#include <iostream>
#include <stdexcept>
#include <functional>
#include <iomanip>

#include "constants.h"
#include "dixy-emulator.h"

#include "utils.h"

unsigned char dixy::Emulator::getMemory() {
  return memory[data];
}

void dixy::Emulator::setMemory(unsigned char value) {
  memory[data] = value & 0xffu;
}

void dixy::Emulator::reset() {
  data = 0;
  instruction = 0;
  registerX = 0;
  registerY = 0;
  memset(memory, 0, MEMORY_SIZE);
}

std::vector<unsigned char> dixy::Emulator::decompress(const std::vector<unsigned char> &compressedBytes) {
  std::vector<unsigned char> decompressedBytes;

  auto compressedSize = compressedBytes.size();

  for (int i = 4; i < compressedSize; i++) {
    auto byte = compressedBytes[i];
    if (byte == 0xff) {
//          std::cout << "Got compression flag" << std::endl;
      if (i + 2 < compressedSize) {
        auto count = compressedBytes[i + 1];
        auto fill = compressedBytes[i + 2];
//            std::cout << "Expanding " << (int)count << " of " << (int)fill << std::endl;
        for (int x = 0; x < count; x++) {
          decompressedBytes.push_back(fill);
        }
        i += 2;
      } else {
        throw std::runtime_error("Program is Corrupt - Decompression Failed.");
      }
    } else {
      decompressedBytes.push_back(byte);
    }
  }

  std::cout
    << "Decompression Complete: "
    << compressedSize - 4
    << " bytes expanded to "
    << decompressedBytes.size()
    << " bytes."
    << std::endl;

  // dump program before run?
//      int n = 0;
//      for (auto byte : decompressedBytes) {
//        std::cout << std::setbase(16) << (int)byte;
//        n++;
//        if (n % 32 == 0) {
//          std::cout << std::endl;
//        }
//      }
//      std::cout << std::endl;

  return decompressedBytes;
}

bool dixy::Emulator::identifyProgram(const std::vector<unsigned char> &compressedBytes) {
  if (compressedBytes.size() >= 4) {
    auto signatureOK = (compressedBytes[0] == 0x44u &&
                        compressedBytes[1] == 0x49u &&
                        compressedBytes[2] == 0x58u &&
                        compressedBytes[3] == 0x59u);
    std::cout << "Checking Program File Signature " << (signatureOK ? "OK" : "ERROR") << std::endl;
    return signatureOK;
  }
  throw std::runtime_error("Program File Size Failure");
}

void dixy::Emulator::runProgram(const std::vector<unsigned char> &decompressedBytes) {
  auto& code = decompressedBytes;
  auto codeLength = decompressedBytes.size();

  std::function<void()> ops[] = {
    // 0x00 - increment data pointer
    [&](){
      data++;
      data = data >= MEMORY_SIZE ? MEMORY_SIZE - 1 : data;
    },
    // 0x01 - increment value of byte at data pointer
    [&](){
      setMemory(getMemory() + 1);
    },
    // 0x02 - output byte at data pointer to standard output device
    [&](){
      std::cout << static_cast<char>(getMemory());
    },
    // 0x03 - if byte at data pointer is zero, move instruction pointer to after next 'C' instruction
    [&](){
      if (getMemory() != 0x00u) {
        return;
      }
      while (code[instruction] != 0x0cu) {
        if (instruction >= codeLength) {
          std::cerr << "Code Instruction Overflow Error!" << std::endl;
          break;
        } else {
          instruction++;
        }
      }
    },
    // 0x04 - multiply value of X storage by byte at data pointer, storing product in Y storage
    [&](){
      registerY = (getMemory() * registerX);
      registerY &= 0xffu;
    },
    // 0x05 - copy value of byte at data pointer into X storage
    [&](){
      registerX = getMemory();
    },
    // 0x06 - copy value of byte at data pointer into Y storage
    [&](){
      registerY = getMemory();
    },
    // 0x07 - add value of X storage and byte at data pointer, storing sum in Y storage
    [&](){
      registerY = (getMemory() + registerX);
      registerY &= 0xffu;
    },
    // 0x08 - subtract value of X from byte at data pointer, storing difference in Y storage
    [&](){
      registerY = (getMemory() - registerX);
      registerY &= 0xffu;
    },
    // 0x09 - copy value of Y storage into byte at data pointer
    [&](){
      setMemory(registerY);
    },
    // 0x0A - copy value of X storage into byte at data pointer
    [&](){
      setMemory(registerX);
    },
    // 0x0B - divide value of X storage by byte at data pointer, storing quotient in Y storage
    [&](){
      registerY = (getMemory() / registerX);
      registerY &= 0xffu;
    },
    // 0x0C - if byte at data pointer is not zero, move instruction pointer to after last '3' instruction
    [&](){
      if (getMemory() == 0x00u) {
        return;
      }
      while (code[instruction] != 0x03u) {
        if (instruction < 0) {
          std::cerr << "Code Instruction Underflow Error!" << std::endl;
          break;
        } else {
          instruction--;
        }
      }
    },
    // 0x0D - accept byte of input from standard input device, storing value at data pointer
    [&](){
      std::string input;
      std::getline(std::cin, input);
      unsigned char value = utils::TextUtils::parseByteValue(input);
      setMemory(value);
    },
    // 0x0E - decrement value of byte at data pointer
    [&](){
      setMemory(getMemory() - 1);
    },
    // 0x0F - decrement data pointer
    [&](){
      data--;
      data = data < 0 ? 0 : data;
    }
  };

  std::cout << "-- Running Program" << std::endl;
  reset();
  while (instruction < codeLength) {
//        try {
    unsigned char x = code[instruction];
    auto op = ops[x];
    op();
//        } catch (std::exception& ex) {
//          std::cerr << "Well this was unexpected.\n" << ex.what() << std::endl;
//          break;
//        }
    instruction++;
  }
  std::cout << "-- Program End" << std::endl;
}

dixy::Emulator::Emulator() {
  reset();
}

void dixy::Emulator::run(const std::string &programFilename) {
  std::vector<unsigned char> programCompressedBytes = utils::FSUtils::sourceBytes(programFilename);
  if (Emulator::identifyProgram(programCompressedBytes)) {
    std::vector<unsigned char> programDecompressedBytes = Emulator::decompress(programCompressedBytes);
    runProgram(programDecompressedBytes);
    // TODO - process flag to dump the emulator state after run?
    dumpState();
  }
}

void dixy::Emulator::dumpState() {
  auto d = data;
  auto i = instruction;
  auto x = registerX;
  auto y = registerY;
  std::cout
    << "-- Registers:\nD ["
    << std::setw(2)
    << std::setfill('0')
    << std::setbase(16)
    << (int)d
    << " (" << std::setbase(10) << (int)d << ")]\nI ["
    << std::setw(4)
    << std::setfill('0')
    << std::setbase(16)
    << (int)i
    << " (" << std::setbase(10) << (int)i << ")]\nX ["
    << std::setw(2)
    << std::setfill('0')
    << std::setbase(16)
    << (int)x
    << " (" << std::setbase(10) << (int)x << ")]\nY ["
    << std::setw(2)
    << std::setfill('0')
    << std::setbase(16)
    << (int)y
    << " (" << std::setbase(10) << (int)y << ")]\n"
    << "-- Memory: " << std::setbase(10) << MEMORY_SIZE << " bytes\n";

  for (int offset = 0; offset < MEMORY_SIZE; offset++) {
    std::cout
      << std::setw(4)
      << std::setfill('0')
      << std::setbase(16)
      << offset
      << ": ";
    for (int i = 0; i < 16; i++) {
      std::cout
        << std::setw(2)
        << std::setfill('0')
        << std::setbase(16)
        << (int)memory[offset + i]
        << " ";
    }
    std::cout << " |";
    for (int i = 0; i < 16; i++) {
      char chr = (char)memory[offset + i];
      if (chr < 32 || chr > 127) {
        chr = '.';
      }
      std::cout << chr;
    }
    std::cout << "|" << std::endl;
    offset += 16;
  }
  std::cout << std::endl;
}
