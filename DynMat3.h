#pragma once

#include "bezier.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace glm;

class DynMat3 {
private:
	vec3 ** mMatr;
	int h, w;
public :
	DynMat3();
	DynMat3(const int h, const int w) {
		//allocation
		this->h = h;
		this->w = w;

		mMatr = new vec3 * [h];
		for (int y = 0; y < h; y++) {
			mMatr[y] = new vec3[w];
		}

		// initialization
		for (int y = 0; y < h; y++) {
			for (int x = 0; x < w; x++) {
				mMatr[y][x] = vec3(0., 0., 0.);
			}
		}
	}

	~DynMat3() {
		for (int y = 0; y < w; y++) delete[] mMatr[y];
		delete[] mMatr;
	}
	
	vec3& at(int y, int x) {
		return mMatr[y][x];
	}
};