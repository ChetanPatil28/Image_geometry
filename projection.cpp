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

	temp.x = (m[0][0] * p[0]) + m[0][3];
	temp.y = (m[1][1] * p[1]) + m[1][3];
	temp.z = (m[2][2] * p[2]) + m[2][3];

	//cout << "X map to " << (int)temp.x << endl;
	//cout << "Y map to " << (int)temp.y << endl;
	//cout << "Z map to " << (int)temp.z << endl;

	//cout << "Vertex aftr multiply is " << temp << endl;


	float w = m[3][3];
	//float w = 1;
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
		m_sAppName = L"3D Demo";
		file = file_name;
	}

	float mat[4][4];
	vector<Triangle> tris;
	float fTheta = 0;

	float width = 200;
	float height = 200;
	Vertex3D mins;
	Vertex3D maxs;
	
	float offSet = 2.0f;  //The bigger this number relative to gets, the smaller the object appers on screen

	Vertex3D C = { 0.0f, 0.0f, 0.0f };

	Vertex3D OrthoGraphicRotateZ(Vertex3D &i, float &theta)
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
	Vertex3D OrthoGraphicRotateY(Vertex3D &i, float &theta)
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
	Vertex3D OrthoGraphicRotateX(Vertex3D &i, float &theta)
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

		/*Xmin = object.minX; Xmax = object.maxX;
		Ymin = object.minY; Ymax = object.maxY;
		Zmin = object.minZ; Zmax = object.maxZ;
*/
		mins = object.Min;
		maxs = object.Max;

		tris = object.triangles;
		// pass the values in this order--> top, bottom, left, right, near, far
		OrthoGraphicMatrix ogm(maxs.y * offSet, mins.y * offSet, mins.x * offSet, maxs.x * offSet, mins.z * offSet, maxs.z * offSet);
		ogm.create_matrix(mat);
		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		Fill(0, 0, ScreenWidth(), ScreenHeight(), PIXEL_SOLID, FG_BLACK);
		fTheta += 1.0f * fElapsedTime;

		for (int i = 0; i < tris.size(); i++)
		{

			Vertex3D v1 = OrthoGraphicRotateZ(tris[i].Point[0], fTheta);
			Vertex3D v2 = OrthoGraphicRotateZ(tris[i].Point[1], fTheta);
			Vertex3D v3 = OrthoGraphicRotateZ(tris[i].Point[2], fTheta);

			 v1 = OrthoGraphicRotateY(v1, fTheta);
			 v2 = OrthoGraphicRotateY(v2, fTheta);
			 v3 = OrthoGraphicRotateY(v3, fTheta);

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

	return  0;
}

