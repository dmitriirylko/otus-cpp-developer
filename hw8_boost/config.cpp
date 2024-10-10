#include "config.h"

Config::Config() :
    m_minFileSize{1},
    m_blockSize{10},
    m_scanningLevel{1},
    m_hashAlgorithm{HashAlgorithm::MD5}
{}

void Config::setIncludedFolderPaths(const std::vector<std::string>& paths)
{
    m_includedFolderPaths = paths;
}

const std::vector<std::string>& Config::getIncludedFolderPaths()
{
    return m_includedFolderPaths;
}

void Config::setExcludedFolderPaths(const std::vector<std::string>& paths)
{
    m_excludedFolderPaths = paths;
}

const std::vector<std::string>& Config::getExcludedFolderPaths()
{
    return m_excludedFolderPaths;
}

void Config::setMinFileSize(size_t size)
{
    m_minFileSize = size;
}

size_t Config::getMinFileSize()
{
    return m_minFileSize;
}

void Config::setBlockSize(size_t size)
{
    m_blockSize = size;
}

size_t Config::getBlockSize()
{
    return m_blockSize;
}

void Config::setMask(const std::string& mask)
{
    m_mask = mask;
}

const std::string& Config::getMask()
{
    return m_mask;
}

void Config::setScanningLevel(uint8_t level)
{
    m_scanningLevel = level;
}

uint8_t Config::getScanningLevel()
{
    return m_scanningLevel;
}

void Config::setHashAlgorithm(HashAlgorithm algorithm)
{
    m_hashAlgorithm = algorithm;
}

HashAlgorithm Config::getHashAlgorithm()
{
    return m_hashAlgorithm;
}

CommandLineArgsParser::CommandLineArgsParser(int argc, char* argv[]) :
    m_desc{"Options"}
{
    m_desc.add_options()
    ("include",
    boost::program_options::value<std::vector<std::string>>()->multitoken(),
    "Set folder paths to scan")
    ("exclude",
    boost::program_options::value<std::vector<std::string>>()->multitoken(),
    "Exclude folder paths from scanning")
    ("filesize",
    boost::program_options::value<size_t>(),
    "Minimum size of query file")
    ("blocksize",
    boost::program_options::value<size_t>(),
    "Block size of one file chunk on which file will be splitted")
    ("mask",
    boost::program_options::value<std::string>(),
    "Mask of filenames to scan")
    ("level",
    boost::program_options::value<uint8_t>(),
    "Scanning level: 0 - only current directory, 1 - current directory with all subfolders")
    ("hash",
    boost::program_options::value<std::string>(),
    "Hashing algorithm");
    

    std::cout << m_desc;
}