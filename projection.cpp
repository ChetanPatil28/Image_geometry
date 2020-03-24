#include "olcConsoleGameEngine.h"
#include <iostream>

using namespace std;
#define PI 3.14159265


struct vec3d
{
	float x, y, z;
	vec3d operator+(vec3d p) { return { x + p.x, y + p.y, z+p.z }; }
	vec3d operator-(vec3d p) { return { x - p.x, y - p.y, z - p.z }; }
	vec3d operator/(float p) { return { x/p, y/p, z/p }; }
	vec3d operator*(float p) { return { x * p, y * p, z * p }; }

};

struct triangle
{
	vec3d p1;
	vec3d p2;
	vec3d p3;
};

struct cube
{
	vec3d p1;
	vec3d p2;
	vec3d p3;
	vec3d p4;
	vec3d p5;
	vec3d p6;
	vec3d p7;
	vec3d p8;


};

struct square
{
	vec3d p1, p2, p3, p4;
};


ostream& operator<<(ostream& print, vec3d p)
{
	return print << "(" << p.x << "," << p.y << ")";
}


class Geometry : public olcConsoleGameEngine
{

private:
	triangle tri;
	cube cub;
	square sqr;

	float Theta = 0;
	float off = 5;
	float cX = 50.0f  ;  
	//float yy = m_nSc
	float cY = 50.0f  ;
	float cZ = 10.0f;
	vec3d test_point = { cX + 35.0f , cY + 35.0f , 0.0f };
	vec3d C = { cX, cY, cZ };

	// YAW
	void OrthoGraphicRotateZ(vec3d &i, vec3d &o,float theta)
	{
		vec3d diff = i - C;
		o.x = (cosf(theta)*diff.x) - (sinf(theta)*diff.y) + C.x ;
		o.y = (sinf(theta)*diff.x) + (cosf(theta)*diff.y) + C.y;
		o.z = i.z;
		o = o + C;
	}

	// PITCH
	void OrthoGraphicRotateY(vec3d &i, vec3d &o, float theta)
	{
		vec3d diff = i - C;
		o.x = (cosf(theta)*diff.x) - (sinf(theta)*diff.z) + C.x;
		o.y = i.y;
		o.z = (sinf(theta)*diff.x) + (cosf(theta)*diff.z) + C.y;
		o = o + C;
	}

	void ProjectiveRotateY(vec3d &i, vec3d &o, float theta)
	{
		//i = i / i.z;
		vec3d diff = i - C;
		o.x = (cosf(theta)*diff.x) - (sinf(theta)*diff.z) + C.x;
		o.y = i.y;
		o.z = (sinf(theta)*diff.x) + (cosf(theta)*diff.z) + C.y;
		o = o + C;

		//o = o * i.z;
		o = o / o.z;

	}

	// ROLL
	void OrthoGraphicRotateX(vec3d &i, vec3d &o, float theta)
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
		
		sqr.p1 = { 50.0f, 50.0f, 0.0f };
		sqr.p2 = { 100.0f, 50.0f, 0.0f };
		sqr.p3 = { 100.0f, 100.0f, 0.0f };
		sqr.p4 = { 50.0f, 100.0f, 0.0f };

		cub.p1 = { C.x-25.0f,   C.y-25.0f,   C.z - 25.0f };
		cub.p2 = { C.x +25.0f,  C.y -25.0f,  C.z - 25.0f };
		cub.p3 = { C.x +25.0f,  C.y +25.0f,  C.z - 25.0f };
		cub.p4 = { C.x -25.0f,  C.y + 25.0f, C.z - 25.0f };
		cub.p5 = { C.x - 25.0f, C.y - 25.0f, C.z + 25.0f };
		cub.p6 = { C.x + 25.0f, C.y - 25.0f, C.z + 25.0f };
		cub.p7 = { C.x + 25.0f, C.y + 25.0f, C.z + 25.0f };
		cub.p8 = { C.x - 25.0f, C.y + 25.0f, C.z + 25.0f };

		return true;
	}


	bool OnUserUpdate(float fElapsedTime) override
	{
		Theta += 1.0f * fElapsedTime;

		// Clear Screen
		Fill(0, 0, ScreenWidth(), ScreenHeight(), PIXEL_SOLID, FG_BLACK);
		
		cube op;
		vec3d test_point_op;


		OrthoGraphicRotateY(cub.p1, op.p1, Theta);
		OrthoGraphicRotateY(cub.p2, op.p2, Theta);
		OrthoGraphicRotateY(cub.p3, op.p3, Theta);
		OrthoGraphicRotateY(cub.p4, op.p4, Theta);
		OrthoGraphicRotateY(cub.p5, op.p5, Theta);
		OrthoGraphicRotateY(cub.p6, op.p6, Theta);
		OrthoGraphicRotateY(cub.p7, op.p7, Theta);
		OrthoGraphicRotateY(cub.p8, op.p8, Theta);

		//ProjectiveRotateY(cub.p1, op.p1, Theta);
		//ProjectiveRotateY(cub.p2, op.p2, Theta);
		//ProjectiveRotateY(cub.p3, op.p3, Theta);
		//ProjectiveRotateY(cub.p4, op.p4, Theta);
		//ProjectiveRotateY(cub.p5, op.p5, Theta);
		//ProjectiveRotateY(cub.p6, op.p6, Theta);
		//ProjectiveRotateY(cub.p7, op.p7, Theta);
		//ProjectiveRotateY(cub.p8, op.p8, Theta);

		OrthoGraphicRotateZ(test_point, test_point_op, Theta);

		DrawLine(op.p1.x,op.p1.y,op.p2.x,op.p2.y, PIXEL_SOLID, FG_WHITE);
		DrawLine(op.p2.x, op.p2.y, op.p3.x, op.p3.y, PIXEL_SOLID, FG_WHITE);
		DrawLine(op.p3.x, op.p3.y, op.p4.x, op.p4.y, PIXEL_SOLID, FG_WHITE);
		DrawLine(op.p4.x, op.p4.y, op.p1.x, op.p1.y, PIXEL_SOLID, FG_WHITE);

		DrawLine(op.p5.x, op.p5.y, op.p6.x, op.p6.y, PIXEL_SOLID, FG_WHITE);
		DrawLine(op.p6.x, op.p6.y, op.p7.x, op.p7.y, PIXEL_SOLID, FG_WHITE);
		DrawLine(op.p7.x, op.p7.y, op.p8.x, op.p8.y, PIXEL_SOLID, FG_WHITE);
		DrawLine(op.p8.x, op.p8.y, op.p5.x, op.p5.y, PIXEL_SOLID, FG_WHITE);

		DrawLine(op.p1.x, op.p1.y, op.p5.x, op.p5.y, PIXEL_SOLID, FG_WHITE);
		DrawLine(op.p2.x, op.p2.y, op.p6.x, op.p6.y, PIXEL_SOLID, FG_WHITE);
		DrawLine(op.p3.x, op.p3.y, op.p7.x, op.p7.y, PIXEL_SOLID, FG_WHITE);
		DrawLine(op.p4.x, op.p4.y, op.p8.x, op.p8.y, PIXEL_SOLID, FG_WHITE);




		DrawLine(test_point_op.x, test_point_op.y, test_point.x, test_point.y, PIXEL_SOLID, FG_WHITE);


		//DrawTriangle(op.p1.x , op.p1.y,
	//		op.p2.x, op.p2.y,
	//		op.p3.x, op.p3.y,
	//		PIXEL_SOLID, FG_WHITE);
		//DrawCircle(test_point.x, test_point.y, 2, PIXEL_SOLID, FG_WHITE);


		DrawCircle(test_point_op.x, test_point_op.y, 2, PIXEL_SOLID, FG_DARK_GREY);
		 
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

