#pragma once
// exifTools.h
#ifndef AUSLESEN_SCHREIBEN_H
#define AUSLESEN_SCHREIBEN_H

#include <string>

void showEXif(const std::string& filename);
void ChangeCameraModel(const std::string& filename);
void ChangeGPSData(const std::string& filename);
void ChangeDateTime(const std::string& filename);
void ChangeOpticalData(const std::string& filename);
void DeleteAllExifData(const std::string& filename);
#endif