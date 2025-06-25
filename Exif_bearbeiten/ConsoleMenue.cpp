#include "ConsoleMenue.h"
#include "ConsoleMenue.h"
#include <iostream>
#include <unordered_map>
#include <chrono>
#include <thread>

#ifdef _WIN32
#include <windows.h>
#endif

// Initialisiert die Standardsprache auf Englisch
std::string ConsoleUtils::lang = "en";

// Sprachdatenbank: Je nachdem welche Sprache am Anfang gesetzt wurde, werden die entsprechenden Texte geladen, sollten neue Funktionen
// hinzugefügt werden, müssen diese hier auch in die Datenbank eingetragen werden ( in der Funktion steht ein Schlüsselwort, welches dann in der Funktion getText() verwendet wird, um den Text zu laden )
// andere Sprachen einfach un beliebig hinzufügen, die Struktur bleibt gleich, nur die Texte müssen übersetzt werden.
static const std::unordered_map<std::string, std::unordered_map<std::string, std::string>> translations = {
    { "en", {
		// Begrüßungstext und Menüoptionen auf Englisch
        { "welcome", "Welcome to the Exif Manipulator!" },
        { "choose_option", "Please choose an option:" },
        { "read_exif", "1. Read Exif Data" },
        { "write_exif", "2. Write Camera Model" },
        { "delete_exif", "3. Write GPS Data" }, 
        { "optical", "4. Change Optical Info" },
        { "datetime", "5. Change Date/Time" },
        { "remove_all", "6. Delete all Exif Data" },
        { "exit", "Exiting program..." },
        { "invalid", "Invalid option. Please try again." },
        { "enterPaths", "Enter image paths (empty line to finish):" },
        { "fileExists", "File exists: " },
        { "fileNotFound", "File not found: " },
        { "fileProcessing", "Process File: "},

        // für Auslesen_Schreiben.cpp
        { "exiv2_open_error", "Error: Could not open file with Exiv2 or it's not a valid image format: " },
        { "no_exif_data", "No Exif data found in file: " },
        { "exif_data_for", "Exif data for " },
        { "exiv2_read_error", "Exiv2 error reading Exif data for " },
        { "general_read_error", "General error reading Exif data for " },
        { "enter_manufacturer", "Please enter the Manufacturer: " },
        { "no_manufacturer_entered", "No Manufacturer entered." },
        { "enter_model", "Please enter the Model: " },
        { "no_model_entered", "No Model entered." },
        { "camera_model_changed_success", "Camera model updated successfully." },
        { "exiv2_error", "Exiv2 error" },
        { "general_error", "General error" },
        { "current_gps_data", "Current GPS Data" },
        { "longitude", "Longitude" },
        { "latitude", "Latitude" },
        { "altitude", "Altitude" },
        { "no_gps_data_found", "No GPS data found." },
        { "what_to_change_gps", "What would you like to change in GPS data?" },
        { "change_individual_values", "Change individual values" },
        { "change_all_values_gps", "Change all values" },
        { "choice", "Choice" },
        { "change_longitude", "Change Longitude" },
        { "change_latitude", "Change Latitude" },
        { "change_altitude", "Change Altitude" },
        { "enter_new_longitude", "Enter new Longitude (e.g. 48/1 7/1 30/1 for 48deg 07min 30sek):" },
        { "enter_new_latitude", "Enter new Latitude (e.g. 48/1 7/1 30/1 for 48deg 07min 30sek):" },
        { "enter_new_altitude", "Enter new Altitude (e.g. 100 for 100 meters): " },
        { "invalid_number_format", "Invalid number format entered: " },
        { "gps_data_updated_success", "GPS data updated successfully." },
        { "current_datetime", "Current DateTime" },
        { "no_datetime_found", "No DateTime found in metadata." },
        { "enter_new_datetime", "Enter new DateTime (YYYY:MM:DD HH:MM:SS): " },
        { "invalid_datetime_format", "Invalid DateTime format. Please use YYYY:MM:DD HH:MM:SS." },
        { "datetime_updated_success", "DateTime updated successfully." },
        { "current_optical_data", "Current Optical Data" },
        { "not_found", "Not found" },
        { "f_number", "F-Number" },
        { "iso", "ISO" },
        { "exposure_time", "Exposure Time" },
        { "focal_length", "Focal Length" },
        { "what_to_change_optical", "What would you like to change in optical data?" },
        { "change_all_values_optical", "Change all values" },
        { "change_f_number", "Change F Number" },
        { "change_iso", "Change ISO" },
        { "change_shutterspeed", "Change Shutterspeed" },
        { "change_focal_length", "Change Focal Length" },
        { "enter_new_f_number", "Enter new F Number (e.g. 2.8 for f/2.8): " },
        { "enter_new_iso", "Enter new ISO (e.g. 100): " },
        { "enter_new_shutterspeed", "Enter new Shutterspeed (e.g. 1/125 for 1/125 seconds): " },
        { "enter_new_focal_length", "Enter new Focal Length (e.g. 50 for 50mm): " },
        { "optical_data_updated_success", "Optical data updated successfully." },
        { "all_exif_data_deleted_success", "All Exif data deleted successfully." }
    }},
    { "de", {
		// Begrüßungstext und Menüoptionen auf Deutsch
        { "welcome", "Willkommen beim Exif-Manipulator!" },
        { "choose_option", "Bitte waehlen Sie eine Option:" },
        { "read_exif", "1. Exif-Daten auslesen" },
        { "write_exif", "2. Kameramodell schreiben" },
        { "delete_exif", "3. GPS-Daten schreiben" }, 
        { "optical", "4. Optische Daten aendern" },
        { "datetime", "5. Datum/Uhrzeit aendern" },
        { "remove_all", "6. Alle Exif-Daten loeschen" },
        { "exit", "Programm wird beendet..." },
        { "invalid", "Ungueltige Eingabe. Bitte erneut versuchen." },
        { "enterPaths", "Bitte Bildpfade eingeben (Leere Zeile zum Beenden):" },
        { "fileExists", "Datei existiert: " },
        { "fileNotFound", "Datei nicht gefunden: " },
		{ "fileProcessing", "Verarbeite Datei: "},

        // für Auslesen_Schreiben.cpp
        { "exiv2_open_error", "Fehler: Datei konnte mit Exiv2 nicht geoeffnet werden oder ist kein gueltiges Bildformat: " },
        { "no_exif_data", "Keine Exif-Daten in der Datei gefunden: " },
        { "exif_data_for", "Exif-Daten fuer " },
        { "exiv2_read_error", "Exiv2 Fehler beim Auslesen der Exif-Daten für " },
        { "general_read_error", "Allgemeiner Fehler beim Auslesen der Exif-Daten fuer " },
        { "enter_manufacturer", "Bitte geben Sie den Hersteller ein: " },
        { "no_manufacturer_entered", "Kein Hersteller eingegeben." },
        { "enter_model", "Bitte geben Sie das Modell ein: " },
        { "no_model_entered", "Kein Modell eingegeben." },
        { "camera_model_changed_success", "Kameramodell erfolgreich geaendert." },
        { "exiv2_error", "Exiv2 Fehler" },
        { "general_error", "Allgemeiner Fehler" },
        { "current_gps_data", "Aktuelle GPS-Daten" },
        { "longitude", "Laengengrad" },
        { "latitude", "Breitengrad" },
        { "altitude", "Hoehe" },
        { "no_gps_data_found", "Keine GPS-Daten gefunden." },
        { "what_to_change_gps", "Was moechten Sie bei den GPS-Daten aendern?" },
        { "change_individual_values", "Einzelne Werte aendern" },
        { "change_all_values_gps", "Alle Werte aendern" },
        { "choice", "Auswahl" },
        { "change_longitude", "Laengengrad aendern" },
        { "change_latitude", "Breitengrad aendern" },
        { "change_altitude", "Hoehe aendern" },
        { "enter_new_longitude", "Neuen Laengengrad eingeben (z.B. 48/1 7/1 30/1 fuer 48deg 07min 30sek):" },
        { "enter_new_latitude", "Neuen Breitengrad eingeben (z.B. 48/1 7/1 30/1 fuer 48deg 07min 30sek):" },
        { "enter_new_altitude", "Neue Hoehe eingeben (z.B. 100 für 100 Meter): " },
        { "invalid_number_format", "Ungueltiges Zahlenformat eingegeben: " },
        { "gps_data_updated_success", "GPS-Daten erfolgreich aktualisiert." },
        { "current_datetime", "Aktuelles Datum/Uhrzeit" },
        { "no_datetime_found", "Keine Datum/Uhrzeit-Daten in den Metadaten gefunden." },
        { "enter_new_datetime", "Neues Datum/Uhrzeit eingeben (JJJJ:MM:TT HH:MM:SS): " },
        { "invalid_datetime_format", "Ungueltiges Datum/Uhrzeit-Format. Bitte verwenden Sie JJJJ:MM:TT HH:MM:SS." },
        { "datetime_updated_success", "Datum/Uhrzeit erfolgreich aktualisiert." },
        { "current_optical_data", "Aktuelle optische Daten" },
        { "not_found", "Nicht gefunden" },
        { "f_number", "Blende" },
        { "iso", "ISO" },
        { "exposure_time", "Belichtungszeit" },
        { "focal_length", "Brennweite" },
        { "what_to_change_optical", "Was moechten Sie bei den optischen Daten aendern?" },
        { "change_all_values_optical", "Alle Werte aendern" },
        { "change_f_number", "Blende aendern" },
        { "change_iso", "ISO aendern" },
        { "change_shutterspeed", "Belichtungszeit aendern" },
        { "change_focal_length", "Brennweite aendern" },
        { "enter_new_f_number", "Neue Blende eingeben (z.B. 2.8 fuer f/2.8): " },
        { "enter_new_iso", "Neue ISO eingeben (z.B. 100): " },
        { "enter_new_shutterspeed", "Neue Belichtungszeit eingeben (z.B. 1/125 für 1/125 Sekunden): " },
        { "enter_new_iso", "Neue ISO eingeben (z.B. 100): " },
        { "enter_new_shutterspeed", "Neue Belichtungszeit eingeben (z.B. 1/125 für 1/125 Sekunden): " },
        { "enter_new_focal_length", "Neue Brennweite eingeben (z.B. 50 fuer 50mm): " },
        { "optical_data_updated_success", "Optische Daten erfolgreich aktualisiert." },
        { "all_exif_data_deleted_success", "Alle Exif-Daten erfolgreich geloescht." }
    }}
};


void ConsoleUtils::setGreenText() {
#ifdef _WIN32
    //grüne Farbe für Windows
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
#else
	//grüne Farbe für Linux/Mac
    std::cout << "\033[32m"; 
#endif
}

void ConsoleUtils::setLanguage(const std::string& language) {
	//Prüft, ob Sprache in Sprachdatenbank vorhanden ist
	if (translations.count(language) > 0) {
		lang = language; // wenn ja, wird die Sprache gesetzt
	}
	else {
        // Wenn nicht unterstützt, wird Sprache auf Englisch gesetzt
		std::cerr << "Unsupported language: " << language << std::endl;
		lang = "en";
	}
}


std::string ConsoleUtils::getText(const std::string& key) {

	// Sucht den Text in der Sprachdatenbank anhand des Schlüsselwortes
	auto langMapIt = translations.find(lang);
	if (langMapIt == translations.end()) {
		return key; //Wenn Sprache nicht gefunden, wird der Schlüssel zurückgegeben
	}
	const auto& texts = langMapIt->second; // Sucht die Texte für die aktuelle Sprache
	auto textIt = texts.find(key);
	if (textIt != texts.end()) {
		return textIt->second; // Wenn der Text gefunden wird, wird er zurückgegeben
	}
	return key; // Wenn der Text nicht gefunden wird, wird der Schlüssel zurückgegeben 
}

void ConsoleUtils::printMenu() {
	std::cout << std::endl; // Fügt eine Leerzeile zur Übersichtlichkeit ein
	//Verwendet die getText-Funktion, um die Texte aus der Sprachdatenbank zu laden
    std::cout << getText("welcome") << std::endl;
    std::cout << getText("choose_option") << std::endl;
    std::cout << getText("read_exif") << std::endl;
    std::cout << getText("write_exif") << std::endl;
    std::cout << getText("delete_exif") << std::endl;
    std::cout << getText("optical") << std::endl;
    std::cout << getText("datetime") << std::endl;
	std::cout << getText("remove_all") << std::endl;
    std::cout << getText("0. Exit / Beenden") << std::endl;
}
