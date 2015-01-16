/*
 * FannData.h
 *
 *  Created on: Aug 3, 2012
 *      Author: hadianeh
 */

#ifndef FANNDATA_H_
#define FANNDATA_H_

#include <fstream>
#include <iostream>

using namespace std;

class FannData {
	int n;
	int nX;
	int nY;
	float* data;

public:
	FannData() {
		n = 0;
		nX = 0;
		nY = 0;

		data = 0;
	}

	FannData(int n, int nX, int nY) {
		setup(n, nX, nY);
	}

	bool setup(int n, int nX, int nY) {
		this->n = n;
		this->nX = nX;
		this->nY = nY;

		data = new float[n * (nX + nY)];

		if (data == 0) {
			this->n = 0;
			this->nX = 0;
			this->nY = 0;

			return false;
		}

		return true;
	}

	virtual ~FannData() {
		if (data != 0)
			delete[] data;
	}

	bool load(char* path) {
		ifstream in;
		int n, nX, nY;
		int i, j;
		float v;

		in.open(path);

		//TODO: if ()
		in >> n >> nX >> nY;

		if (!setup(n, nX, nY))
			return false;

		//cout << n << '\n';
		//cout << nX << '\n';
		//cout << nY << '\n';
		for (i = 0; i < this->n; ++i) {
			for (j = 0; j < this->nX; ++j) {
				in >> v;
				data[i * (this->nX + this->nY) + j] = v;
				//cout << v << ' ' << data[i * (this->nX + this->nY) + j] << ' ';
			}
			//cout << '\n';
			for (j = 0; j < this->nY; ++j) {
				in >> v;
				data[i * (this->nX + this->nY) + this->nX + j] = v;
				//cout << v << ' ' << data[i * (this->nX + this->nY) + this->nX + j] << ' ';
			}
			//cout << '\n';
		}

		return true;
	}

	int getN() {
		return n;
	}

	int getNX() {
		return nX;
	}

	int getNY() {
		return nY;
	}

	float* getData() {
		return data;
	}

	void applyFn(void (*f)(float*, float*)) {
		int i;
		int k;
		float* x;
		float* y;

		for (
			i = 0, x = data, y = data + nX, k = nX + nY;
			i < n;
			++i, x += k, y += k
		) {
			f(x, y);
		}
	}

	void dump(ostream& out) {
		int i;
		int j;

		out << n << '\n';
		out << nX << '\n';
		out << nY << '\n';

		out.precision(6);
		for (i = 0; i < n; ++i) {
			for(j = 0; j < nX - 1; ++j)
				out << fixed << data[i * (nX + nY) + j] << ' ';
			out << fixed << data[i * (nX + nY) + j] << '\n';
			for(j = 0; j < nY - 1; ++j)
				out << fixed << data[i * (nX + nY) + nX + j] << ' ';
			out << fixed << data[i * (nX + nY) + nX + j] << '\n';
		}
	}
};

#endif /* FANNDATA_H_ */
