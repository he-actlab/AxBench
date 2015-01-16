#include "fann.h"
#include "parroto.h"
extern ParrotObserver parroto;

/*
 * kmeans.c
 *
 * Created on: Sep 9, 2013
 * 			Author: Amir Yazdanbakhsh <a.yazdanbakhsh@gatech.edu>
 */


#include <stdio.h>
#include <string>
#include "rgbimage.h"
#include "segmentation.h"
#include <iostream>
#include <fstream>
#include <sstream>

int main (int argc, const char* argv[]) {

	RgbImage srcImage;
	Clusters clusters;

	initRgbImage(&srcImage);

	std::string inImageName  = argv[1];
	std::string outImageName = argv[2];

	loadRgbImage(inImageName.c_str(), &srcImage, 256);

	initClusters(&clusters, 6, 1);
	segmentImage(&srcImage, &clusters, 1);

	saveRgbImage(&srcImage, outImageName.c_str(), 255);


	freeRgbImage(&srcImage);
	freeClusters(&clusters);
	return 0;
}

