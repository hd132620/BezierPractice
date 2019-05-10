#pragma once

#include "bezier.h"
#include "DynMat3.h"
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define X_Y 0
#define Y_X 0

#define X_Z 1
#define Z_X 1

#define Y_Z 2
#define Z_Y 2

using namespace glm;

class BezierInput {

private:
	Bezier::Bezier<2> *curve;
	DynMat3 *matrix;

	float widthStart;
	float width;
	unsigned int scale;
	unsigned int mode = Y_Z;

	bool isCalculated = false;

public:

	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Constructor
	BezierInput() {
		curve = new Bezier::Bezier<2>();

		width = 1;
		scale = 3;
		widthStart = 0.;
		mode = Y_Z;

		matrix = new DynMat3(scale, 2);
	}
	BezierInput(unsigned int scale = 3, float widthStart = 0., float width=1., unsigned int mode = Y_Z)
	{
		curve = new Bezier::Bezier<2>();

		this->scale = scale;
		this->widthStart = widthStart;
		this->width = width;

		matrix = new DynMat3(scale, 2);
	}
	BezierInput(Bezier::Point start = { 0, 0 }, Bezier::Point control = { 0, 0.5 }, Bezier::Point end = { 0.25, 0.25 }, 
		unsigned int scale = 1000, float widthStart = 0., float width = 1., unsigned int mode = Y_Z)
	{
		curve = new Bezier::Bezier<2>({ start, control, end });
		this->scale = scale;
		this->width = width;
		this->widthStart = widthStart;

		matrix = new DynMat3(scale, 2);
	};

	~BezierInput()
	{
		delete curve;
		delete matrix;
	}

	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Actual Calculation
	void BezierInput::calculatePoints(void) {
		Bezier::Point calpoint;
		for (int bNum = 0; bNum <= scale - 1; bNum++) {
			for (int wNum = 0; wNum < 2; wNum++) {

				calpoint = curve->valueAt((float)bNum / (scale - 1));
				switch (mode)
				{
				case X_Y:
					matrix->at(bNum, wNum) = vec3(calpoint.x, calpoint.y, widthStart + widthStart * wNum);
				case X_Z:
					matrix->at(bNum, wNum) = vec3(calpoint.x, widthStart + widthStart * wNum, calpoint.y);
				case Y_Z:
					matrix->at(bNum, wNum) = vec3(widthStart + widthStart*wNum, calpoint.x, calpoint.y);
				}
			}
		}
		this->isCalculated = true;
		std::cout << "calculated\n";
	}

	float* getVertexes() {

		if (!this->isCalculated)
			calculatePoints();

		std::vector<float> vertexes = std::vector<float>();
		for (int bNum = 0; bNum <= scale - 1; bNum++) {
			for (int wNum = 0; wNum < 2; wNum++) {
				vertexes.push_back(matrix->at(bNum, wNum).x);
				vertexes.push_back(matrix->at(bNum, wNum).y);
				vertexes.push_back(matrix->at(bNum, wNum).z);
				//std::cout << "Inside getVertexes()" <<  matrix->at(bNum, wNum).x << matrix->at(bNum, wNum).y << matrix->at(bNum, wNum).z << std::endl;
			}
		}

		float* fver = new float[vertexes.size()];
		for (int i = 0; i < vertexes.size(); i++)
			fver[i] = vertexes[i];

		return fver;
	}

	unsigned int* getElements() {

		// Vertex number definition
		unsigned int vxN = 2 * (scale - 1);

		std::vector<unsigned int> elements = std::vector<unsigned int>();

		// Changing correct type -> 1-dim float type array
		for (int i = 0; i < vxN; i++) 
			for (int j = 0; j < 3; j++) 
				elements.push_back(i+j);


		unsigned int* felm = new unsigned int[elements.size()];
		for (int i = 0; i < elements.size(); i++)
			felm[i] = elements[i];

		return felm;
	}

	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	// getter, setter

	unsigned int getScale(void)
	{
		return scale;
	}

	void setScale(unsigned int scale = 3)
	{
		this->scale = scale; 

		delete matrix;
		matrix = new DynMat3(scale, 2);

		this->isCalculated = false;
	}

	unsigned int getMode(void)
	{
		return mode;
	}
	void setMode(unsigned int mode = Y_Z)
	{
		this->mode = mode;
		this->isCalculated = false;
	}
};