#include <iostream>
#include <fstream>
#include <vector>
#include <cstdio>
#include <string>
#include <cstdlib>

size_t encSuccess = 0, encFailure = 0;
size_t decSuccess = 0, decFailure = 0;

size_t fileSizeG(std::fstream& file)
{
    file.seekg(0, std::ios::end);
    size_t fs = (size_t)file.tellg();
    file.seekg(0, std::ios::beg);
    return fs;
}

std::vector<uint8_t> MagicHeader = {
  0x79, 0x65, 0x65, 0x6C, 0x69, 0x6F, 0x6E, 0x2D,
  0x6B, 0x75, 0x77, 0x6F, 0x2D, 0x74, 0x6D, 0x65 };

const char PreDefinedKey[] = "MoOtOiTvINGwd2E6n0E1i7L5t2IoOoNk";

void trimKey(std::string &key) {
  for (; key.length() < 32 ;) {
    key = key + key;
  }
  key.resize(32);
  return;
}

void createMaskFromKey(std::vector<uint8_t> keyBytes, std::vector<uint8_t> &key) {
    std::string keyStr;
    keyStr=std::to_string(*((uint64_t*)keyBytes.data()));
    trimKey(keyStr);
    key.resize(32);
    for (size_t i = 0; i < 32; i++) {
        key[i] = ((uint8_t)PreDefinedKey[i]) ^ ((uint8_t)keyStr[i]);
    }
    return;
}

void encrypt(std::string fileName, std::string pswFn = "") {
    std::fstream infile(fileName, std::ios::in | std::ios::binary);
    if (!infile.is_open()) {
        std::cerr << "Cannot read file " << fileName << "." << std::endl;
        encFailure++;
        return;
    }
    srand(time(0));
    std::vector<uint8_t> fileKey(8);
    *((uint64_t*)fileKey.data()) = (uint64_t)(rand() << 48 + rand() << 32 + rand() << 16 + rand());
    std::vector<uint8_t> mask;
    createMaskFromKey(fileKey, mask);
    std::vector<uint8_t> fileData;
    fileData.resize(fileSizeG(infile));
    infile.read((char*)fileData.data(), fileData.size());
    for (size_t i = 0; i < fileData.size(); i++) {
        fileData[i] ^= mask[i % 0x20];
    }
    std::string outFn = fileName.substr(0, fileName.find_last_of(".")) + ".kwm" + fileName.substr(1 + fileName.find_last_of("."));
    std::cout << "Output:\n" << outFn << std::endl;
    std::fstream outfile;
    if (pswFn != "")
    {
        outfile.open(pswFn, std::ios::out | std::ios::binary);
    }
    else
    {
        outfile.open(outFn, std::ios::out | std::ios::binary);
    }
    if (!outfile.is_open()) {
        std::cerr << "Cannot write file " << outFn << "." << std::endl;
        encFailure++;
        return;
    }
    outfile.write((char*)MagicHeader.data(), 0x10);
    for (size_t i = 0; i < 8; i++)
    {
        outfile << (uint8_t)rand();
    }
    outfile.write((char*)fileKey.data(), 8);
    for (size_t i = 0; i < 0x3e0; i++)
    {
        outfile << (uint8_t)rand();
    }
    if (pswFn != "")
    {
        outfile.close();
        outfile.open(outFn, std::ios::out | std::ios::binary);
    }
    outfile.write((char*)fileData.data(), fileData.size());
    encSuccess++;
}

std::string extViaExt(std::string fileName)
{
    if (fileName.find(".kwm") < fileName.size())
    {
        return "." + fileName.substr(4 + fileName.find_last_of("."));
    }
    else
    {
        return ".bin";
    }
}

void decrypt(std::string fileName, std::string pswFn = "") {
    std::fstream infile;
    if (pswFn != "")
    {
        infile.open(pswFn, std::ios::in | std::ios::binary);
    }
    else
    {
        infile.open(fileName, std::ios::in | std::ios::binary);
    }

    if (!infile.is_open()) {
        std::cerr << "Cannot read file " << fileName << "." << std::endl;
        decFailure++;
        return;
    }

    std::vector<uint8_t> fileKey;
    fileKey.resize(8);
    infile.seekg(0x18, std::ios::beg);
    infile.read((char*)fileKey.data(), 8);
    std::vector<uint8_t> mask;
    createMaskFromKey(fileKey, mask);
    std::vector<uint8_t> fileData;
    if (pswFn != "")
    {
        infile.close();
        infile.open(fileName, std::ios::in | std::ios::binary);
        if (!infile.is_open()) {
            std::cerr << "Cannot read file " << fileName << "." << std::endl;
            decFailure++;
            return;
        }
        fileData.resize(fileSizeG(infile));
    }
    else
    {
        fileData.resize(fileSizeG(infile) - 0x400);
        infile.seekg(0x400, std::ios::beg);
    }
    infile.read((char*)fileData.data(), fileData.size());
    for (size_t cur = 0; cur < fileData.size(); ++cur) fileData[cur] ^= mask[cur % 0x20];
    std::string ext = extViaExt(fileName);
    std::string outFn = fileName.substr(0, fileName.find_last_of(".")) + ext;
    std::cout << "Output:\n" << outFn << std::endl;
    std::fstream outfile(outFn, std::ios::out | std::ios::binary);
    if (!outfile.is_open()) {
        std::cerr << "Cannot write file " << outFn << "." << std::endl;
        decFailure++;
        return;
    }
    outfile.write((char*)fileData.data(), fileData.size());
    decSuccess++;
}
