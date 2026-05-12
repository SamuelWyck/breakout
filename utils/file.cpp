#include <stdexcept>
#include <string>
#include <utility>
#include <iostream>
#include <string_view>
#include <vector>
#include "./file.h"



File::File(std::string_view filePath) : m_filePath{filePath} {
};

File::File() : m_filePath{""} {
};


std::string File::readFile(){
    throwErrorIfNoFilePath();
    m_fileStream.open(m_filePath, std::ios::in);
    throwErrorIfOpenFailed();

    std::string contents{};
    std::string fileLine{};
    while (std::getline(m_fileStream, fileLine)) {
        fileLine += '\n';
        contents.append(std::move(fileLine));
    }

    m_fileStream.close();
    return contents;
};


std::vector<std::string> File::readLines() {
    throwErrorIfNoFilePath();
    m_fileStream.open(m_filePath, std::ios::in);
    throwErrorIfOpenFailed();

    std::vector<std::string> fileLines{};
    std::string fileLine{};
    while (std::getline(m_fileStream, fileLine)) {
        fileLine += '\n';
        fileLines.push_back(std::move(fileLine));
    }

    m_fileStream.close();
    return fileLines;
};


std::string File::readLine() {
    throwErrorIfNoFilePath();
    if (m_filePtrAtEOF) {
        return std::string{};
    }

    m_fileStream.open(m_filePath, std::ios::in);
    throwErrorIfOpenFailed();


    m_fileStream.seekg(m_filePtrOffset, std::ios::beg);

    std::string fileLine{};
    std::getline(m_fileStream, fileLine);
    fileLine += '\n';

    m_filePtrOffset = m_fileStream.tellg();
    m_filePtrAtEOF = m_fileStream.eof();
    m_fileStream.close();

    return fileLine;
};


void File::resetFilePtr() {
    m_filePtrAtEOF = false;
    m_filePtrOffset = 0;
};


File& File::writeToFile(std::string_view content) {
    throwErrorIfNoFilePath();
    m_fileStream.open(m_filePath, std::ios::out);
    throwErrorIfOpenFailed();

    m_fileStream << content;

    char lastChar {content[content.size() - 1]};
    if (lastChar != '\n') {
        m_fileStream << '\n';
    }

    m_fileStream.close();
    return *this;
};


File& File::appendToFile(std::string_view content) {
    throwErrorIfNoFilePath();
    m_fileStream.open(m_filePath, std::ios::app);
    throwErrorIfOpenFailed();

    m_fileStream << content;

    char lastChar {content[content.size() - 1]};
    if (lastChar != '\n') {
        m_fileStream << '\n';
    }

    m_fileStream.close();
    return *this;
};


File& File::writeLinesToFile(const std::vector<std::string>& lines) {
    throwErrorIfNoFilePath();
    m_fileStream.open(m_filePath, std::ios::out);
    throwErrorIfOpenFailed();

    for (const std::string& line : lines) {
        m_fileStream << line;
        char lastChar {line[line.size() - 1]};
        if (lastChar != '\n') {
            m_fileStream << '\n';
        }
    }

    m_fileStream.close();
    return *this;
};

File& File::writeLinesToFile(const std::vector<std::string_view>& lines) {
    throwErrorIfNoFilePath();
    m_fileStream.open(m_filePath, std::ios::out);
    throwErrorIfOpenFailed();

    for (std::string_view line : lines) {
        m_fileStream << line;
        char lastChar {line[line.size() - 1]};
        if (lastChar != '\n') {
            m_fileStream << '\n';
        }
    }

    m_fileStream.close();
    return *this;
};


File& File::appendLinesToFile(const std::vector<std::string>& lines) {
    throwErrorIfNoFilePath();
    m_fileStream.open(m_filePath, std::ios::app);
    throwErrorIfOpenFailed();

    for (const std::string& line : lines) {
        m_fileStream << line;
        char lastChar {line[line.size() - 1]};
        if (lastChar != '\n') {
            m_fileStream << '\n';
        }
    }

    m_fileStream.close();
    return *this;
};

File& File::appendLinesToFile(const std::vector<std::string_view>& lines) {
    throwErrorIfNoFilePath();
    m_fileStream.open(m_filePath, std::ios::app);
    throwErrorIfOpenFailed();

    for (std::string_view line : lines) {
        m_fileStream << line;
        char lastChar {line[line.size() - 1]};
        if (lastChar != '\n') {
            m_fileStream << '\n';
        }
    }

    m_fileStream.close();
    return *this;
};


void File::throwErrorIfOpenFailed() {
    if (!m_fileStream) {
        throw std::runtime_error("Error: Cannot open file: " + m_filePath);
    }
};


void File::throwErrorIfNoFilePath() {
    if (m_filePath.size() == 0) {
        throw std::runtime_error("Error: No file path given. (Use setFilePath to give file path.)");
    }
};


File& File::setFilePath(std::string_view filePath) {
    m_filePath = filePath;
    resetFilePtr();
    return *this;
};


bool File::hasFilePath() const {
    return m_filePath.size() > 0;
};


const std::string& File::filePath() const {
    return m_filePath;
};


bool operator==(const File& file1, const File& file2) {
    return file1.m_filePath == file2.m_filePath;
};


std::ostream& operator<<(std::ostream& out, const File& file) {
    out << "FileObject(filepath: " << file.m_filePath << ")";
    return out;
};