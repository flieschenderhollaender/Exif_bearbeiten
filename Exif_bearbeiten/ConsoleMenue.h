#pragma once
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
#include <exiv2/exiv2.hpp>
#include <windows.h> // For Windows console color manipulation


class ConsoleUtils
{
public:
    static void setGreenText();
    static void resetTextColor();
    static void printMenu();
    static void setLanguage(const std::string& language);
    static std::string getText(const std::string& key);

private:
    static std::string lang;
};

