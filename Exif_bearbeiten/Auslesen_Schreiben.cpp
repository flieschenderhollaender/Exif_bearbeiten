#include <iostream>
#include <string>
#include <memory>              // F�r std::unique_ptr
#include <exiv2/exiv2.hpp>     // Exiv2-Bibliothek einbinden

void showEXif(const std::string& filename) {
	try {
		std::unique_ptr<Exiv2::Image> image = Exiv2::ImageFactory::open(filename);
		if (!image) {
			std::cerr << "Bild konnte nicht ge�ffnet werden: " << filename << std::endl;
			return;
		}

		image->readMetadata();
		Exiv2::ExifData& exifData = image->exifData();

		if (exifData.empty()) {
			std::cout << "Keine EXIF-Daten gefunden." << std::endl;
			return;
		}

		for (const auto& entry : exifData) {
			std::cout << entry.key() << " = " << entry.value() << std::endl;
		}
	}
	catch (const Exiv2::Error& e) {
		std::cerr << "Fehler: " << e.what() << std::endl;
	}
}
void ChangeCameraModel(const std::string& filename) {
	try {
		std::unique_ptr<Exiv2::Image> image = Exiv2::ImageFactory::open(filename);
		if (!image) {
			std::cerr << "Bild konnte nicht ge�ffnet werden: " << filename << std::endl;
			return;
		}
		std::cout << "Please enter the Manufacturer";
		std::string neuerHersteller;
		std::getline(std::cin, neuerHersteller); // Eingabe des neuen Modells
		if (neuerHersteller.empty()) {
			std::cerr << "No Manufacturer entered." << std::endl;
			return;
		}
		std::cout << "Please enter the Model";
		std::string neuesModell;
		std::getline(std::cin, neuesModell); // Eingabe des neuen Modells
		if (neuesModell.empty()) {
			std::cerr << "No Model entered." << std::endl;
			return;
		}




		image->readMetadata();
		Exiv2::ExifData& exifData = image->exifData();
		exifData["Exif.Image.Make"] = neuerHersteller;
		image->setExifData(exifData);
		image->writeMetadata();
		std::cout << "Camera-Model has changed successfully" << std::endl;
		image->readMetadata();

		exifData["Exif.Image.Model"] = neuesModell;
		image->setExifData(exifData);
		image->writeMetadata();
		std::cout << "Camera-Manufacturer has changed succesfully" << std::endl;
	}
	catch (const Exiv2::Error& e) {
		std::cerr << "Fehler: " << e.what() << std::endl;
	}
}


void ChangeGPSData(const std::string& filename) {
	try {
		std::unique_ptr<Exiv2::Image> image = Exiv2::ImageFactory::open(filename);
		if (!image) { // �berpr�fen, ob EXIF-Daten vorhanden sind
			std::cerr << "image not found." << std::endl;
			return;
		}
		Exiv2::ExifData& exifData = image->exifData();
		bool hatlongitude = exifData.findKey(Exiv2::ExifKey("Exif.GPSInfo.GPSLongitude")) != exifData.end();
		bool hatlatitude = exifData.findKey(Exiv2::ExifKey("Exif.GPSInfo.GPSLatitude")) != exifData.end();
		bool hataltitude = exifData.findKey(Exiv2::ExifKey("Exif.GPSInfo.GPSAltitude")) != exifData.end();

		image->readMetadata(); // Metadaten lesen
		std::cout << "\n--- Current GPS Data ---\n" << std::endl;
		if (hatlongitude == true) { std::cout << "Longitude: " << exifData["Exif.GPSInfo.GPSLongitude"] << std::endl; }
		if (hatlatitude == true) { std::cout << "Latitude: " << exifData["Exif.GPSInfo.GPSLatitude"] << std::endl; }
		if (hataltitude == true) { std::cout << "Altitude: " << exifData["Exif.GPSInfo.GPSAltitude"] << std::endl; }



		std::cout << "\nWhat would you like to change?\n";
		std::cout << " 1.Change individual values\n";
		std::cout << " 2.Change all values\n";
		std::cout << "Choice: ";
		std::string auswahl;
		std::getline(std::cin, auswahl);

		bool changeLongitude = false, changeLatitude = false, changeAltitude = false;

		if (auswahl == "2") {
			changeLongitude = changeLatitude = changeAltitude = true;
		}
		else if (auswahl == "1") {
			std::cout << "Change Longitude? (y/n): ";
			std::string input; std::getline(std::cin, input);
			if (input == "y" || input == "Y") changeLongitude = true;
			std::cout << "Change Latitude? (y/n): ";
			std::getline(std::cin, input);
			if (input == "y" || input == "Y") changeLatitude = true;
			std::cout << "Change Altitude? (y/n): ";
			std::getline(std::cin, input);
			if (input == "y" || input == "Y") changeAltitude = true;

		}
		else {
			std::cerr << "Invalid input." << std::endl;
			return;
		}

		if (changeLongitude) {
			std::cout << "Enter new Longitude (e.g. 48/1 7/1 30/1 for 48�07�30\"): ";
			std::string newLongitude;
			std::getline(std::cin, newLongitude);
			exifData["Exif.GPSInfo.GPSLongitude"] = newLongitude;
			image->setExifData(exifData);
			image->writeMetadata();
		}

		if (changeLatitude) {
			std::cout << "Enter new Latitude (e.g. 48/1 7/1 30/1 for 48�07�30\"): ";
			std::string newLatitude;
			std::getline(std::cin, newLatitude);
			exifData["Exif.GPSInfo.GPSLatitude"] = newLatitude;
			image->setExifData(exifData);
			image->writeMetadata();
		}
		if (changeAltitude) {
			std::cout << "Enter new Altitude (e.g. 100 for 100 meters): ";
			std::string newAltitude;
			std::getline(std::cin, newAltitude);
			exifData["Exif.GPSInfo.GPSAltitude"] = newAltitude + "/ 1";
			image->setExifData(exifData);
			image->writeMetadata();
		}


		std::cout << "GPS data updated successfully." << std::endl;
	}
	catch (const Exiv2::Error& e) {
		std::cerr << "Error " << e.what() << std::endl;
	}
}
void ChangeDateTime(const std::string& filename) {
	try {
		std::unique_ptr<Exiv2::Image> image = Exiv2::ImageFactory::open(filename);
		if (!image) {
			std::cerr << "Bild konnte nicht ge�ffnet werden: " << filename << std::endl;
			return;
		}

		image->readMetadata();
		Exiv2::ExifData& exifData = image->exifData();

		auto it = exifData.findKey(Exiv2::ExifKey("Exif.Image.DateTime"));
		if (it != exifData.end()) {
			std::cout << "Current DateTime: " << it->value() << std::endl;
		}
		else {
			std::cout << "No DateTime found in metadata.\n";
		}

		std::cout << "Enter new DateTime (YYYY:MM:DD HH:MM:SS): ";
		std::string newDateTime;
		std::getline(std::cin, newDateTime);

		exifData["Exif.Image.DateTime"] = newDateTime;
		image->setExifData(exifData);
		image->writeMetadata();

		std::cout << "DateTime updated successfully.\n";
	}
	catch (const Exiv2::Error& e) {
		std::cerr << "Exiv2 error: " << e.what() << std::endl;
	}
}



void ChangeOpticalData(const std::string& filename) {
	try {
		std::unique_ptr<Exiv2::Image> image = Exiv2::ImageFactory::open(filename);
		if (!image) { // �berpr�fen, ob EXIF-Daten vorhanden sind
			std::cerr << "image not found." << std::endl;
			return;
		}
		Exiv2::ExifData& exifData = image->exifData();
		bool hatBlende = exifData.findKey(Exiv2::ExifKey("Exif.Photo.FNumber")) != exifData.end();
		bool hatISO = exifData.findKey(Exiv2::ExifKey("Exif.Photo.ISOSpeedRatings")) != exifData.end();
		bool hatVerschlusszeit = exifData.findKey(Exiv2::ExifKey("Exif.Photo.ExposureTime")) != exifData.end();
		bool hatBrennweite = exifData.findKey(Exiv2::ExifKey("Exif.Photo.FocalLength")) != exifData.end();

		image->readMetadata(); // Metadaten lesen


		std::cout << "\nWhat would you like to change?\n";
		std::cout << " 1.Change individual values\n";
		std::cout << " 2.Change all values\n";
		std::cout << "Choice: ";
		std::string auswahl;
		std::getline(std::cin, auswahl);

		bool changeBlende = false, changeIso = false, changeVerschlusszeit = false, changeBrennweite = false;

		if (auswahl == "2") {
			changeBlende = changeIso = changeVerschlusszeit = changeBrennweite = true;
		}
		else if (auswahl == "1") {
			std::cout << "Change F Number? (y/n): ";
			std::string input; std::getline(std::cin, input);
			if (input == "y" || input == "Y") changeBlende = true;
			std::cout << "Change �SO? (y/n): ";
			std::getline(std::cin, input);
			if (input == "y" || input == "Y") changeIso = true;
			std::cout << "Change Shutterspeed? (y/n): ";
			std::getline(std::cin, input);
			if (input == "y" || input == "Y") changeVerschlusszeit = true;
			std::cout << "Change Focal Length? (y/n): ";
			std::getline(std::cin, input);
			if (input == "y" || input == "Y") changeBrennweite = true;

		}
		else {
			std::cerr << "Invalid input." << std::endl;
			return;
		}

		if (changeBlende) {
			std::cout << "Enter new F Number (e.g. 2.8 for f/2.8): ";
			std::string NewFNumber;
			std::getline(std::cin, NewFNumber);
			float fnumberFloat = std::stof(NewFNumber);
			exifData["Exif.Photo.FNumber"] = Exiv2::Rational(static_cast<int>(fnumberFloat * 10), 10);
		}

		if (changeIso) {
			std::cout << "Enter new ISO (e.g. 100): ";
			std::string newISO;
			std::getline(std::cin, newISO);
			exifData["Exif.Photo.ISOSpeedRatings"] = newISO;
		}
		if (changeVerschlusszeit) {
			std::cout << "Enter new Shutterspeed (e.g. 1/125 for 1/125 seconds): ";
			std::string newShutterspeed;
			std::getline(std::cin, newShutterspeed);
			exifData["Exif.Photo.ExposureTime"] = newShutterspeed;
		}
		if (changeBrennweite) {
			std::cout << "Enter new Focal Length (e.g. 50 for 50mm): ";
			std::string newFocalLength;
			std::getline(std::cin, newFocalLength);
			exifData["Exif.Photo.FocalLength"] = newFocalLength + "/1";
		}

		image->setExifData(exifData);
		image->writeMetadata(); // �nderungen speichern
		std::cout << "Optical data updated successfully." << std::endl;
	}
	catch (const Exiv2::Error& e) {
		std::cerr << "Error " << e.what() << std::endl;
	}
}

void DeleteAllExifData(const std::string& filename) {
	try {
		std::unique_ptr<Exiv2::Image> image = Exiv2::ImageFactory::open(filename);
		if (!image) {
			std::cerr << "Bild konnte nicht ge�ffnet werden: " << filename << std::endl;
			return;
		}
		image->readMetadata();
		Exiv2::ExifData& exifData = image->exifData();
		exifData.clear(); // Alle EXIF-Daten l�schen
		image->setExifData(exifData);
		image->writeMetadata(); // �nderungen speichern
		std::cout << "all exif data deleted successfully" << std::endl;
	}
	catch (const Exiv2::Error& e) {
		std::cerr << "Fehler: " << e.what() << std::endl;
	}
}

/*#include <iostream>
#include <string>
#include <exiv2/exiv2.hpp> // Einbinden der Exiv2-Bibliothek*

void showEXif(const std::string& filename) { //Exif-Daten anzeigen
	try {
		Exiv2::Image::AutoPtr image = Exiv2::ImageFactory::open(filename);
		image->readMetadata(); // Metadaten lesen
		Exiv2::ExifData &exifData = image->exifData(); // EXIF-Daten abrufen
		if (exifData.empty()) {
			std::cout << "Keine EXIF-Daten gefunden." << std::endl;
			return;
		}
		for (const auto &entry : exifData) {
			std::cout << entry.key() << " = " << entry.value() << std::endl; // Schl�ssel-Wert-Paare ausgeben
		}
	} catch (const Exiv2::Error& e) {
		std::cerr << "Fehler: " << e.what() << std::endl; // Fehlerbehandlung
	}
}

void changeCameraModel(const std::string& filename, const std::string& neuesModell) { //Kamera-Modell �ndern
	try {
		Exiv2::Image::AutoPtr image = Exiv2::ImageFactory::open(filename);
		image->readMetadata(); // Metadaten lesen
		Exiv2::ExifData &exifData = image->exifData(); // EXIF-Daten abrufen
		exifData["Exif.Image.Model"] = neuesModell; // Kamera-Modell �ndern
		image->setExifData(exifData); // �nderungen speichern
		image->writeMetadata(); // Metadaten schreiben
		std::cout << "Kamera-Modell ge�ndert." << std::endl;
	} catch (const Exiv2::Error& e) {
		std::cerr << "Fehler: " << e.what() << std::endl; // Fehlerbehandlung
	}
}
void changeMaker
(const std::string& filename, const std::string& neueMarke) { //Hersteller �ndern
	try {
		Exiv2::Image::AutoPtr image = Exiv2::ImageFactory::open(filename);
		image->readMetadata(); // Metadaten lesen
		Exiv2::ExifData &exifData = image->exifData(); // EXIF-Daten abrufen
		exifData["Exif.Image.Make"] = neueMarke; // Hersteller �ndern
		image->setExifData(exifData); // �nderungen speichern
		image->writeMetadata(); // Metadaten schreiben
		std::cout << "Hersteller ge�ndert." << std::endl;
	} catch (const Exiv2::Error& e) {
		std::cerr << "Fehler: " << e.what() << std::endl; // Fehlerbehandlung
	}
}
void changeSerialNumber
(const std::string& filename, const std::string& neueSeriennummer) { //Seriennummer �ndern
	try {
		Exiv2::Image::AutoPtr image = Exiv2::ImageFactory::open(filename);
		image->readMetadata(); // Metadaten lesen
		Exiv2::ExifData &exifData = image->exifData(); // EXIF-Daten abrufen
		exifData["Exif.Image.SerialNumber"] = neueSeriennummer; // Seriennummer �ndern
		image->setExifData(exifData); // �nderungen speichern
		image->writeMetadata(); // Metadaten schreiben
		std::cout << "Seriennummer ge�ndert." << std::endl;
	} catch (const Exiv2::Error& e) {
		std::cerr << "Fehler: " << e.what() << std::endl; // Fehlerbehandlung
	}
}

void changeExposureTime(const std::string& filename, const std::string& neueBelichtungszeit) { //Belichtungszeit �ndern
	try {
		Exiv2::Image::AutoPtr image = Exiv2::ImageFactory::open(filename);
		image->readMetadata(); // Metadaten lesen
		Exiv2::ExifData &exifData = image->exifData(); // EXIF-Daten abrufen
		exifData["Exif.Photo.ExposureTime"] = neueBelichtungszeit; // Belichtungszeit �ndern
		image->setExifData(exifData); // �nderungen speichern
		image->writeMetadata(); // Metadaten schreiben
		std::cout << "Belichtungszeit ge�ndert." << std::endl;
	} catch (const Exiv2::Error& e) {
		std::cerr << "Fehler: " << e.what() << std::endl; // Fehlerbehandlung
	}
}
void changeFNumber(const std::string& filename, const std::string& neueBlende) { //Blende �ndern
	try {
		Exiv2::Image::AutoPtr image = Exiv2::ImageFactory::open(filename);
		image->readMetadata(); // Metadaten lesen
		Exiv2::ExifData &exifData = image->exifData(); // EXIF-Daten abrufen
		exifData["Exif.Photo.FNumber"] = neueBlende; // Blende �ndern
		image->setExifData(exifData); // �nderungen speichern
		image->writeMetadata(); // Metadaten schreiben
		std::cout << "Blende ge�ndert." << std::endl;
	} catch (const Exiv2::Error& e) {
		std::cerr << "Fehler: " << e.what() << std::endl; // Fehlerbehandlung
	}
}
void changeISO(const std::string& filename, const std::string& neueISO) { //ISO �ndern
	try {
		Exiv2::Image::AutoPtr image = Exiv2::ImageFactory::open(filename);
		image->readMetadata(); // Metadaten lesen
		Exiv2::ExifData &exifData = image->exifData(); // EXIF-Daten abrufen
		exifData["Exif.Photo.ISOSpeedRatings"] = neueISO; // ISO �ndern
		image->setExifData(exifData); // �nderungen speichern
		image->writeMetadata(); // Metadaten schreiben
		std::cout << "ISO ge�ndert." << std::endl;
	} catch (const Exiv2::Error& e) {
		std::cerr << "Fehler: " << e.what() << std::endl; // Fehlerbehandlung
	}
}
void changeFlash(const std::string& filename, const std::string& neuerBlitz) { //Blitz �ndern
	try {
		Exiv2::Image::AutoPtr image = Exiv2::ImageFactory::open(filename);
		image->readMetadata(); // Metadaten lesen
		Exiv2::ExifData &exifData = image->exifData(); // EXIF-Daten abrufen
		exifData["Exif.Photo.Flash"] = neuerBlitz; // Blitz �ndern
		image->setExifData(exifData); // �nderungen speichern
		image->writeMetadata(); // Metadaten schreiben
		std::cout << "Blitz ge�ndert." << std::endl;
	} catch (const Exiv2::Error& e) {
		std::cerr << "Fehler: " << e.what() << std::endl; // Fehlerbehandlung
	}
}
void changeFocallength(const std::string& filename, const std::string& neueBrennweite) { //Brennweite �ndern
	try {
		Exiv2::Image::AutoPtr image = Exiv2::ImageFactory::open(filename);
		image->readMetadata(); // Metadaten lesen
		Exiv2::ExifData &exifData = image->exifData(); // EXIF-Daten abrufen
		exifData["Exif.Photo.FocalLength"] = neueBrennweite; // Brennweite �ndern
		image->setExifData(exifData); // �nderungen speichern
		image->writeMetadata(); // Metadaten schreiben
		std::cout << "Brennweite ge�ndert." << std::endl;
	} catch (const Exiv2::Error& e) {
		std::cerr << "Fehler: " << e.what() << std::endl; // Fehlerbehandlung
	}
}

void changeWhiteBalance(const std::string& filename, const std::string& neueWei�abgleich) { //Wei�abgleich �ndern
	try {
		Exiv2::Image::AutoPtr image = Exiv2::ImageFactory::open(filename);
		image->readMetadata(); // Metadaten lesen
		Exiv2::ExifData &exifData = image->exifData(); // EXIF-Daten abrufen
		exifData["Exif.Photo.WhiteBalance"] = neueWei�abgleich; // Wei�abgleich �ndern
		image->setExifData(exifData); // �nderungen speichern
		image->writeMetadata(); // Metadaten schreiben
		std::cout << "Wei�abgleich ge�ndert." << std::endl;
	} catch (const Exiv2::Error& e) {
		std::cerr << "Fehler: " << e.what() << std::endl; // Fehlerbehandlung
	}
}

void changeDateTime(const std::string& filename, const std::string& neuesDatumUhrzeit) { //Datum und Uhrzeit �ndern
	try {
		Exiv2::Image::AutoPtr image = Exiv2::ImageFactory::open(filename);
		image->readMetadata(); // Metadaten lesen
		Exiv2::ExifData& exifData = image->exifData(); // EXIF-Daten abrufen
		exifData["Exif.Image.DateTime"] = neuesDatumUhrzeit; // Datum und Uhrzeit �ndern
		image->setExifData(exifData); // �nderungen speichern
		image->writeMetadata(); // Metadaten schreiben
		std::cout << "Datum und Uhrzeit ge�ndert." << std::endl;
	}
	catch (const Exiv2::Error& e) {
		std::cerr << "Fehler: " << e.what() << std::endl; // Fehlerbehandlung
	}

}
void changeGPSlatitude(const std::string& filename, const std::string& neueGPSBreitengrad) { //GPS-Breitengrad �ndern
	try {
		Exiv2::Image::AutoPtr image = Exiv2::ImageFactory::open(filename);
		image->readMetadata(); // Metadaten lesen
		Exiv2::ExifData &exifData = image->exifData(); // EXIF-Daten abrufen
		exifData["Exif.GPSInfo.GPSLatitude"] = neueGPSBreitengrad; // GPS-Breitengrad �ndern
		image->setExifData(exifData); // �nderungen speichern
		image->writeMetadata(); // Metadaten schreiben
		std::cout << "GPS-Breitengrad ge�ndert." << std::endl;
	} catch (const Exiv2::Error& e) {
		std::cerr << "Fehler: " << e.what() << std::endl; // Fehlerbehandlung
	}
}
void changeGPSlongitude(const std::string& filename, const std::string& neueGPSL�ngengrad) { //GPS-L�ngengrad �ndern
	try {
		Exiv2::Image::AutoPtr image = Exiv2::ImageFactory::open(filename);
		image->readMetadata(); // Metadaten lesen
		Exiv2::ExifData &exifData = image->exifData(); // EXIF-Daten abrufen
		exifData["Exif.GPSInfo.GPSLongitude"] = neueGPSL�ngengrad; // GPS-L�ngengrad �ndern
		image->setExifData(exifData); // �nderungen speichern
		image->writeMetadata(); // Metadaten schreiben
		std::cout << "GPS-L�ngengrad ge�ndert." << std::endl;
	} catch (const Exiv2::Error& e) {
		std::cerr << "Fehler: " << e.what() << std::endl; // Fehlerbehandlung
	}
}

void changeGPSaltitude(const std::string& filename, const std::string& neueGPSH�he) { //GPS-H�he �ndern
	try {
		Exiv2::Image::AutoPtr image = Exiv2::ImageFactory::open(filename);
		image->readMetadata(); // Metadaten lesen
		Exiv2::ExifData &exifData = image->exifData(); // EXIF-Daten abrufen
		exifData["Exif.GPSInfo.GPSAltitude"] = neueGPSH�he; // GPS-H�he �ndern
		image->setExifData(exifData); // �nderungen speichern
		image->writeMetadata(); // Metadaten schreiben
		std::cout << "GPS-H�he ge�ndert." << std::endl;
	} catch (const Exiv2::Error& e) {
		std::cerr << "Fehler: " << e.what() << std::endl; // Fehlerbehandlung
	}
}
void changeGPSDateTime(const std::string& filename, const std::string& neuesGPSDatumUhrzeit) { //GPS-Datum und Uhrzeit �ndern
	try {
		Exiv2::Image::AutoPtr image = Exiv2::ImageFactory::open(filename);
		image->readMetadata(); // Metadaten lesen
		Exiv2::ExifData &exifData = image->exifData(); // EXIF-Daten abrufen
		exifData["Exif.GPSInfo.GPSDateTime"] = neuesGPSDatumUhrzeit; // GPS-Datum und Uhrzeit �ndern
		image->setExifData(exifData); // �nderungen speichern
		image->writeMetadata(); // Metadaten schreiben
		std::cout << "GPS-Datum und Uhrzeit ge�ndert." << std::endl;
	} catch (const Exiv2::Error& e) {
		std::cerr << "Fehler: " << e.what() << std::endl; // Fehlerbehandlung
	}
}

#include <iostream>
#include <string>
#include <exiv2/exiv2.hpp>

void readExifData(const std::string& filename) {
	try {
		// Bild �ffnen (gibt std::unique_ptr zur�ck)
		std::unique_ptr<Exiv2::Image> image = Exiv2::ImageFactory::open(filename);
		if (!image) {
			std::cerr << "Bild konnte nicht ge�ffnet werden: " << filename << std::endl;
			return;
		}

		// Metadaten lesen
		image->readMetadata();

		// EXIF-Daten abrufen
		Exiv2::ExifData& exifData = image->exifData();

		if (exifData.empty()) {
			std::cout << "Keine EXIF-Daten gefunden in " << filename << std::endl;
			return;
		}

		// Alle EXIF-Schl�ssel und Werte ausgeben
		for (const auto& entry : exifData) {
			std::cout << entry.key() << " = " << entry.value() << std::endl;
		}
	}
	catch (const Exiv2::Error& e) {
		std::cerr << "Fehler beim Lesen der EXIF-Daten: " << e.what() << std::endl;
	}
}*/
