//
// Created by Richard Marks on 6/24/20.
//

#include "constants.h"
#include "dixy-assembler.h"

int dixy::Assembler::getMnemonicIndex(const std::string &mnemonic) {
  int count = mnemonics.size();

  for (int i = 0; i < count; i++) {
    if (mnemonics[i] == mnemonic) {
      return i;
    }
  }

  return -1;
}

std::ostream &dixy::Assembler::outputProgram(std::ostream &outStream, const std::string &outputFilename, const std::vector<unsigned char> &assembledBytes) const {
  outStream
    << "; Assembled by DIXY Assembler v" << ASSEMBLER_VERSION << "\n; "
    << outputFilename
    << std::endl;

  int n = 0;
  if (shouldExplain) {
    std::cout << "Encoding " << assembledBytes.size() << " Bytes" << std::endl;
  }
  for (auto byte : assembledBytes) {
    if (byte >= 0x0 && byte <= 0x9) {
//          if (shouldExplain) {
//            std::cout << (int) byte << " -> " << (char) (0x30 + byte) << std::endl;
//          }
      outStream << (char) (0x30 + byte);
    } else if (byte >= 0xa && byte <= 0xf) {
//          if (shouldExplain) {
//            std::cout << (int) byte << " -> " << (char) (0x61 + (byte - 0xa)) << std::endl;
//          }
      outStream << (char) (0x61 + (byte - 0xa));
    } else {
      outStream << "; INVALID BYTE VALUE: " << (int) byte << std::endl;
    }
    n++;
    if (n % 32 == 0) {
      outStream << std::endl;
    }
  }

  auto m = (assembledBytes.size() + 32 - 1);
  m -= (m % 32);
  auto padding = m - assembledBytes.size();
  if (padding) {
    for (int i = 0; i < padding; i++) {
      outStream << "6";
      n++;
      if (n % 32 == 0) {
        outStream << std::endl;
      }
    }
  }

  outStream << std::endl;

  return outStream;
}

void dixy::Assembler::createFile(const std::string &outputFilename, const std::vector<unsigned char> &assembledBytes) const {
  std::ofstream outStream(outputFilename);
  outputProgram(outStream, outputFilename, assembledBytes);
  outStream.close();
}

void dixy::Assembler::listSyntax() {
  for (auto& desc : descriptions) {
    std::cout << desc << std::endl;
  }
}

void dixy::Assembler::disassemble(const std::string &sourceFile, const std::string &outputFile) {
  std::cout << "*** Disassembling " << sourceFile << std::endl;

  std::ofstream outStream(outputFile);

  outStream
    << "; Disassembled by DIXY Disassembler v" << DISASSEMBLER_VERSION << "\n; Source: "
    << sourceFile << "\n; "
    << outputFile
    << std::endl;

  std::vector<unsigned char> compressedBytes = utils::FSUtils::sourceBytes(sourceFile);
  auto compressedSize = compressedBytes.size();
  auto headerSize = 4;

  for (int i = headerSize; i < compressedSize; i++) {
    auto byte = compressedBytes[i];
    if (byte == 0xff) {
      if (i + 2 < compressedSize) {
        auto count = compressedBytes[i + 1];
        auto fill = compressedBytes[i + 2];
        if (fill > 0xf) {
          throw std::runtime_error("Program is Corrupt - Disassembly Failed.");
        }
        std::string desc = descriptions[fill];
        desc = desc.substr(15);
        outStream << mnemonics[fill] << " " << (int)count << " ; " << desc << " (x" << (int)count << ")" << std::endl;
        i += 2;
      } else {
        throw std::runtime_error("Program is Corrupt - Disassembly Failed.");
      }
    } else {
      if (byte > 0xf) {
        throw std::runtime_error("Program is Corrupt - Disassembly Failed.");
      }
      std::string desc = descriptions[byte];
      desc = desc.substr(15);
      outStream << mnemonics[byte] << " ; " << desc << std::endl;
    }
  }

  outStream.close();
}

void dixy::Assembler::assemble(const std::vector<std::string> &sourceFiles, const std::string &outputFile) {
  std::vector<unsigned char> totalAssembledBytes;
  int n = 0;
  int k = sourceFiles.size();
  for (auto& sourceFile : sourceFiles) {
    if (shouldExplain) {
      std::cout << "*** Assembling [" << (n + 1) << "/" << k << "] " << sourceFile << std::endl;
    }
    std::string sourceFileContent = utils::FSUtils::source(sourceFile);

    assemble(sourceFileContent.c_str(), [&](const std::vector<unsigned char>& assembledBytes){
      for (unsigned char byte : assembledBytes) {
        totalAssembledBytes.push_back(byte);
      }
      n++;
    });
  }

  if (shouldExplain) {
    std::cout << "Writing to " << outputFile << std::endl;
  }

  Assembler::createFile(outputFile, totalAssembledBytes);
}

void dixy::Assembler::assemble(const char *source, const std::function<void(std::vector<unsigned char>)> &onComplete) {
  bytes.clear();

  std::vector<std::string> lines = utils::TextUtils::split(source);

  int currentLine = 0;

  for (auto &line : lines) {
    currentLine++;
    if (line.empty()) {
      if (shouldExplain) {
        std::cout << "*** Skipped Empty Line" << std::endl;
      }
      continue;
    }
    if (line.at(0) == ';') {
      if (shouldExplain) {
        std::cout << "*** Skipped Comment Line" << std::endl;
      }
      continue;
    }
    auto commentStart = line.find(';');
    auto lineHasComment = commentStart != std::string::npos;
    auto code = lineHasComment ? line.substr(0, commentStart) : line;
    code = utils::TextUtils::trimmed(code);
//        std::cout << "Assembling \"" << code << "\"" << std::endl;
    if (code.size() != 3) {
//          std::cout << "Expanding Instruction Sequence" << std::endl;
      auto countStr = code.substr(3, code.size());
      auto count = atol(countStr.c_str());
      auto mnemonic = code.substr(0, 3);
      auto index = getMnemonicIndex(mnemonic);

      if (index < 0) {
        std::cerr
          << "Line #" << currentLine << "\n"
          << code << "\n"
          << "mnemonic = \"" << mnemonic << "\"\n"
          << "count = " << count << "\n"
          << "index = " << index << "\n"
          << std::endl;
        throw std::runtime_error("Invalid Instruction Error");
      }
//          std::cout << "Expanding x" << count << " " << mnemonic << std::endl;
      if (shouldExplain) {
        std::cout << "(" << count << " times) " << descriptions.at(index) << std::endl;
      }
      for (int i = 0; i < count; i++) {
        bytes.push_back(index);
      }
    } else {
//          std::cout << "Single Instruction: " << code << std::endl;
      auto index = getMnemonicIndex(code);
      if (index < 0) {
        std::cerr
          << "Line #" << currentLine << "\n"
          << code << "\n"
          << "mnemonic = \"" << code << "\"\n"
          << "index = " << index << "\n"
          << std::endl;
        throw std::runtime_error("Invalid Instruction Error");
      }
      if (shouldExplain) {
        std::cout << descriptions.at(index) << std::endl;
      }
      bytes.push_back(index);
    }
  }
//
//      for (auto byte : bytes) {
//        std::cout << (int)byte << " ";
//      }
//      std::cout << std::endl;
  onComplete(bytes);
}

void dixy::Assembler::explain() {
  shouldExplain = true;
}
