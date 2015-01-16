/*
 * distance.c
 * 
 * Created on: Sep 9, 2013
 * 			Author: Amir Yazdanbakhsh <a.yazdanbakhsh@gatech.edu>
 */


#include "distance.h"
#include <math.h>
#include <map>

int count = 0;
#define MAX_COUNT 1200000

float euclideanDistance(RgbPixel* p, Centroid* c1) {
	float r;

	r = 0;
	double r_tmp;
	
	double dataIn[6];
	dataIn[0] = p->r;
	dataIn[1] = p->g;
	dataIn[2] = p->b;
	dataIn[3] = c1->r;
	dataIn[4] = c1->g;
	dataIn[5] = c1->b;

#pragma parrot(input, "kmeans", [6]dataIn)

	r += (p->r - c1->r) * (p->r - c1->r);
	r += (p->g - c1->g) * (p->g - c1->g);
	r += (p->b - c1->b) * (p->b - c1->b);

	r_tmp = sqrt(r);

#pragma parrot(output, "kmeans", <0.0; 1.0>r_tmp)

	return r_tmp;
}

int pickCluster(RgbPixel* p, Centroid* c1) {
	float d1;

	d1 = euclideanDistance(p, c1);

	if (p->distance <= d1)
		return 0;

	return 1;
}

void assignCluster(RgbPixel* p, Clusters* clusters) {
	int i = 0;

	float d;
	d = euclideanDistance(p, &clusters->centroids[i]);
	p->distance = d;

	for(i = 1; i < clusters->k; ++i) {
		d = euclideanDistance(p, &clusters->centroids[i]);
		if (d < p->distance) {
			p->cluster = i;
			p->distance = d;
		}
	}
}

