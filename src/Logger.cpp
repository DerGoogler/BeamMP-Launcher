/*
 Copyright (C) 2024 BeamMP Ltd., BeamMP team and contributors.
 Licensed under AGPL-3.0 (or later), see <https://www.gnu.org/licenses/>.
 SPDX-License-Identifier: AGPL-3.0-or-later
*/


#include "Logger.h"
#include "Startup.h"
#include <chrono>
#include <fstream>
#include <sstream>
#include <thread>
#include <regex>
#include "Options.h"

std::string getDate() {
    time_t tt = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    tm local_tm = *localtime(&tt);
    std::stringstream date;
    int S = local_tm.tm_sec;
    int M = local_tm.tm_min;
    int H = local_tm.tm_hour;
    std::string Secs = (S > 9 ? std::to_string(S) : "0" + std::to_string(S));
    std::string Min = (M > 9 ? std::to_string(M) : "0" + std::to_string(M));
    std::string Hour = (H > 9 ? std::to_string(H) : "0" + std::to_string(H));
    date
        << "["
        << local_tm.tm_mday << "/"
        << local_tm.tm_mon + 1 << "/"
        << local_tm.tm_year + 1900 << " "
        << Hour << ":"
        << Min << ":"
        << Secs
        << "] ";
    return date.str();
}
void InitLog() {
    std::ofstream LFS;
    LFS.open(GetEP() + "Launcher.log");
    if (!LFS.is_open()) {
        error("logger file init failed!");
    } else
        LFS.close();
}
void addToLog(const std::string& Line) {
    std::ofstream LFS;
    std::regex ansi_escape_regex("\x1B\\[[0-9;]*[a-zA-Z]");
    std::string clean_message = std::regex_replace(Line.c_str(), ansi_escape_regex, "");
    LFS.open(GetEP() + "Launcher.log", std::ios_base::app);
    LFS << clean_message.c_str();
    LFS.close();
}
void info(const std::string& toPrint) {
    std::string Print = getDate() + "\x1b[37;44m[INFO]\x1b[0m " + toPrint + "\n";
    std::cout << Print;
    addToLog(Print);
}
void debug(const std::string& toPrint) {
    if (!options.verbose)
        return;
    std::string Print = getDate() + "\x1b[30;47m[DEBUG]\x1b[0m " + toPrint + "\n";
    std::cout << Print;
    addToLog(Print);
}
void warn(const std::string& toPrint) {
    std::string Print = getDate() + "\x1b[30;43m[WARN]\x1b[0m " + toPrint + "\n";
    std::cout << Print;
    addToLog(Print);
}
void error(const std::string& toPrint) {
    std::string Print = getDate() + "\x1b[37;41m[ERROR]\x1b[0m " + toPrint + "\n";
    std::cout << Print;
    addToLog(Print);
}
void fatal(const std::string& toPrint) {
    std::string Print = getDate() + "\x1b[30;41;1;4m[FATAL]\x1b[0m " + toPrint + "\n";
    std::cout << Print;
    addToLog(Print);
    std::this_thread::sleep_for(std::chrono::seconds(5));
    _Exit(-1);
}
void except(const std::string& toPrint) {
    std::string Print = getDate() + "\x1b[37;45m[EXCEP]\x1b[0m " + toPrint + "\n";
    std::cout << Print;
    addToLog(Print);
}
