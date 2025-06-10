#include "ConsoleMenue.h"
#include "ConsoleMenue.h"
#include <iostream>
#include <unordered_map>
#include <chrono>
#include <thread>

#ifdef _WIN32
#include <windows.h>
#endif

std::string ConsoleUtils::lang = "de";

void ConsoleUtils::setGreenText() {
#ifdef _WIN32
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
#else
    std::cout << "\033[32m"; // ANSI escape code for green text
#endif
}

void ConsoleUtils::resetTextColor() {
#ifdef _WIN32
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, 7); // Reset to default color
#else
    std::cout << "\033[0m"; // ANSI escape code to reset text color
#endif
}

void ConsoleUtils::setLanguage(const std::string& language) {
    lang = language;
}

std::string ConsoleUtils::getText(const std::string& key) {
    static const std::unordered_map<std::string, std::pair<std::string, std::string>> texts = {
        {"menuTitle", {"--- Exif-Bildbearbeitung ---", "--- Exif Image Editor ---"}},
        {"menu1", {"1. Exif-Daten anzeigen", "1. Show Exif Data"}},
        {"menu2", {"2. Hersteller und Kameramodell aendern", "2. Change manufactorer and camera model"}},
        {"menu3", {"3. GPS Informationen aendern", "3. Change GPS Information"}},
        {"menu4", {"4.  Optische Informationen aendern (Blende, ISO, Belichtungszeit,Brennweite)", "4. Change Optical Information (Aperture, ISO, Exposure Time,Focal Lenght)" }},
        {"menu5", {"5. Datum und Uhrzeit aendern", "5. Change Date and Time information"}},
        {"menu6", {"6. Alle Exif-Daten loeschen", "Delete all Exif information",}},
        {"menu10", {"Witz am Rande: Exif-Daten sind wie ein gutes Geheimnis - sie sollten nicht zu leicht zu finden sein!", "Joke on the side: Exif data is like a good secret - it shouldn't be too easy to find!"}},
        {"menu0", {"0. Beenden", "0. Exit"}},
        {"invalid", {"Ungueltige Eingabe.", "Invalid input."}},
        {"exit", {"Beenden...", "Exiting..."}},
        {"enterPaths", {"Bildpfade eingeben (mit Enter bestätigen, leer zum Beenden):",
                         "Enter image paths (press Enter after each, empty to finish):"}}
    };

    auto it = texts.find(key);
    if (it != texts.end()) {
        return lang == "en" ? it->second.second : it->second.first;
    }
    return key;
}

void ConsoleUtils::printMenu() {
    std::cout << "\n" << getText("menuTitle") << "\n";
    std::cout << getText("menu1") << "\n";
    std::cout << getText("menu2") << "\n";
    std::cout << getText("menu3") << "\n";
    std::cout << getText("menu4") << "\n";
    std::cout << getText("menu5") << "\n";
    std::cout << getText("menu6") << "\n";
    std::cout << getText("menu0") << "\n";
    std::cout << std::endl;
    std::cout << getText("menu10") << "\n";
}
