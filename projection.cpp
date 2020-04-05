#include<vector>
#include "Utilities.h"
#include "olcConsoleGameEngine.h"

using namespace std;


struct Vertex3i {
	int x, y, z;
};

ostream& operator<<(ostream& disp, Vertex3i &p)
{
	return disp << " (" << p.x << "," << p.y << "," << p.z << ")" << endl;
}

Vertex3i float_to_int(Vertex3D &p)
{
	return { (int)p.x, (int)p.y, (int)p.z };
}


Vertex3D MatMul(float m[][4], Vertex3D &p)
{
	Vertex3D temp;
	//cout << "Vertex b4 multiply is " << p << endl;

	/*temp.x = (m[0][0] * p[0]) + m[0][3];
	temp.y = (m[1][1] * p[1]) + m[1][3];
	temp.z = (m[2][2] * p[2]) + m[2][3];
*/
	temp.x = (m[0][0] * p[0]) + (m[0][1] * p[1]) + (m[0][2] * p[2]) + (m[0][3] * 1);
	temp.y = (m[1][0] * p[0]) + (m[1][1] * p[1]) + (m[1][2] * p[2]) + (m[1][3] * 1);
	temp.z = (m[2][0] * p[0]) + (m[2][1] * p[1]) + (m[2][2] * p[2]) + (m[2][3] * 1);

	float w = (m[3][0] * p[0]) + (m[3][1] * p[1]) + (m[3][2] * p[2]) + (m[3][3] * 0);

	//cout << "X map to " << (int)temp.x << endl;
	//cout << "Y map to " << (int)temp.y << endl;
	//cout << "Z map to " << (int)temp.z << endl;

	//cout << "Vertex aftr multiply is " << temp << endl;


	//float w = 1.0f;
	if (w != 0.0f)
	{
		temp /= w;
	}
	return temp;

}

void print_mat(float m[][4])
{
	//cout << "Matrix starts now" << endl;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			//cout << m[i][j] << "\t";
		}
		//cout << "\n";
	}
}


class olcEngine3D : public olcConsoleGameEngine
{
public:

	string file;

	olcEngine3D(string file_name)
	{
		m_sAppName = L"Perspective Demo";
		file = file_name;
	}

	float mat[4][4];
	vector<Triangle> tris;
	float fTheta = 0;

	float width =200;
	float height = 200;
	Vertex3D mins;
	Vertex3D maxs;

	float Xmin = 10000.0f; float Ymin = 10000.0f; float Zmin = 10000.0f;
	float Xmax = -10000.0f; float Ymax = -10000.0f; float Zmax = -10000.0f;
	
	float offSet = 1.0f;  //The bigger this number relative to gets, the smaller the object appers on screen

	Vertex3D C = { 0.0f, 0.0f, 0.0f };

	Vertex3D RotateZ(Vertex3D &i, float &theta)
	{
		float alpha = cosf(theta);
		float beta = sinf(theta);
		Vertex3D o = { 0.0f, 0.0f, 0.0f };
		o.x = (alpha * i.x) - (beta * i.y) + (C.x * (1 - alpha)) + (beta * C.y);
		o.y = (beta * i.x) + (alpha * i.y) - (beta* C.x) + (C.y * (1 - alpha)) ;
		o.z = i.z ;
		return o;
	}
			
	//                   PITCH
	Vertex3D RotateY(Vertex3D &i, float &theta)
	{
		float alpha = cosf(theta);
		float beta = sinf(theta);
		Vertex3D o = { 0.0f, 0.0f, 0.0f };
		o.x = (alpha * i.x) - (beta * i.z) + (C.x * (1 - alpha)) + (beta * C.z);
		o.y = i.y ;
		o.z = (beta * i.x) + (alpha * i.z) - (beta* C.z) + (C.z * (1 - alpha)) ;
		return o;
	}
		
	//                          ROLL
	Vertex3D RotateX(Vertex3D &i, float &theta)
	{
		float alpha = cosf(theta);
		float beta = sinf(theta);
		Vertex3D o = { 0.0f, 0.0f, 0.0f };
		o.x = i.x;
		o.y = (alpha * i.y) - (beta * i.z) + (C.y * (1 - alpha)) + (beta * C.z);
		o.z = (beta * i.y) + (alpha * i.z) - (beta* C.z) + (C.z * (1 - alpha)) ;
		return o;
	}
	
public:
	bool OnUserCreate() override
	{
		Mesh object(file);
		object.buildMesh();

		Xmin = object.MinX; Xmax = object.MaxX;
		Ymin = object.MinY; Ymax = object.MaxY;
		Zmin = object.MinZ; Zmax = object.MaxZ;

		//mins = object.Min;
		//maxs = object.Max;

		tris = object.triangles;
		// pass the values in this order--> top, bottom, left, right, near, far
		//Matrix ogm(maxs.y * offSet, mins.y * offSet, mins.x * offSet, maxs.x * offSet, mins.z * offSet, maxs.z * offSet);

		float top = Ymax * offSet; float bottom = Ymin * offSet;
		float left = Xmin * offSet; float right = Xmax * offSet;

		float nearr = 0.5f;
		float farr = 10.0f;

		Matrix ogm(top, bottom, left, right, nearr, farr);

		//Matrix ogm(+8.0f, -8.0f, -8.0f, +8.0f, -8.0f, 8.0f);


		ogm.create_PerspectiveMatrix(mat);
		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		Fill(0, 0, ScreenWidth(), ScreenHeight(), PIXEL_SOLID, FG_BLACK);
		fTheta += 1.0f * fElapsedTime;

		for (int i = 0; i < tris.size(); i++)
		{
			Vertex3D p1 = tris[i].Point[0];
			Vertex3D p2 = tris[i].Point[1];
			Vertex3D p3 = tris[i].Point[2];



			//Vertex3D v1 = p1; Vertex3D v2 = p2; Vertex3D v3 = p3;

			Vertex3D v1 = RotateZ(p1, fTheta);
			Vertex3D v2 = RotateZ(p2, fTheta);
			Vertex3D v3 = RotateZ(p3, fTheta);


			 //v1 = RotateX(v1, fTheta);
			 //v2 = RotateX(v2, fTheta);
			 //v3 = RotateX(v3, fTheta);

	/*		 v1 = RotateY(v1, fTheta);
			 v2 = RotateY(v2, fTheta);
			 v3 = RotateY(v3, fTheta);*/


			float of = 2.7f;
			// This z-plane translation is because some points get projected at the 
			// back of the screen.!
			v1.z += of; v2.z += of; v3.z += of;

			//Vertex3D v1 = MatMul(mat, tris[i].Point[0]);
			//Vertex3D v2 = MatMul(mat, tris[i].Point[1]);
			//Vertex3D v3 = MatMul(mat, tris[i].Point[2]);


			v1 = MatMul(mat, v1);
			v2 = MatMul(mat, v2);
			v3 = MatMul(mat, v3);

			//cout << "Tested point is " << test << endl;

			Vertex3D unit_vec{ 1.0f,1.0f,1.0f };
			//cout << "Maping to screen " << ((test + unit_vec) / 2.0f) * 400 << endl;
			Vertex3D raster_points1 = v1.Ndc_to_Raster(width, height);
			Vertex3D raster_points2 = v2.Ndc_to_Raster(width, height);
			Vertex3D raster_points3 = v3.Ndc_to_Raster(width, height);

			

				DrawTriangle(raster_points1.x, raster_points1.y,
					raster_points2.x, raster_points2.y,
					raster_points3.x, raster_points3.y, PIXEL_SOLID, FG_WHITE);
			


		}

		return true; 
	}
};




int main()
{
	string object_file = "TeaPot.obj";
	olcEngine3D demo(object_file);
	if (demo.ConstructConsole(200, 200, 2, 2))
		demo.Start();
	return 0;

}

