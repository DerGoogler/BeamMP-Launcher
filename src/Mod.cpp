#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include "Mod.h"
#include <zip.h>
#if defined(_WIN32)
#include <windows.h>
#elif defined(__linux__)
#include <unistd.h>
#endif

namespace fs = std::filesystem;

bool UnpackZip(const std::string& zipFilePath, const std::string& destDir) {
    // Open the ZIP archive
    int err = 0;
    zip_t* archive = zip_open(zipFilePath.c_str(), 0, &err);
    if (!archive) {
        std::cerr << "Failed to open ZIP file: " << zipFilePath << " (Error code: " << err << ")\n";
        return false;
    }

    // Create destination directory if it doesn't exist
    fs::path destPath(destDir);
    if (!fs::exists(destPath)) {
        fs::create_directories(destPath);
    }

    // Iterate over the entries in the ZIP file
    zip_int64_t numEntries = zip_get_num_entries(archive, 0);
    for (zip_int64_t i = 0; i < numEntries; ++i) {
        const char* name = zip_get_name(archive, i, 0);
        if (!name) {
            std::cerr << "Failed to get name for entry " << i << "\n";
            continue;
        }

        // Create full path for extracted file
        fs::path filePath = destPath / name;

        // Create directories if the entry is a directory
        if (name[strlen(name) - 1] == '/') {
            fs::create_directories(filePath);
            continue;
        }

        // Open the file in the ZIP archive
        zip_file_t* file = zip_fopen(archive, name, 0);
        if (!file) {
            std::cerr << "Failed to open file in ZIP: " << name << "\n";
            continue;
        }

        // Create the output file
        std::ofstream outFile(filePath, std::ios::binary);
        if (!outFile) {
            std::cerr << "Failed to create output file: " << filePath << "\n";
            zip_fclose(file);
            continue;
        }

        // Read from the ZIP file and write to the output file
        char buffer[8192];
        zip_int64_t bytesRead;
        while ((bytesRead = zip_fread(file, buffer, sizeof(buffer))) > 0) {
            outFile.write(buffer, bytesRead);
        }

        // Clean up
        outFile.close();
        zip_fclose(file);
    }

    // Close the ZIP archive
    zip_close(archive);
    return true;
}

void ReplaceTextInFile(const std::string& filePath, const std::string& toReplace, const std::string& replacement) {
    // Open the file in read mode
    std::ifstream inputFile(filePath);
    if (!inputFile.is_open()) {
        std::cerr << "Error opening file: " << filePath << std::endl;
        return;
    }

    // Read the file's content into a string
    std::ostringstream oss;
    oss << inputFile.rdbuf();
    std::string fileContent = oss.str();
    inputFile.close();  // Close the input file

    // Replace the text
    size_t pos = 0;
    while ((pos = fileContent.find(toReplace, pos)) != std::string::npos) {
        fileContent.replace(pos, toReplace.length(), replacement);
        pos += replacement.length();  // Move past the last replaced substring
    }

    // Open the file in write mode to overwrite it
    std::ofstream outputFile(filePath);
    if (!outputFile.is_open()) {
        std::cerr << "Error opening file for writing: " << filePath << std::endl;
        return;
    }

    // Write the modified content back to the file
    outputFile << fileContent;
    outputFile.close();  // Close the output file
}