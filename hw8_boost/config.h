#pragma once

#include <vector>
#include <iostream>

#include <boost/program_options.hpp>

enum class HashAlgorithm
{
    MD5,
    CRC32
};

class Config
{
public:
    static Config& instance()
    {
        static Config instance;
        return instance;
    }

    void setIncludedFolderPaths(const std::vector<std::string>& paths);
    const std::vector<std::string>& getIncludedFolderPaths();

    void setExcludedFolderPaths(const std::vector<std::string>& paths);
    const std::vector<std::string>& getExcludedFolderPaths();

    void setMinFileSize(size_t size);
    size_t getMinFileSize();

    void setBlockSize(size_t size);
    size_t getBlockSize();

    void setMask(const std::string& mask);
    const std::string& getMask();

    void setScanningLevel(uint8_t level);
    uint8_t getScanningLevel();

    void setHashAlgorithm(HashAlgorithm algorithm);
    HashAlgorithm getHashAlgorithm();

private:
    Config();
    Config(const Config& rhs) = delete;
    Config& operator=(const Config& rhs) = delete;
    Config(Config&& rhs) = delete;
    Config& operator=(Config&& rhs) = delete;

    std::vector<std::string> m_includedFolderPaths;
    std::vector<std::string> m_excludedFolderPaths;
    size_t m_minFileSize;
    size_t m_blockSize;
    std::string m_mask;
    uint8_t m_scanningLevel;
    HashAlgorithm m_hashAlgorithm;
};

class CommandLineArgsParser
{
public:
    CommandLineArgsParser(int argc, char* argv[]);

private:
    boost::program_options::options_description m_desc;
};