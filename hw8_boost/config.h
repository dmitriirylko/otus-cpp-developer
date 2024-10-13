#pragma once

#include <vector>
#include <iostream>
#include <stdexcept>

#include <boost/program_options.hpp>
#include <boost/algorithm/string.hpp>

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

    const std::vector<std::string>& getIncludedFolderPaths();

    const std::vector<std::string>& getExcludedFolderPaths();

    size_t getMinFileSize();

    size_t getBlockSize();

    const std::string& getMask();

    uint8_t getScanningLevel();

    HashAlgorithm getHashAlgorithm();

    void setData(int argc, char* argv[]);

private:
    Config();
    Config(const Config& rhs) = delete;
    Config& operator=(const Config& rhs) = delete;
    Config(Config&& rhs) = delete;
    Config& operator=(Config&& rhs) = delete;

    std::vector<std::string> m_includedFolderPaths;
    std::vector<std::string> m_excludedFolderPaths;
    
    /**
     * @brief Minimal size of file to compare in bytes.
     */
    size_t m_minFileSize = 1;

    /**
     * @brief Size of chunks on which file will be divided. Files will be
     *          compared by this chunks.
     */
    size_t m_blockSize = 32;
    std::string m_mask;
    
    /**
     * @brief Scanning level of directory:
     *          0 - only specified directory will be scanned;
     *          1 - nested directories will be scanned as well.
     */
    int m_scanningLevel = 1;
    HashAlgorithm m_hashAlgorithm;

    boost::program_options::options_description m_desc;
    boost::program_options::variables_map m_vm;
};