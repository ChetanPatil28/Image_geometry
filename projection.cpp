#include "olcConsoleGameEngine.h"
#include <iostream>

using namespace std;
#define PI 3.14159265


struct vec3d
{
	float x, y, z;
	vec3d operator+(vec3d p) { return { x + p.x, y + p.y, z+p.z }; }
	vec3d operator-(vec3d p) { return { x - p.x, y - p.y, z - p.z }; }
};

struct triangle
{
	vec3d p1;
	vec3d p2;
	vec3d p3;
};


ostream& operator<<(ostream& print, vec3d p)
{
	return print << "(" << p.x << "," << p.y << ")";
}


class Geometry : public olcConsoleGameEngine
{

private:
	triangle tri;
	float Theta = 0;
	float off = 0;
	float cX = ScreenWidth() / 2;  
	float cY = ScreenHeight() / 2;
	float cZ = 0;
	vec3d test_point = { 100, 75, 1 };
	vec3d C = { cX, cY, cZ };

	// YAW
	void RotateZ(vec3d &i, vec3d &o,float theta)
	{
		vec3d diff = i - C;
		o.x = (cosf(theta)*diff.x) - (sinf(theta)*diff.y) + C.x ;
		o.y = (sinf(theta)*diff.x) + (cosf(theta)*diff.y) + C.y;
		o.z = i.z;
		o = o + C;
	}

	// PITCH
	void RotateY(vec3d &i, vec3d &o, float theta)
	{
		vec3d diff = i - C;
		o.x = (cosf(theta)*diff.x) - (sinf(theta)*diff.z) + C.x;
		o.y = i.y;
		o.z = (sinf(theta)*diff.x) + (cosf(theta)*diff.z) + C.y;
		o = o + C;
	}

	// ROLL
	void RotateX(vec3d &i, vec3d &o, float theta)
	{
		vec3d diff = i - C;
		o.x = i.x;
		o.y = (cosf(theta)*diff.y) - (sinf(theta)*diff.z) + C.y ;
		o.z = (sinf(theta)*diff.y) + (cosf(theta)*diff.z) + C.z;
		o = o + C;
	}


public:
	Geometry()
	{
		m_sAppName = L"Projection";
	}
	bool OnUserCreate() override
	{
		tri.p1 = { 50.0f, 50.0f, 10.0f };
		tri.p2 = {70.0f, 35.0f, 12.0f };
		tri.p3 = { 90.0f, 75.0f, 20.0f };
		return true;
	}


	bool OnUserUpdate(float fElapsedTime) override
	{
		Theta += 1.0f * fElapsedTime;

		// Clear Screen
		Fill(0, 0, ScreenWidth(), ScreenHeight(), PIXEL_SOLID, FG_BLACK);
		
		triangle op;
		vec3d test_point_op;


		RotateZ(tri.p1, op.p1, Theta);
		RotateZ(tri.p2, op.p2, Theta);
		RotateZ(tri.p3, op.p3, Theta);

		RotateZ(test_point, test_point_op, Theta);


		DrawTriangle(op.p1.x , op.p1.y,
			op.p2.x, op.p2.y,
			op.p3.x, op.p3.y,
			PIXEL_SOLID, FG_WHITE);

		DrawCircle(test_point_op.x, test_point_op.y, 2, PIXEL_SOLID, FG_WHITE);
		 
		return true;

	}

};




int main()
{
	Geometry demo;
	
	if (demo.ConstructConsole(200, 200, 2, 2))
		demo.Start();
	return 0;
}

