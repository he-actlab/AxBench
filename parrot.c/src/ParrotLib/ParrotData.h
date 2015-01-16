/*
 * ParrotData.h
 *
 *  Created on: Aug 3, 2012
 *      Author: hadianeh
 */

#ifndef PARROTDATA_H_
#define PARROTDATA_H_

#include <vector>
#include <iostream>
#include "ParrotVector.h"

using namespace std;


class ParrotData {
	vector<ParrotVector> data;

public:
	ParrotData() {
		return;
	}

	virtual ~ParrotData() {
		return;
	}
};

#endif /* PARROTDATA_H_ */
