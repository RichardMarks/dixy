//
// Created by Richard Marks on 6/24/20.
//

#include "constants.h"
#include "dixy-linker.h"

std::vector<unsigned char> dixy::Linker::compress(const std::vector<unsigned char> &uncompressedBytes) {
  std::vector<unsigned char> compressed;

  auto n = uncompressedBytes.size();
  for (int i = 0; i < n; i++) {
    int count = 1;
    while (i < n - 1 && uncompressedBytes[i] == uncompressedBytes[i + 1]) {
      count++;
      i++;
    }
    if (count > 1) {
      compressed.push_back(0xff);
      compressed.push_back(count);
      compressed.push_back(uncompressedBytes[i]);
    } else {
      compressed.push_back(uncompressedBytes[i]);
    }
  }

  return compressed;
}

void dixy::Linker::createFile(const std::string &outputFilename, const std::vector<unsigned char> &assembledBytes) const {
  std::ofstream outStream;
  outStream.open(outputFilename, std::ios::binary | std::ios::out);
  if (outStream.fail()) {
    throw std::runtime_error("Unable to Write the Output File");
  }

  unsigned char fourcc[] = { 0x44u, 0x49u, 0x58u, 0x59u };
  if (shouldExplain) {
    std::cout << "Writing File Signature..." << std::endl;
  }
  outStream.write((char*)fourcc, 4 * sizeof(unsigned char));
  if (shouldExplain) {
    std::cout << "Compressing Program..." << std::endl;
  }
  std::vector<unsigned char> compressedBytes = Linker::compress(assembledBytes);
  if (shouldExplain) {
    auto uncompressedSize = 4 + assembledBytes.size();
    auto compressedSize = 4 + compressedBytes.size();
    auto compressionRatio = (double)((double)uncompressedSize / (double)compressedSize);
    auto diskSpaceSavingsPercentage = 100.0 * ((double)(uncompressedSize - compressedSize) / (double)uncompressedSize);
    std::cout
      << "COMPRESSION STATS:\n"
      << "Uncompressed Size: " << uncompressedSize << " bytes\n"
      << "  Compressed Size: " << compressedSize << " bytes\n"
      << "Compression Ratio: " << compressionRatio << "\n"
      << "    Total Savings: " << diskSpaceSavingsPercentage << " %" << std::endl;
  }
  outStream.write((char*)&compressedBytes[0], compressedBytes.size());
  outStream.close();
  if (shouldExplain) {
    std::cout << "DONE." << std::endl;
  }
}

void dixy::Linker::explain() {
  shouldExplain = true;
}

void dixy::Linker::link(const std::vector<std::string> &sourceFiles, const std::string &outputFile) {
  std::vector<unsigned char> totalCodeBytes;
  int n = 0;
  int k = sourceFiles.size();
  for (auto& sourceFile : sourceFiles) {
    if (shouldExplain) {
      std::cout << "*** Linking [" << (n + 1) << "/" << k << "] " << sourceFile << std::endl;
    }
    std::string sourceFileContent = utils::FSUtils::source(sourceFile);

    link(sourceFileContent.c_str(), [&](const std::vector<unsigned char>& codeBytes){
      for (unsigned char byte : codeBytes) {
        totalCodeBytes.push_back(byte);
      }
      n++;
    });
  }

  if (shouldExplain) {
    std::cout << "Writing to " << outputFile << std::endl;
  }

  Linker::createFile(outputFile, totalCodeBytes);
}

void dixy::Linker::link(const char *source, const std::function<void(std::vector<unsigned char>)> &onComplete) {
  bytes.clear();

  std::vector<std::string> lines = utils::TextUtils::split(source);

  for (auto &line : lines) {
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

    for (auto &c : code) {
      if (c == '\n') {
        if (shouldExplain) {
          std::cout << "*** Skipping Newline" << std::endl;
        }
      }
      auto byte = utils::TextUtils::getLiteralCharValue(c);
      bytes.push_back(byte);
//          if (c >= 0x30 && c <= 0x39) {
//            // 0-9
//            bytes.push_back(c - 0x30);
//          } else if (c >= 0x61 && c <= 0x66) {
//            // A-F
//            bytes.push_back(0xa + (c - 0x61));
//          } else if (c == '\n') {
//
//          } else {
//            throw std::runtime_error("Invalid Character");
//          }
    }
  }
  onComplete(bytes);
}
