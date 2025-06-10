#include <iostream>
#include <string>
#include <memory>              // Für std::unique_ptr
#include <exiv2/exiv2.hpp>  
#include "ConsoleMenue.h"// Exiv2-Bibliothek einbinden

void showEXif(const std::string& filename) {
	try {
		std::unique_ptr<Exiv2::Image> image = Exiv2::ImageFactory::open(filename);
		if (!image) {
			std::cerr << ConsoleUtils::getText("exiv2_open_error") << filename << std::endl;
			return;
		}

		image->readMetadata();
		Exiv2::ExifData& exifData = image->exifData();

		if (exifData.empty()) {
			std::cout << ConsoleUtils::getText("no_exif_data") << filename << std::endl;
			return;
		}

		for (const auto& entry : exifData) {
			std::cout << entry.key() << " = " << entry.value() << std::endl;
		}
	}
	catch (const Exiv2::Error& e) {
		std::cerr << ConsoleUtils::getText("exiv2_read_error") << filename << ": " << e.what() << std::endl;
	}
	catch (const std::exception& e) {
		std::cerr << ConsoleUtils::getText("general_read_error") << filename << ": " << e.what() << std::endl;
	}
}


void ChangeCameraModel(const std::string& filename) {
	try {
		std::unique_ptr<Exiv2::Image> image = Exiv2::ImageFactory::open(filename);

		if (!image) 
		{
			std::cerr << ConsoleUtils::getText("exiv2_open_error") << filename << std::endl;
			return;
		}

		image->readMetadata();
		Exiv2::ExifData& exifData = image->exifData();

		std::cout << ConsoleUtils::getText("enter_manufacturer");
		std::string neuerHersteller;
		std::getline(std::cin, neuerHersteller); // Eingabe des neuen Modells

		if (neuerHersteller.empty()) 
		{
			std::cerr << ConsoleUtils::getText("no_manufacturer_entered") << std::endl;
			return;
		}

		std::cout << ConsoleUtils::getText("enter_model");
		std::string neuesModell;
		std::getline(std::cin, neuesModell); // Eingabe des neuen Modells
		if (neuesModell.empty()) {
			std::cerr << ConsoleUtils::getText("no_model_entered") << std::endl;
			return;
		}

		
		exifData["Exif.Image.Make"] = neuerHersteller;
		exifData["Exif.Image.Model"] = neuesModell;

		image->setExifData(exifData);
		image->writeMetadata();
		std::cout << ConsoleUtils::getText("camera_model_changed_success") << std::endl;

	}
	catch (const Exiv2::Error& e) {
		std::cerr << ConsoleUtils::getText("exiv2_error") << ": " << e.what() << std::endl;
	}
	catch (const std::exception& e) {
		std::cerr << ConsoleUtils::getText("general_error") << ": " << e.what() << std::endl;
	}
}


void ChangeGPSData(const std::string& filename) {
	try {
		std::unique_ptr<Exiv2::Image> image = Exiv2::ImageFactory::open(filename);
		if (!image) { // Überprüfen, ob EXIF-Daten vorhanden sind
			std::cerr << ConsoleUtils::getText("exiv2_open_error") << filename << std::endl;
			return;
		}

		image->readMetadata(); // Metadaten lesen
		Exiv2::ExifData& exifData = image->exifData();

		std::cout << std::endl << ConsoleUtils::getText("current_gps_data") << std::endl;
		bool hasLongitude = exifData.findKey(Exiv2::ExifKey("Exif.GPSInfo.GPSLongitude")) != exifData.end();
		bool hasLatitude = exifData.findKey(Exiv2::ExifKey("Exif.GPSInfo.GPSLatitude")) != exifData.end();
		bool hasAltitude = exifData.findKey(Exiv2::ExifKey("Exif.GPSInfo.GPSAltitude")) != exifData.end();

		if (hasLongitude) { std::cout << ConsoleUtils::getText("longitude") << ": " << exifData["Exif.GPSInfo.GPSLongitude"] << std::endl; }
		if (hasLatitude) { std::cout << ConsoleUtils::getText("latitude") << ": " << exifData["Exif.GPSInfo.GPSLatitude"] << std::endl; }
		if (hasAltitude) { std::cout << ConsoleUtils::getText("altitude") << ": " << exifData["Exif.GPSInfo.GPSAltitude"] << std::endl; }
		if (!hasLongitude && !hasLatitude && !hasAltitude) { std::cout << ConsoleUtils::getText("no_gps_data_found") << std::endl; }

		std::cout << "\n" << ConsoleUtils::getText("what_to_change_gps") << "\n";
		std::cout << " 1. " << ConsoleUtils::getText("change_individual_values") << "\n";
		std::cout << " 2. " << ConsoleUtils::getText("change_all_values_gps") << "\n";
		std::cout << ConsoleUtils::getText("choice") << ": ";
		std::string auswahl;
		std::getline(std::cin, auswahl);

		bool changeLongitude = false, changeLatitude = false, changeAltitude = false;

		if (auswahl == "2") {
			changeLongitude = changeLatitude = changeAltitude = true;
		}
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
		}
		else {
			std::cerr << ConsoleUtils::getText("invalid_input") << std::endl;
			return;
		}

		if (changeLongitude) {
			std::cout << ConsoleUtils::getText("enter_new_longitude");
			std::string newLongitude;
			std::getline(std::cin, newLongitude);
			exifData["Exif.GPSInfo.GPSLongitude"] = newLongitude;
		}

		if (changeLatitude) {
			std::cout << ConsoleUtils::getText("enter_new_latitude");
			std::string newLatitude;
			std::getline(std::cin, newLatitude);
			exifData["Exif.GPSInfo.GPSLatitude"] = newLatitude;
		}
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
		}

		image->setExifData(exifData);
		image->writeMetadata();
		std::cout << ConsoleUtils::getText("gps_data_updated_success") << std::endl;

	}
	catch (const Exiv2::Error& e) {
		std::cerr << ConsoleUtils::getText("exiv2_error") << ": " << e.what() << std::endl;
	}
	catch (const std::exception& e) {
		std::cerr << ConsoleUtils::getText("general_error") << ": " << e.what() << std::endl;
	}
}

void ChangeDateTime(const std::string& filename) {
	try {
		std::unique_ptr<Exiv2::Image> image = Exiv2::ImageFactory::open(filename);
		if (!image) {
			std::cerr << ConsoleUtils::getText("exiv2_open_error") << filename << std::endl;
			return;
		}

		image->readMetadata();
		Exiv2::ExifData& exifData = image->exifData();

		auto it = exifData.findKey(Exiv2::ExifKey("Exif.Image.DateTime"));
		if (it != exifData.end()) {
			std::cout << ConsoleUtils::getText("current_datetime") << ": " << it->value() << std::endl;
		}
		else {
			std::cout << ConsoleUtils::getText("no_datetime_found") << "\n";
		}

		std::cout << ConsoleUtils::getText("enter_new_datetime");
		std::string newDateTime;
		std::getline(std::cin, newDateTime);

		if (newDateTime.length() != 19 || newDateTime[4] != ':' || newDateTime[7] != ':' || newDateTime[10] != ' ' || newDateTime[13] != ':' || newDateTime[16] != ':') {
			std::cerr << ConsoleUtils::getText("invalid_datetime_format") << std::endl;
			return;
		}

		exifData["Exif.Image.DateTime"] = newDateTime;
		image->setExifData(exifData);
		image->writeMetadata();

		std::cout << ConsoleUtils::getText("datetime_updated_success") << "\n";
	}
	catch (const Exiv2::Error& e) {
		std::cerr << ConsoleUtils::getText("exiv2_error") << ": " << e.what() << std::endl;
	}
	catch (const std::exception& e) {
		std::cerr << ConsoleUtils::getText("general_error") << ": " << e.what() << std::endl;
	}
}

void ChangeOpticalData(const std::string& filename) {
	try {
		std::unique_ptr<Exiv2::Image> image = Exiv2::ImageFactory::open(filename);
		if (!image) { // Überprüfen, ob EXIF-Daten vorhanden sind
			std::cerr << ConsoleUtils::getText("exiv2_open_error") << filename << std::endl;
			return;
		}

		image->readMetadata(); // Metadaten lesen
		Exiv2::ExifData& exifData = image->exifData();

		std::cout << std::endl << ConsoleUtils::getText("current_optical_data") << std::endl;

		bool hatBlende = exifData.findKey(Exiv2::ExifKey("Exif.Photo.FNumber")) != exifData.end();
		bool hatISO = exifData.findKey(Exiv2::ExifKey("Exif.Photo.ISOSpeedRatings")) != exifData.end();
		bool hatVerschlusszeit = exifData.findKey(Exiv2::ExifKey("Exif.Photo.ExposureTime")) != exifData.end();
		bool hatBrennweite = exifData.findKey(Exiv2::ExifKey("Exif.Photo.FocalLength")) != exifData.end();



		std::cout << "\n" << ConsoleUtils::getText("what_to_change_optical") << "\n";
		std::cout << " 1. " << ConsoleUtils::getText("change_individual_values") << "\n";
		std::cout << " 2. " << ConsoleUtils::getText("change_all_values_optical") << "\n";
		std::cout << ConsoleUtils::getText("choice") << ": ";
		std::string auswahl;
		std::getline(std::cin, auswahl);

		bool changeBlende = false, changeIso = false, changeVerschlusszeit = false, changeBrennweite = false;

		if (auswahl == "2") {
			changeBlende = changeIso = changeVerschlusszeit = changeBrennweite = true;
		}
		else if (auswahl == "1") {
			std::cout << ConsoleUtils::getText("change_f_number") << " (y/n): ";
			std::string input; std::getline(std::cin, input);
			if (input == "y" || input == "Y") changeBlende = true;
			std::cout << ConsoleUtils::getText("change_iso") << " (y/n): ";
			std::getline(std::cin, input);
			if (input == "y" || input == "Y") changeIso = true;
			std::cout << ConsoleUtils::getText("change_shutterspeed") << " (y/n): ";
			std::getline(std::cin, input);
			if (input == "y" || input == "Y") changeVerschlusszeit = true;
			std::cout << ConsoleUtils::getText("change_focal_length") << " (y/n): ";
			std::getline(std::cin, input);
			if (input == "y" || input == "Y") changeBrennweite = true;

		}
		else {
			std::cerr << ConsoleUtils::getText("invalid_input") << std::endl;
			return;
		}

		if (changeBlende) {
			std::cout << ConsoleUtils::getText("enter_new_f_number");
			std::string NewFNumber;
			std::getline(std::cin, NewFNumber);
			try {
				float fnumberFloat = std::stof(NewFNumber);
				exifData["Exif.Photo.FNumber"] = Exiv2::Rational(static_cast<long>(fnumberFloat * 100), 100);
			}
			catch (const std::exception& e) {
				std::cerr << ConsoleUtils::getText("invalid_number_format") << NewFNumber << ": " << e.what() << std::endl;
			}
		}

		if (changeIso) {
			std::cout << ConsoleUtils::getText("enter_new_iso");
			std::string newISO;
			std::getline(std::cin, newISO);
			try {
				int isoVal = std::stoi(newISO);
				exifData["Exif.Photo.ISOSpeedRatings"] = isoVal;
			}
			catch (const std::exception& e) {
				std::cerr << ConsoleUtils::getText("invalid_number_format") << newISO << ": " << e.what() << std::endl;
			}
		}
		if (changeVerschlusszeit) {
			std::cout << ConsoleUtils::getText("enter_new_shutterspeed");
			std::string newShutterspeed;
			std::getline(std::cin, newShutterspeed);
			exifData["Exif.Photo.ExposureTime"] = newShutterspeed;
		}
		if (changeBrennweite) {
			std::cout << ConsoleUtils::getText("enter_new_focal_length");
			std::string newFocalLength;
			std::getline(std::cin, newFocalLength);
			try {
				float focalLengthFloat = std::stof(newFocalLength);
				exifData["Exif.Photo.FocalLength"] = Exiv2::Rational(static_cast<long>(focalLengthFloat * 100), 100);
			}
			catch (const std::exception& e) {
				std::cerr << ConsoleUtils::getText("invalid_number_format") << newFocalLength << ": " << e.what() << std::endl;
			}
		}

		image->setExifData(exifData);
		image->writeMetadata(); // Änderungen speichern
		std::cout << ConsoleUtils::getText("optical_data_updated_success") << std::endl;
	}
	catch (const Exiv2::Error& e) {
		std::cerr << ConsoleUtils::getText("exiv2_error") << ": " << e.what() << std::endl;
	}
	catch (const std::exception& e) {
		std::cerr << ConsoleUtils::getText("general_error") << ": " << e.what() << std::endl;
	}
}

void DeleteAllExifData(const std::string& filename) {
	try {
		std::unique_ptr<Exiv2::Image> image = Exiv2::ImageFactory::open(filename);
		if (!image) {
			std::cerr << ConsoleUtils::getText("exiv2_open_error") << filename << std::endl;
			return;
		}
		image->readMetadata();
		Exiv2::ExifData& exifData = image->exifData();
		exifData.clear(); // Alle EXIF-Daten löschen
		image->setExifData(exifData);
		image->writeMetadata(); // Änderungen speichern
		std::cout << ConsoleUtils::getText("all_exif_data_deleted_success") << std::endl;
	}
	catch (const Exiv2::Error& e) {
		std::cerr << ConsoleUtils::getText("exiv2_error") << ": " << e.what() << std::endl;
	}
	catch (const std::exception& e) {
		std::cerr << ConsoleUtils::getText("general_error") << ": " << e.what() << std::endl;
	}
}
