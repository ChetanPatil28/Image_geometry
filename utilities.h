#pragma once

#include<iostream>
#include<math.h>
#include<vector>
#include<string>
#include<fstream>
#include<strstream>



class Vertex3D
{
public:
	float x, y, z;
	unsigned int length = 3;

	//constructing using member-list initializer.
	Vertex3D();
	Vertex3D(float X, float Y, float Z);

	//defining operator-overloading for this class.
	Vertex3D operator+(Vertex3D p);
	Vertex3D operator-(Vertex3D p);
	Vertex3D operator*(float p);
	Vertex3D operator/(float p);
	Vertex3D operator+=(Vertex3D p);
	Vertex3D operator-=(Vertex3D p);
	Vertex3D operator*=(float p);
	Vertex3D operator/=(float p);
	friend std::ostream& operator<<(std::ostream& print, Vertex3D p);
	
	// Defining methods for a 3-d vector.
	Vertex3D normalise();
	float dot(Vertex3D p);

	Vertex3D cross(Vertex3D p);

	const float operator[](unsigned int i);


	Vertex3D Ndc_to_Raster(const int width, const int height);

};


class Triangle
{
public:
	int length = 3;
	std::vector<Vertex3D>Points;
	Triangle(Vertex3D p1, Vertex3D p2, Vertex3D p3);
};

class Mesh
{
public:
	std::string file_path;
	std::vector<Triangle>triangles;

	float MinX = 10000.0f;
	float MaxX = -10000.0f;
	float MinY = 10000.0f;
	float MaxY = -10000.0f;
	float MinZ = 10000.0f;
	float MaxZ = -10000.0f;

	Vertex3D Min = { 10.000f, 10.000f , 10.000f };
	Vertex3D Max = { 10.000f, -10.000f , -10.000f };

	Mesh();

	Mesh(std::string fname);
	void buildMesh(); 
};

class Matrix
{

private:
	float top, bottom, left, right, near, far;

public:
	//float m[4][4] = {0.0f};
	Matrix();

	Matrix(float t, float b, float l, float r, float n, float f); 

	void create_OrthoMatrix(float m[][4]);
	

	void create_PerspectiveMatrix(float m[][4]);
};

