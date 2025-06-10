#include <iostream>
#include <string>
#include <exiv2/exiv2.hpp>
#include <vector>
#include <chrono>
#include <thread>
#include "ConsoleMenue.h"
#include "Auslesen_Schreiben.h"

// ===================== Matrix-Effekt ======================
void setCursor(int x, int y) {
    COORD pos = { (SHORT)x, (SHORT)y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

void hideCursor(bool hide) {
    HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(out, &cursorInfo);
    cursorInfo.bVisible = !hide;
    SetConsoleCursorInfo(out, &cursorInfo);
}

void runMatrixEffect(int durationMs = 5000, int delayMs = 50) {
    const int width = 100;
    const int height = 50;
    std::vector<int> yPositions(width);
    for (int i = 0; i < width; ++i)
        yPositions[i] = rand() % height;

    hideCursor(true);
    auto start = std::chrono::steady_clock::now();
    while (std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::steady_clock::now() - start)
        .count() < durationMs)
    {
        for (int i = 0; i < width; ++i) {
            setCursor(i, yPositions[i]);
            char c = 33 + rand() % 93;
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10); // Grün
            std::cout << c;

            setCursor(i, (yPositions[i] - 2 + height) % height);
            std::cout << " ";

            yPositions[i] = (yPositions[i] + 1) % height;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(delayMs));
    }
    system("cls"); // Bildschirm löschen
    hideCursor(false);
}
// ===========================================================



int main() {
    ConsoleUtils::setGreenText();

    std::string langChoice;

    while (true)
    {
        std::cout << "Sprache waehlen / Select language (de/en): " << std::endl;
        std::getline(std::cin, langChoice);
        if (langChoice == "en") {
            ConsoleUtils::setLanguage("en");
            std::cout << "Loading Language." << std::endl;
            //Sleep(3000); // Wartezeit zum lesen.
            break;
        }
        else if (langChoice == "de") {
            ConsoleUtils::setLanguage("de");
            std::cout << "Sprache wird geladen." << std::endl;
            //Sleep(3000); // Wartezeit zum lesen.
            break;
        }
        else {
            std::cout << "Nicht unterstuetztes Sprachformat/Language-Package not availible" << std::endl;
        }
    }


    // Matrix-Effekt anzeigen
    //runMatrixEffect();


    std::vector<std::string> imagePaths;
    std::string path;
    std::cout << ConsoleUtils::getText("enterPaths") << "\n";
    while (true) {
        std::getline(std::cin, path);
        if (path.empty()) break;
        imagePaths.push_back(path);
    }

 
    int choice;
    do {
        ConsoleUtils::printMenu();
        std::cin >> choice;
        std::cin.ignore();

        switch (choice) {

        case 0:
            std::cout << ConsoleUtils::getText("exit") << "\n";
            break;

        case 1:
            for (const auto& path : imagePaths) {
                std::cout << "Verarbeite Datei: " << path << std::endl;
                showEXif(path);
            }
            break;



        case 2: for (const auto& path : imagePaths) {
            std::cout << "Verarbeite Datei: " << path << std::endl;
            ChangeCameraModel(path);
        }
              break;

        case 3: for (const auto& path : imagePaths) {
            std::cout << "Verarbeite Datei: " << path << std::endl;
            ChangeGPSData(path);
        }
              break;

        case 4:
            for (const auto& path : imagePaths) {
                std::cout << "Verarbeite Datei: " << path << std::endl;
                ChangeOpticalData(path);
            }
            break;
        case 5: for (const auto& path : imagePaths) {
            std::cout << "Verarbeite Datei: " << path << std::endl;
            ChangeDateTime(path);
        }
              break;

        case 6: for (const auto& path : imagePaths) {
            std::cout << "Verarbeite Datei: " << path << std::endl;
            DeleteAllExifData(path);
        }
              break;


        default:
            std::cout << ConsoleUtils::getText("invalid") << "\n";
        }
    } while (choice != 0);

    ConsoleUtils::resetTextColor();
    return 0;
}
