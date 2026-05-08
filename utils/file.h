#ifndef MY_FILE_H
#define MY_FILE_H


#include <string>
#include <iostream>
#include <string_view>
#include <vector>
#include <fstream>


class File {
    std::fstream m_fileStream{};
    std::string m_filePath{};

    std::streamoff m_filePtrOffset {0};
    bool m_filePtrAtEOF {false};


public:
    explicit File(std::string_view filePath);
    File();

    // Reads the whole file into a string. Throws an std::runtime_error if unable to open file.
    std::string readFile();

    // Reads the file into an array of its lines. Throws an std::runtime_error if unable to open file.
    std::vector<std::string> readLines();

    // Reads a line from the file. The last pos is saved so sequential calls will return sequential lines.
    // Returns an empty string when EOF is reached. Throws an std::runtime_error if unable to open file.
    std::string readLine();

    // Resets the saved file pos. This will make readLine read from the beginning of the file again.
    void resetFilePtr();

    // Write content to file overwriting everything that was in the file. Throws an std::runtime_error if unable to open file.
    // If the content does not end in a newline character, one will be added.
    File& writeToFile(std::string_view content);

    // Append the content to the end of the file. Throws an std::runtime_error if unable to open file.
    // If the content does not end in a newline character, one will be added.
    File& appendToFile(std::string_view content);

    // Write an array of lines to file overwriting everything that was in the file. 
    // Throws an std::runtime_error if unable to open file. 
    // If each line does not end in a newline character, one will be added.
    File& writeLinesToFile(const std::vector<std::string>& lines);

    // Write an array of lines to file overwriting everything that was in the file. 
    // Throws an std::runtime_error if unable to open file. 
    // If each line does not end in a newline character, one will be added.
    File& writeLinesToFile(const std::vector<std::string_view>& lines);

    // Append an array of lines to the end of the file. Throws an std::runtime_error if unable to open file. 
    // If each line does not end in a newline character, one will be added.
    File& appendLinesToFile(const std::vector<std::string>& lines);

    // Append an array of lines to the end of the file. Throws an std::runtime_error if unable to open file. 
    // If each line does not end in a newline character, one will be added.
    File& appendLinesToFile(const std::vector<std::string_view>& lines);

    // Change the file that this object works on. This will reset the file ptr as well.
    File& setFilePath(std::string_view filePath);

    // Returns true if the current filePath is not an empty string. 
    bool hasFilePath() const;

    // Returns the current file path this object is working on.
    const std::string& filePath() const;

    // Returns true if both objects have the same file path string. 
    friend bool operator==(const File& file1, const File& file2);


    friend std::ostream& operator<<(std::ostream& out, const File& file);


private:
    void throwErrorIfOpenFailed();
    void throwErrorIfNoFilePath();
};



#endif