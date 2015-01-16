/*
 * parroto.h
 *
 *  Created on: Jun 27, 2012
 *      Author: hadianeh
 */

#ifndef PARROTO_H_
#define PARROTO_H_

#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <iostream>
using namespace std;

#define MODULE_NAME_MAX_LEN 8
#define PARROTO_DATA_FILE_POSTFIX ".parroto.data"
#define PARROTO_DATA_FILE_POSTFIX_LEN 13



struct ParrotObserver {
	FILE* dataFile;

	enum {
		BOOL = 0,
		CHAR,
		INT,
		UNIT,
		FLOAT,
	};

	ParrotObserver() { dataFile = 0; }

	ParrotObserver(const char* moduleName) {
		if (!open(moduleName)) {
			assert(0);
			return;
		}
	}

	~ParrotObserver() {
		if (dataFile != NULL)
			fclose(dataFile);
	}

	bool open(const char* moduleName) {
		char dataFileName[MODULE_NAME_MAX_LEN + PARROTO_DATA_FILE_POSTFIX_LEN + 1];

		if (strlen(moduleName) >= MODULE_NAME_MAX_LEN) {
			strncpy(dataFileName, moduleName, MODULE_NAME_MAX_LEN);
			dataFileName[MODULE_NAME_MAX_LEN] = 0;
		} else {
			strcpy(dataFileName, moduleName);
		}
		strcat(dataFileName, ".parroto.data");

		dataFile = fopen(dataFileName, "wb");

		return (dataFile != NULL);
	}

	void write(const char* parrotName, char inOut, bool value, double rangeA, double rangeB) {
		int parrotNameLen = strlen(parrotName);
		char type = BOOL;

		fwrite(&parrotNameLen, sizeof(parrotNameLen), 1, dataFile);
		fwrite(parrotName, sizeof(parrotName[0]), parrotNameLen, dataFile);

		fwrite(&inOut, sizeof(inOut), 1, dataFile);

		fwrite(&type, sizeof(type), 1, dataFile);

		fwrite(&value, sizeof(value), 1, dataFile);

		fwrite(&rangeA, sizeof(rangeA), 1, dataFile);
		fwrite(&rangeB, sizeof(rangeB), 1, dataFile);
	}

	void write(const char* parrotName, char inOut, char value, double rangeA, double rangeB) {
		int parrotNameLen = strlen(parrotName);
		char type = CHAR;

		fwrite(&parrotNameLen, sizeof(parrotNameLen), 1, dataFile);
		fwrite(parrotName, sizeof(parrotName[0]), parrotNameLen, dataFile);

		fwrite(&inOut, sizeof(inOut), 1, dataFile);

		fwrite(&type, sizeof(type), 1, dataFile);

		fwrite(&value, sizeof(value), 1, dataFile);

		fwrite(&rangeA, sizeof(rangeA), 1, dataFile);
		fwrite(&rangeB, sizeof(rangeB), 1, dataFile);
	}

	void write(const char* parrotName, char inOut, int value, double rangeA, double rangeB) {
		int parrotNameLen = strlen(parrotName);
		char type = INT;

		fwrite(&parrotNameLen, sizeof(parrotNameLen), 1, dataFile);
		fwrite(parrotName, sizeof(parrotName[0]), parrotNameLen, dataFile);

		fwrite(&inOut, sizeof(inOut), 1, dataFile);

		fwrite(&type, sizeof(type), 1, dataFile);

		fwrite(&value, sizeof(value), 1, dataFile);

		fwrite(&rangeA, sizeof(rangeA), 1, dataFile);
		fwrite(&rangeB, sizeof(rangeB), 1, dataFile);
	}

	void write(const char* parrotName, char inOut, unsigned value, double rangeA, double rangeB) {
		int parrotNameLen = strlen(parrotName);
		char type = UNIT;

		fwrite(&parrotNameLen, sizeof(parrotNameLen), 1, dataFile);
		fwrite(parrotName, sizeof(parrotName[0]), parrotNameLen, dataFile);

		fwrite(&inOut, sizeof(inOut), 1, dataFile);

		fwrite(&type, sizeof(type), 1, dataFile);

		fwrite(&value, sizeof(value), 1, dataFile);

		fwrite(&rangeA, sizeof(rangeA), 1, dataFile);
		fwrite(&rangeB, sizeof(rangeB), 1, dataFile);
	}

	void write(const char* parrotName, char inOut, double value, double rangeA, double rangeB) {
		int parrotNameLen = strlen(parrotName);
		char type = FLOAT;

		fwrite(&parrotNameLen, sizeof(parrotNameLen), 1, dataFile);
		fwrite(parrotName, sizeof(parrotName[0]), parrotNameLen, dataFile);

		fwrite(&inOut, sizeof(inOut), 1, dataFile);

		fwrite(&type, sizeof(type), 1, dataFile);

		fwrite(&value, sizeof(value), 1, dataFile);
        fread(&value, sizeof(value), 1, dataFile);
        //double *val = &value;
        //cout << *val << "printing";

		fwrite(&rangeA, sizeof(rangeA), 1, dataFile);
		fwrite(&rangeB, sizeof(rangeB), 1, dataFile);
	}

	void write(const char* parrotName, char inOut, bool* value, int len, bool rangeA, bool rangeB) {
		int parrotNameLen = strlen(parrotName);
		char type = BOOL;

		fwrite(&parrotNameLen, sizeof(parrotNameLen), 1, dataFile);
		fwrite(parrotName, sizeof(parrotName[0]), parrotNameLen, dataFile);

		fwrite(&inOut, sizeof(inOut), 1, dataFile);

		fwrite(&type, sizeof(type), 1, dataFile);

		fwrite(value, sizeof(value), len, dataFile);

		fwrite(&rangeA, sizeof(rangeA), 1, dataFile);
		fwrite(&rangeB, sizeof(rangeB), 1, dataFile);
	}

	void write(const char* parrotName, char inOut, char* value, int len, char rangeA, char rangeB) {
		int parrotNameLen = strlen(parrotName);
		char type = CHAR;

		fwrite(&parrotNameLen, sizeof(parrotNameLen), 1, dataFile);
		fwrite(parrotName, sizeof(parrotName[0]), parrotNameLen, dataFile);

		fwrite(&inOut, sizeof(inOut), 1, dataFile);

		fwrite(&type, sizeof(type), 1, dataFile);

		fwrite(value, sizeof(value), len, dataFile);

		fwrite(&rangeA, sizeof(rangeA), 1, dataFile);
		fwrite(&rangeB, sizeof(rangeB), 1, dataFile);
	}

	void write(const char* parrotName, char inOut, int* value, int len, int rangeA, int rangeB) {
		int parrotNameLen = strlen(parrotName);
		char type = INT;

		fwrite(&parrotNameLen, sizeof(parrotNameLen), 1, dataFile);
		fwrite(parrotName, sizeof(parrotName[0]), parrotNameLen, dataFile);

		fwrite(&inOut, sizeof(inOut), 1, dataFile);

		fwrite(&type, sizeof(type), 1, dataFile);

		fwrite(value, sizeof(value), len, dataFile);

		fwrite(&rangeA, sizeof(rangeA), 1, dataFile);
		fwrite(&rangeB, sizeof(rangeB), 1, dataFile);
	}

	void write(const char* parrotName, char inOut, unsigned* value, int len, unsigned rangeA, unsigned rangeB) {
		int parrotNameLen = strlen(parrotName);
		char type = UNIT;

		fwrite(&parrotNameLen, sizeof(parrotNameLen), 1, dataFile);
		fwrite(parrotName, sizeof(parrotName[0]), parrotNameLen, dataFile);

		fwrite(&inOut, sizeof(inOut), 1, dataFile);

		fwrite(&type, sizeof(type), 1, dataFile);

		fwrite(value, sizeof(value), len, dataFile);

		fwrite(&rangeA, sizeof(rangeA), 1, dataFile);
		fwrite(&rangeB, sizeof(rangeB), 1, dataFile);
	}

	void write(const char* parrotName, char inOut, double* value, int len, double rangeA, double rangeB) {
		int parrotNameLen = strlen(parrotName);
		char type = FLOAT;

		fwrite(&parrotNameLen, sizeof(parrotNameLen), 1, dataFile);
		fwrite(parrotName, sizeof(parrotName[0]), parrotNameLen, dataFile);

		fwrite(&inOut, sizeof(inOut), 1, dataFile);

		fwrite(&type, sizeof(type), 1, dataFile);

		fwrite(value, sizeof(value), len, dataFile);

		fwrite(&rangeA, sizeof(rangeA), 1, dataFile);
		fwrite(&rangeB, sizeof(rangeB), 1, dataFile);
	}
};


#endif /* PARROTO_H_ */
