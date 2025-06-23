#include <iostream>
#include <string>
#include <exiv2/exiv2.hpp>
#include <vector>
#include <chrono>
#include <thread>
#include "ConsoleMenue.h"
#include "Auslesen_Schreiben.h"

//Matrix-Effekt  start

//Funktion zum Setzen des Konsolen-Cursors an bestimmte x-/y-Koordinaten
//so können Zeichen gezielt in der Konsole platziert werden
void setCursor(int x, int y) {
    COORD pos = { (SHORT)x, (SHORT)y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

//Ein- und Ausblenden des Konsolen-Cursors
//Cursor wird während Animation ausgeblendet
void hideCursor(bool hide) {
    HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(out, &cursorInfo);
    cursorInfo.bVisible = !hide;
    SetConsoleCursorInfo(out, &cursorInfo);
}

//Hauptfunktion des Matrix-Effektes
//Zeichen werden in zufälligen y-Positionen von oben nach unten bewegt
void runMatrixEffect(int durationMs = 5000, int delayMs = 50) {
    const int width = 100;
    const int height = 50;
    //speichert aktuelle y-Positionen der Zeichen
    std::vector<int> yPositions(width);

    //Initialisiert Zufallszahlen
    for (int i = 0; i < width; ++i)
        yPositions[i] = rand() % height;

    hideCursor(true); //Cursor ausblenden
    auto start = std::chrono::steady_clock::now();

    //Hauptschleife des Effekts: solange die Dauer nicht abgelaufen ist, werden Zeichen in zufälligen y-Positionen angezeigt
    while (std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::steady_clock::now() - start)
        .count() < durationMs)
    {
        // Iteration über die Breite der Konsole
        for (int i = 0; i < width; ++i) {
            // Zufällige y-Position für jedes Zeichen
            setCursor(i, yPositions[i]);
            char c = 33 + rand() % 93;
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10); // Grün
            std::cout << c;

            //Löscht Zeichen zwei Zeilen darüber, um fallende Bewegung zu simulieren
            setCursor(i, (yPositions[i] - 2 + height) % height);
            std::cout << " ";
            //bewegt y-Position nach unten
            yPositions[i] = (yPositions[i] + 1) % height;
        }
        //steuern der Animationsgeschwindigkeit
        std::this_thread::sleep_for(std::chrono::milliseconds(delayMs));
    }
    system("cls"); // Bildschirm löschen
    hideCursor(false);//Cursor einblenden
}
//Matrix Effekt ende

// Hilfsfunktion zum Entfernen von Anführungszeichen am Anfang und Ende
std::string trimQuotes(const std::string& str) {
    if (str.length() >= 2 &&
        ((str.front() == '"' && str.back() == '"') ||
         (str.front() == '\'' && str.back() == '\''))) {
        return str.substr(1, str.length() - 2);
    }
    return str;
}

int main() {
    // setze Schriftfarbe auf grün
    ConsoleUtils::setGreenText();

    std::string langChoice; //Variable für die Spracheingabe

    //Schleife zur Sprachauswahl(solange, bis Eingabe gültig)
    while (true)
    {
        std::cout << "Sprache waehlen / Select language (de/en): " << std::endl;
        std::getline(std::cin, langChoice);
        if (langChoice == "en") {
            ConsoleUtils::setLanguage("en"); //setzt Sprache auf Englisch
            std::cout << "Loading Language..." << std::endl;
            Sleep(3000); // Wartezeit zum lesen von "Loading Language..."
            break;
        }
        else if (langChoice == "de") {
            ConsoleUtils::setLanguage("de"); //setzt Sprache auf Deutsch
            std::cout << "Sprache wird geladen..." << std::endl;
            Sleep(3000); // Wartezeit zum lesen von "Sprache wird geladen..."
            break;
        }
        else {
            // Fehlermeldung bei ungültiger Eingabe
            std::cout << "Nicht unterstuetztes Sprachformat./Language-Package not availible" << std::endl;
        }
    }


    // Matrix-Effekt anzeigen
    runMatrixEffect();


    std::vector<std::string> imagePaths;            // Vektor für die Bildpfade
    std::string path;
    // Eingabeaufforderung für Bildpfade
    std::cout << ConsoleUtils::getText("enterPaths") << "\n";
    // Eingabe der Bildpfade, solange bis eine leere Zeile eingegeben wird
    while (true) {                      
		std::getline(std::cin, path);               // Eingabe der Dateipfade
		if (path.empty()) break;                    // Beenden, wenn eine leere Zeile eingegeben wird
		imagePaths.push_back(trimQuotes(path));     //Übergabe fuer Anführungszeichen loeschen
    }

    

	system("cls");  // Bildschirm löschen

    int choice; // Variable für Menüauswahl

    // Hauptmenü-Schleife
    // wird bei Eingabe von 0 beendet
    do {
        ConsoleUtils::printMenu();   // gibt das Menü aus
        std::cin >> choice;          // Eingabe der Menüoption
        std::cin.ignore();           // Löschen des Zeilenumbruchs im Eingabestrom

        switch (choice) {

        case 0:
            //Meldung bei Programmende
            std::cout << ConsoleUtils::getText("exit") << "\n";
            break;

        case 1:
            // Zeigt die EXIF-Daten für alle eingegebenen Bildpfade an
            for (const auto& path : imagePaths) {
                std::cout << "Verarbeite Datei: " << path << std::endl;
                showEXif(path);
            }
            break;

        case 2:
            // Ändert das Kameramodell für alle eingegebenen Bildpfade
            for (const auto& path : imagePaths) {
                std::cout << "Verarbeite Datei: " << path << std::endl;
                ChangeCameraModel(path);
            }
            break;

        case 3:
            // Ändert die GPS-Daten für alle eingegebenen Bildpfade
            for (const auto& path : imagePaths) {
                std::cout << "Verarbeite Datei: " << path << std::endl;
                ChangeGPSData(path);
            }
            break;

        case 4:
            // Ändert die optischen Daten für alle eingegebenen Bildpfade
            for (const auto& path : imagePaths) {
                std::cout << "Verarbeite Datei: " << path << std::endl;
                ChangeOpticalData(path);
            }
            break;

        case 5:
            // Ändert das Aufnahmedatum für alle eingegebenen Bildpfade
            for (const auto& path : imagePaths) {
                std::cout << "Verarbeite Datei: " << path << std::endl;
                ChangeDateTime(path);
            }
            break;

        case 6:
            // Löscht alle EXIF-Daten für alle eingegebenen Bildpfade
            for (const auto& path : imagePaths) {
                std::cout << "Verarbeite Datei: " << path << std::endl;
                DeleteAllExifData(path);
            }
            break;

        default:
            //Meldung bei ungültiger Auswahl
            std::cout << ConsoleUtils::getText("invalid") << "\n";
        }
    } while (choice != 0); // Schleife läuft weiter, bis 0 eingegeben wird

    return 0; // Programm erfolgreich beendet
}
