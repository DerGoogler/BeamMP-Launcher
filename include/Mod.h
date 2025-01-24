#pragma once
#include <compare>
#include <string>
#include <vector>

bool UnpackZip(const std::string& zipFilePath, const std::string& destDir);
void ReplaceTextInFile(const std::string& filePath, const std::string& toReplace, const std::string& replacement);
