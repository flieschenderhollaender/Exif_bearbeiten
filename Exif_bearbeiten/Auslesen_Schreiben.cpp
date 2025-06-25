#include <iostream>
#include <string>
#include <memory>              // Für std::unique_ptr
#include <exiv2/exiv2.hpp>  
#include "ConsoleMenue.h"// Exiv2-Bibliothek einbinden

void showEXif(const std::string& filename) { // Funktion zum Anzeigen der EXIF-Daten samt Formaleingabeparameter filename 
	try {
		std::unique_ptr<Exiv2::Image> image = Exiv2::ImageFactory::open(filename); // Öffnen des Bildes mit Exiv2
		if (!image) {
			std::cerr << ConsoleUtils::getText("exiv2_open_error") << filename << std::endl;
			return;
		} // Überprüfen, ob das Bild erfolgreich geöffnet wurde

		image->readMetadata();
		Exiv2::ExifData& exifData = image->exifData();
		// Überprüfen, ob EXIF-Daten vorhanden sind
		if (exifData.empty()) {
			std::cout << ConsoleUtils::getText("no_exif_data") << filename << std::endl;
			return;
		}// Wenn keine EXIF-Daten vorhanden sind, eine entsprechende Nachricht ausgeben

		for (const auto& entry : exifData) {
			std::cout << entry.key() << " = " << entry.value() << std::endl;
		} // Alle EXIF-Daten durchlaufen und ausgeben
	}
	catch (const Exiv2::Error& e) {
		std::cerr << ConsoleUtils::getText("exiv2_read_error") << filename << ": " << e.what() << std::endl;
	}// Fehlermeldung ausgeben, wenn ein Fehler beim Lesen der EXIF-Daten auftritt
	catch (const std::exception& e) {
		std::cerr << ConsoleUtils::getText("general_read_error") << filename << ": " << e.what() << std::endl;
	}// Allgemeine Fehlermeldung ausgeben, wenn ein anderer Fehler auftritt
}


void ChangeCameraModel(const std::string& filename) { // Funktion zum Ändern des Kameramodells in den EXIF-Daten samt Formaleingabeparameter filename
	try {
		std::unique_ptr<Exiv2::Image> image = Exiv2::ImageFactory::open(filename);
		// Öffnen des Bildes mit Exiv2
		if (!image) 
		{
			std::cerr << ConsoleUtils::getText("exiv2_open_error") << filename << std::endl;
			return;
		}// Überprüfen, ob das Bild erfolgreich geöffnet wurde

		image->readMetadata();
		Exiv2::ExifData& exifData = image->exifData(); // EXIF-Daten des Bildes lesen

		std::cout << ConsoleUtils::getText("enter_manufacturer");
		std::string neuerHersteller;
		std::getline(std::cin, neuerHersteller); // Eingabe des Herstellers

		if (neuerHersteller.empty()) 
		{
			std::cerr << ConsoleUtils::getText("no_manufacturer_entered") << std::endl;
			return;
		}// Überprüfen, ob ein Hersteller eingegeben und korrekt übergeben wurde

		std::cout << ConsoleUtils::getText("enter_model");
		std::string neuesModell;
		std::getline(std::cin, neuesModell); // Eingabe des neuen Modells
		if (neuesModell.empty()) {
			std::cerr << ConsoleUtils::getText("no_model_entered") << std::endl;
			return;
		}// Überprüfen, ob ein Modell eingegeben und korrekt übergeben wurde

		
		exifData["Exif.Image.Make"] = neuerHersteller;// EXIF-Daten für den Hersteller aktualisieren
		exifData["Exif.Image.Model"] = neuesModell;	// EXIF-Daten für das Modell aktualisieren

		image->setExifData(exifData);
		image->writeMetadata();
		std::cout << ConsoleUtils::getText("camera_model_changed_success") << std::endl;
		// Änderungen in den EXIF-Daten speichern und Erfolgsmeldung ausgeben
	}
	catch (const Exiv2::Error& e) {
		std::cerr << ConsoleUtils::getText("exiv2_error") << ": " << e.what() << std::endl;
	}
	catch (const std::exception& e) {
		std::cerr << ConsoleUtils::getText("general_error") << ": " << e.what() << std::endl;
	}
}


void ChangeGPSData(const std::string& filename) { // Funktion zum Ändern der GPS-Daten in den EXIF-Daten samt Formaleingabeparameter filename
	try {
		std::unique_ptr<Exiv2::Image> image = Exiv2::ImageFactory::open(filename); // Öffnen des Bildes mit Exiv2
		if (!image) { // Überprüfen, ob EXIF-Daten vorhanden sind
			std::cerr << ConsoleUtils::getText("exiv2_open_error") << filename << std::endl;
			return;
		}//Wenn das Bild nicht geöffnet werden konnte, eine Fehlermeldung ausgeben

		image->readMetadata(); // Metadaten lesen
		Exiv2::ExifData& exifData = image->exifData(); // EXIF-Daten des Bildes lesen

		std::cout << std::endl << ConsoleUtils::getText("current_gps_data") << std::endl;// Aktuelle GPS-Daten anzeigen
		bool hasLongitude = exifData.findKey(Exiv2::ExifKey("Exif.GPSInfo.GPSLongitude")) != exifData.end();// Überprüfen, ob GPS-Daten vorhanden sind
		bool hasLatitude = exifData.findKey(Exiv2::ExifKey("Exif.GPSInfo.GPSLatitude")) != exifData.end();
		bool hasAltitude = exifData.findKey(Exiv2::ExifKey("Exif.GPSInfo.GPSAltitude")) != exifData.end();

		if (hasLongitude) { std::cout << ConsoleUtils::getText("longitude") << ": " << exifData["Exif.GPSInfo.GPSLongitude"] << std::endl; }// Wenn GPS-Daten vorhanden sind, diese ausgeben
		if (hasLatitude) { std::cout << ConsoleUtils::getText("latitude") << ": " << exifData["Exif.GPSInfo.GPSLatitude"] << std::endl; }
		if (hasAltitude) { std::cout << ConsoleUtils::getText("altitude") << ": " << exifData["Exif.GPSInfo.GPSAltitude"] << std::endl; }
		if (!hasLongitude && !hasLatitude && !hasAltitude) { std::cout << ConsoleUtils::getText("no_gps_data_found") << std::endl; }// Wenn keine GPS-Daten vorhanden sind, eine entsprechende Nachricht ausgeben

		std::cout << "\n" << ConsoleUtils::getText("what_to_change_gps") << "\n";// Auswahlmöglichkeiten für den Benutzer anzeigen
		std::cout << " 1. " << ConsoleUtils::getText("change_individual_values") << "\n";// Option zum Ändern einzelner GPS-Werte
		std::cout << " 2. " << ConsoleUtils::getText("change_all_values_gps") << "\n";// Option zum Ändern aller GPS-Werte
		std::cout << ConsoleUtils::getText("choice") << ": ";// Eingabeaufforderung für die Auswahl
		std::string auswahl;
		std::getline(std::cin, auswahl);

		bool changeLongitude = false, changeLatitude = false, changeAltitude = false;

		if (auswahl == "2") {
			changeLongitude = changeLatitude = changeAltitude = true;
		}// Wenn der Benutzer "2" wählt, sollen alle GPS-Werte geändert werden
		else if (auswahl == "1") {
			std::cout << ConsoleUtils::getText("change_longitude") << " (y/n): ";
			std::string input; std::getline(std::cin, input);
			if (input == "y" || input == "Y") changeLongitude = true;
			std::cout << ConsoleUtils::getText("change_latitude") << " (y/n): ";
			std::getline(std::cin, input);
			if (input == "y" || input == "Y") changeLatitude = true;
			std::cout << ConsoleUtils::getText("change_altitude") << " (y/n): ";
			std::getline(std::cin, input);
			if (input == "y" || input == "Y") changeAltitude = true;
		}// Wenn der Benutzer "1" wählt, sollen einzelne GPS-Werte geändert werden
		else {
			std::cerr << ConsoleUtils::getText("invalid_input") << std::endl;
			return;
		}// Wenn die Eingabe ungültig ist, eine Fehlermeldung ausgeben und die Funktion beenden

		if (changeLongitude) {
			std::cout << ConsoleUtils::getText("enter_new_longitude");
			std::string newLongitude;
			std::getline(std::cin, newLongitude);
			exifData["Exif.GPSInfo.GPSLongitude"] = newLongitude;
		}// Wenn der Benutzer die Änderung der Längengrad-Daten wünscht, wird eine Eingabeaufforderung angezeigt und die neuen Daten in die EXIF-Daten geschrieben

		if (changeLatitude) {
			std::cout << ConsoleUtils::getText("enter_new_latitude");
			std::string newLatitude;
			std::getline(std::cin, newLatitude);
			exifData["Exif.GPSInfo.GPSLatitude"] = newLatitude;
		}//	Wenn der Benutzer die Änderung der Breitengrad-Daten wünscht, wird eine Eingabeaufforderung angezeigt und die neuen Daten in die EXIF-Daten geschrieben
		if (changeAltitude) {
			std::cout << ConsoleUtils::getText("enter_new_altitude");
			std::string newAltitude;
			std::getline(std::cin, newAltitude);
			try {
				double altVal = std::stod(newAltitude);
				exifData["Exif.GPSInfo.GPSAltitude"] = Exiv2::Rational(static_cast<long>(altVal * 100), 100);
			}
			catch (const std::exception& e) {
				std::cerr << ConsoleUtils::getText("invalid_number_format") << newAltitude << ": " << e.what() << std::endl;
			}
		}// Wenn der Benutzer die Änderung der Höhenangaben wünscht, wird eine Eingabeaufforderung angezeigt und die neuen Daten in die EXIF-Daten geschrieben

		image->setExifData(exifData);
		image->writeMetadata();
		std::cout << ConsoleUtils::getText("gps_data_updated_success") << std::endl;

	}// Änderungen in den EXIF-Daten speichern und Erfolgsmeldung ausgeben
	catch (const Exiv2::Error& e) {
		std::cerr << ConsoleUtils::getText("exiv2_error") << ": " << e.what() << std::endl;
	}// Fehlermeldung ausgeben, wenn ein Fehler bei der Verarbeitung der EXIF-Daten auftritt
	catch (const std::exception& e) {
		std::cerr << ConsoleUtils::getText("general_error") << ": " << e.what() << std::endl;
	}// Allgemeine Fehlermeldung ausgeben, wenn ein anderer Fehler auftritt
}

void ChangeDateTime(const std::string& filename) {// Funktion zum Ändern des Datums und der Uhrzeit in den EXIF-Daten samt Formaleingabeparameter filename
	try {
		std::unique_ptr<Exiv2::Image> image = Exiv2::ImageFactory::open(filename);// Öffnen des Bildes mit Exiv2
		if (!image) {
			std::cerr << ConsoleUtils::getText("exiv2_open_error") << filename << std::endl;
			return;
		}// Überprüfen, ob das Bild erfolgreich geöffnet wurde

		image->readMetadata();
		Exiv2::ExifData& exifData = image->exifData();// EXIF-Daten des Bildes lesen

		auto it = exifData.findKey(Exiv2::ExifKey("Exif.Image.DateTime"));
		if (it != exifData.end()) {
			std::cout << ConsoleUtils::getText("current_datetime") << ": " << it->value() << std::endl;
		}// Aktuelles Datum und Uhrzeit anzeigen, wenn vorhanden
		else {
			std::cout << ConsoleUtils::getText("no_datetime_found") << "\n";
		}// Wenn kein Datum und Uhrzeit gefunden wurden, eine entsprechende Nachricht ausgeben

		std::cout << ConsoleUtils::getText("enter_new_datetime");// Eingabeaufforderung für das neue Datum und die Uhrzeit
		std::string newDateTime;
		std::getline(std::cin, newDateTime);// Eingabe des neuen Datums und der Uhrzeit

		if (newDateTime.length() != 19 || newDateTime[4] != ':' || newDateTime[7] != ':' || newDateTime[10] != ' ' || newDateTime[13] != ':' || newDateTime[16] != ':') {
			std::cerr << ConsoleUtils::getText("invalid_datetime_format") << std::endl;
			return;
		}// Überprüfen, ob das eingegebene Datum und die Uhrzeit im richtigen Format vorliegen (YYYY:MM:DD HH:MM:SS)

		exifData["Exif.Image.DateTime"] = newDateTime;
		image->setExifData(exifData);
		image->writeMetadata();// Änderungen in den EXIF-Daten speichern

		std::cout << ConsoleUtils::getText("datetime_updated_success") << "\n";
	}
	catch (const Exiv2::Error& e) {
		std::cerr << ConsoleUtils::getText("exiv2_error") << ": " << e.what() << std::endl;
	}// Fehlermeldung ausgeben, wenn ein Fehler bei der Verarbeitung der EXIF-Daten auftritt
	catch (const std::exception& e) {
		std::cerr << ConsoleUtils::getText("general_error") << ": " << e.what() << std::endl;
	}// Allgemeine Fehlermeldung ausgeben, wenn ein anderer Fehler auftritt
}

void ChangeOpticalData(const std::string& filename) {// Funktion zum Ändern der optischen Daten in den EXIF-Daten samt Formaleingabeparameter filename
	try {
		std::unique_ptr<Exiv2::Image> image = Exiv2::ImageFactory::open(filename);
		if (!image) { // Überprüfen, ob EXIF-Daten vorhanden sind
			std::cerr << ConsoleUtils::getText("exiv2_open_error") << filename << std::endl;
			return;
		}// Wenn das Bild nicht geöffnet werden konnte, eine Fehlermeldung ausgeben

		image->readMetadata(); // Metadaten lesen
		Exiv2::ExifData& exifData = image->exifData();// EXIF-Daten des Bildes lesen

		std::cout << std::endl << ConsoleUtils::getText("current_optical_data") << std::endl;

		bool hatBlende = exifData.findKey(Exiv2::ExifKey("Exif.Photo.FNumber")) != exifData.end();// Überprüfen, ob Blendenwerte vorhanden sind
		bool hatISO = exifData.findKey(Exiv2::ExifKey("Exif.Photo.ISOSpeedRatings")) != exifData.end();//Überprüfen, ob ISO-Werte vorhanden sind
		bool hatVerschlusszeit = exifData.findKey(Exiv2::ExifKey("Exif.Photo.ExposureTime")) != exifData.end();// Überprüfen, ob Verschlusszeiten vorhanden sind
		bool hatBrennweite = exifData.findKey(Exiv2::ExifKey("Exif.Photo.FocalLength")) != exifData.end();// Überprüfen, ob Brennweiten vorhanden sind



		std::cout << "\n" << ConsoleUtils::getText("what_to_change_optical") << "\n";// Auswahlmöglichkeiten für den Benutzer anzeigen
		std::cout << " 1. " << ConsoleUtils::getText("change_individual_values") << "\n";// Option zum Ändern einzelner optischer Werte
		std::cout << " 2. " << ConsoleUtils::getText("change_all_values_optical") << "\n";// Option zum Ändern aller optischen Werte
		std::cout << ConsoleUtils::getText("choice") << ": ";
		std::string auswahl;
		std::getline(std::cin, auswahl);// Eingabeaufforderung für die Auswahl

		bool changeBlende = false, changeIso = false, changeVerschlusszeit = false, changeBrennweite = false;

		if (auswahl == "2") {
			changeBlende = changeIso = changeVerschlusszeit = changeBrennweite = true;
		}// Wenn der Benutzer "2" wählt, sollen alle optischen Werte geändert werden
		else if (auswahl == "1") {
			std::cout << ConsoleUtils::getText("change_f_number") << " (y/n): ";// Abfrage ob der Benutzer die Blende aendern möchte
			std::string input; std::getline(std::cin, input);
			if (input == "y" || input == "Y") changeBlende = true;
			std::cout << ConsoleUtils::getText("change_iso") << " (y/n): ";// Abfrage ob der Benutzer die ISO aendern möchte
			std::getline(std::cin, input);
			if (input == "y" || input == "Y") changeIso = true;
			std::cout << ConsoleUtils::getText("change_shutterspeed") << " (y/n): ";// Abfrage ob der Benutzer die Verschlusszeit aendern möchte
			std::getline(std::cin, input);
			if (input == "y" || input == "Y") changeVerschlusszeit = true;
			std::cout << ConsoleUtils::getText("change_focal_length") << " (y/n): ";// Abfrage ob der Benutzer die Brennweite aendern möchte
			std::getline(std::cin, input);
			if (input == "y" || input == "Y") changeBrennweite = true;

		}// Wenn der Benutzer "1" wählt, sollen einzelne optische Werte geändert werden
		else {
			std::cerr << ConsoleUtils::getText("invalid_input") << std::endl;
			return;
		}

		if (changeBlende) {
			std::cout << ConsoleUtils::getText("enter_new_f_number");
			std::string NewFNumber;
			std::getline(std::cin, NewFNumber); // Eingabeaufforderung für die neue Blende
			try {
				float fnumberFloat = std::stof(NewFNumber);
				exifData["Exif.Photo.FNumber"] = Exiv2::Rational(static_cast<long>(fnumberFloat * 100), 100);// Umwandlung der Blende in einen rationalen Wert
			}
			catch (const std::exception& e) {// Fehlerbehandlung für ungültige Eingaben
				std::cerr << ConsoleUtils::getText("invalid_number_format") << NewFNumber << ": " << e.what() << std::endl;
			}
		}// Funktion zum Ändern der Blende, Eingabeaufforderung und Validierung der Eingabe

		if (changeIso) {
			std::cout << ConsoleUtils::getText("enter_new_iso");
			std::string newISO;
			std::getline(std::cin, newISO);// Eingabeaufforderung für die neue ISO
			try {
				int isoVal = std::stoi(newISO);
				exifData["Exif.Photo.ISOSpeedRatings"] = isoVal;// Umwandlung der ISO in einen ganzzahligen Wert
			}
			catch (const std::exception& e) {// Fehlerbehandlung für ungültige Eingaben
				std::cerr << ConsoleUtils::getText("invalid_number_format") << newISO << ": " << e.what() << std::endl;
			}
		}// Funktion zum Ändern der ISO, Eingabeaufforderung und Validierung der Eingabe
		if (changeVerschlusszeit) {
			std::cout << ConsoleUtils::getText("enter_new_shutterspeed");
			std::string newShutterspeed;
			std::getline(std::cin, newShutterspeed);
			exifData["Exif.Photo.ExposureTime"] = newShutterspeed;
		}// Eingabeaufforderung für die neue Verschlusszeit und Aktualisierung der EXIF-Daten
		if (changeBrennweite) {
			std::cout << ConsoleUtils::getText("enter_new_focal_length");
			std::string newFocalLength;
			std::getline(std::cin, newFocalLength);// Eingabeaufforderung für die neue Brennweite
			try {
				float focalLengthFloat = std::stof(newFocalLength);
				exifData["Exif.Photo.FocalLength"] = Exiv2::Rational(static_cast<long>(focalLengthFloat * 100), 100);// Umwandlung der Brennweite in einen rationalen Wert
			}
			catch (const std::exception& e) {
				std::cerr << ConsoleUtils::getText("invalid_number_format") << newFocalLength << ": " << e.what() << std::endl;// Fehlerbehandlung für ungültige Eingaben
			}// Eingabeaufforderung für die neue Brennweite und Aktualisierung der EXIF-Daten
		}

		image->setExifData(exifData);
		image->writeMetadata(); // Änderungen speichern
		std::cout << ConsoleUtils::getText("optical_data_updated_success") << std::endl;
	}
	catch (const Exiv2::Error& e) {
		std::cerr << ConsoleUtils::getText("exiv2_error") << ": " << e.what() << std::endl;
	}// Fehlermeldung ausgeben, wenn ein Fehler bei der Verarbeitung der EXIF-Daten auftritt
	catch (const std::exception& e) {
		std::cerr << ConsoleUtils::getText("general_error") << ": " << e.what() << std::endl;
	}// Allgemeine Fehlermeldung ausgeben, wenn ein anderer Fehler auftritt
}

void DeleteAllExifData(const std::string& filename) {// Funktion zum Löschen aller EXIF-Daten in einem Bild samt Formaleingabeparameter filename
	try {
		std::unique_ptr<Exiv2::Image> image = Exiv2::ImageFactory::open(filename);
		if (!image) {
			std::cerr << ConsoleUtils::getText("exiv2_open_error") << filename << std::endl;
			return;
		}// Überprüfen, ob das Bild erfolgreich geöffnet wurde
		image->readMetadata();
		Exiv2::ExifData& exifData = image->exifData();
		exifData.clear(); // Alle EXIF-Daten löschen
		image->setExifData(exifData);
		image->writeMetadata(); // Änderungen speichern
		std::cout << ConsoleUtils::getText("all_exif_data_deleted_success") << std::endl;
	}
	catch (const Exiv2::Error& e) {
		std::cerr << ConsoleUtils::getText("exiv2_error") << ": " << e.what() << std::endl;
	}// Fehlermeldung ausgeben, wenn ein Fehler bei der Verarbeitung der EXIF-Daten auftritt
	catch (const std::exception& e) {
		std::cerr << ConsoleUtils::getText("general_error") << ": " << e.what() << std::endl;
	}// Allgemeine Fehlermeldung ausgeben, wenn ein anderer Fehler auftritt
}
