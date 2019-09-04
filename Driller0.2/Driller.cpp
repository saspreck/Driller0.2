// Lab1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <sstream>
#include <string>
#include <set>
#include <array>
#include <algorithm>
#include <iomanip>

/*
Predefined struct that holds that data for each record
*/
struct drillingRecord {
	double nums[16];   // holds the numeric data, in column order  
	std::string strings[2];  // holds the string data, in column order 
};

/*
Predefined struct that holds the size of the array
*/
struct drillingArray {
	int capacity;    // maximum capacity, in records  
	drillingRecord* data = NULL; // pointer to array of records 
};

/*
Function to double the size of the array used to store the drilling data
*/
drillingArray* doubleDrillingArray(drillingArray* currentDrillingArray) {

	//doubles capacity for new drilling array
	int newSize = currentDrillingArray->capacity * 2;
	
	//declares new drilling array and sets the capacity
	drillingArray* newDrillingArray = new drillingArray;
	newDrillingArray->capacity = newSize;

	//creates a new array of drilling records
	drillingRecord* doubledDrillingRecord = new drillingRecord[newSize];
	
	//copies data from the old drilling record array to the new one
	for (int i = 0; i < currentDrillingArray->capacity; i++) {
		doubledDrillingRecord[i] = currentDrillingArray->data[i];
	}
	
	//adds drilling record array to the new drilling array
	newDrillingArray->data = doubledDrillingRecord;

	//deletes the current array and frees the memory
	delete currentDrillingArray;
	currentDrillingArray = nullptr;

	return newDrillingArray;
}

/*
Function to check if the date stamp matches the correct one
*/
bool checkDateStamp(std::string dataLine, std::string dateStamp, int lineNum) {
	//creates a substring that contains only the date
	std::string dateCheck = dataLine.substr(0, dataLine.find(','));
	//checks if the substring doesn't match the correct one
	if (dateCheck != dateStamp) {
		//prints out the error message
		std::cout << "Non-matching date stamp " << dateCheck << " at line " << lineNum << "." << std::endl;
		return false;
	}
	else {
		return true;
	}

}

/*
Function to check if the time stamp is unique
*/
bool checkTimeStamp(std::string dataLine, std::set<std::string>& timeStamps, int lineNum) {

	//gets the initial size of the set
	int timeStampStartSize = (int)timeStamps.size();

	//makes a substring of everything except the date
	std::string dataLineMinusDate = dataLine.substr(dataLine.find(',') + 1);

	//makes a substring of the time stamp
	std::string timeStampToCheck = dataLineMinusDate.substr(0, dataLineMinusDate.find(','));

	//attempts to insert the time stamp into the set
	timeStamps.insert(timeStampToCheck);

	//if the size of the set did not increase then the time stamp was not unique
	if (timeStampStartSize == (int)timeStamps.size()) {

		//prints out error message
		std::cout << "Duplicate timestamp " << timeStampToCheck << " at line " << lineNum << "." << std::endl;
		return false;
	}
	else {
		return true;
	}

}

/*
Function to check that all the float values are positive
*/
bool checkFloatValues(std::string dataLine, int lineNum) {
	//declare variables
	std::string tempData;
	float floatData;

	//turn input string into a stringstream
	std::stringstream stringData(dataLine);

	//ignores the date and time stamps
	stringData.ignore(256, ',');
	stringData.ignore(256, ',');

	//iterates through each data point checking
	for (int i = 0; i < 16; i++) {
		//splits each piece of data off from the stringstream and turns it into a float
		std::getline(stringData, tempData, ',');
		floatData = std::stof(tempData);

		//checks if the data is positive and throws an error otherwise
		if (floatData <= 0) {
			std::cout << "Invalid floating-point data at line " << lineNum << "." << std::endl;
			return false;
		}
	}
	//returns true if all the data values are positive
	return true;
}

int main()
{
	//declare variables
	int dataLineNum = 1;
	std::string initialDateStamp = "";
	int validLines = 0;

	//declare set to hold all time stamps
	std::set<std::string> timeStamps;

	//declare drillingArray and drillingRecord
	drillingArray* mainDrillingArray = new drillingArray;
	drillingRecord* mainDrillingRecord = new drillingRecord[10];

	//set drillingArray data and capacity
	mainDrillingArray->data = mainDrillingRecord;
	mainDrillingArray->capacity = 10;

	//declares variable to store file line in and then skips the first line
	std::string line = "";
	std::cin.ignore(1000, '\n');

	//while loop to iterate through the file collecting data line by line
	while (std::getline(std::cin, line)) {

		//checks if the array has reached capacity and calls function to double it if need be
		if (validLines >= mainDrillingArray->capacity) {

			mainDrillingArray = doubleDrillingArray(mainDrillingArray);
		}
		
		//assign date stamp
		if (initialDateStamp == "") {
			initialDateStamp = line.substr(0, line.find(','));
		}

		//calls the checkDateStamp function to ensure date stamps are correct
		if (checkDateStamp(line, initialDateStamp, dataLineNum)) {

			//calls the checkTimeStamp function to ensure time stamps are unique
			if (checkTimeStamp(line, timeStamps, dataLineNum)) {

				//calls the checkFloatValues method to ensure they are positive
				if (checkFloatValues(line, dataLineNum)) {

					//declares a drilling record to use and a string stream
					drillingRecord currentRecord;
					std::stringstream stringData(line);


					//splices off the date and time stamp and puts them into the appropriate drilling record array
					for (int i = 0; i < 2; i++) {
						std::string tempData = "";
						std::getline(stringData, tempData, ',');
						currentRecord.strings[i] = tempData;
						
					}
					
					//splices off each piece of data, turns it to a float, and then puts it into the correct array
					for (int i = 0; i < 16; i++) {
						std::string tempData = "";
						std::getline(stringData, tempData, ',');
						float floatData = std::stof(tempData);
						currentRecord.nums[i] = floatData;
											
					}

					//adds the current drilling record to the drilling array and increments the number of valid lines
					mainDrillingArray->data[validLines] = currentRecord;
					validLines++;
					
				}
			}
		}
		//increases the line number
		dataLineNum++;
	}
	
	//iterates through and prints out data
	for (int i = validLines - 1; i >= 0; i--) {

		//iterates through and prints out the date and time stamps
		for (int j = 0; j < 2; j++) {
			std::cout << mainDrillingArray->data[i].strings[j] << ";";
		}

		//iterates through and prints out the floating point data
		for (int k = 0; k < 16; k++) {
			if (k != 15) {
				std::cout << std::fixed << std::setprecision(2) << mainDrillingArray->data[i].nums[k] << ";";
			}
			else {
				std::cout << std::fixed << std::setprecision(2) << mainDrillingArray->data[i].nums[k] << std::endl;
			}
		}
	}
	
	return 0;

}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
