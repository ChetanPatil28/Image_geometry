#include<opencv2/opencv.hpp>
#include<iostream>
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include<string>
#include "utilities.h"
#include<vector>


#include <chrono> 
using namespace std::chrono;

using namespace std;



cv::Point convert_to_cvPoint(Vertex3D i)
{
	cv::Point o(0, 0);
	o.x = (int)i.x;
	o.y = (int)i.y;
	return o;
}

void DrawTriangle(cv::Mat img, Triangle t)
{
	cv::Point p0 = convert_to_cvPoint(t.Points[0]);
	cv::Point p1 = convert_to_cvPoint(t.Points[1]);
	cv::Point p2 = convert_to_cvPoint(t.Points[2]);
	int thickness = 1;
	//int lineType = 1;
	cv::line(img, p0, p1, cv::Scalar(255, 255, 255),thickness, CV_AA);
	cv::line(img, p1, p2, cv::Scalar(255, 255, 255), thickness, CV_AA);
	cv::line(img, p2, p0, cv::Scalar(255, 255, 255), thickness, CV_AA);
}

struct Vertex3i {
	int x, y, z;
};

ostream& operator<<(ostream& disp, Vertex3i &p)
{
	return disp << " (" << p.x << "," << p.y << "," << p.z << ")" << endl;
}

Vertex3i float_to_int(Vertex3D p)
{
	return { (int)p.x, (int)p.y, (int)p.z };
}


Vertex3D MatMul(float m[][4], Vertex3D p)
{
	Vertex3D temp;
	temp.x = (m[0][0] * p[0]) + (m[0][1] * p[1]) + (m[0][2] * p[2]) + (m[0][3] * 1);
	temp.y = (m[1][0] * p[0]) + (m[1][1] * p[1]) + (m[1][2] * p[2]) + (m[1][3] * 1);
	temp.z = (m[2][0] * p[0]) + (m[2][1] * p[1]) + (m[2][2] * p[2]) + (m[2][3] * 1);
	float w = (m[3][0] * p[0]) + (m[3][1] * p[1]) + (m[3][2] * p[2]) + (m[3][3] * 0);
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
		{cout << m[i][j] << "\t";}		
	}
}





class GameEngine
{
public:

	string file = "simplecube_edit.obj";
	int width = 200;
	int height = 200;
	GameEngine() : width(400), height(400) {}
	GameEngine(string file_name, int w, int h) : file(file_name), width(w), height(h) {}
	float mat[4][4];
	vector<Triangle> tris;
	float fTheta = 0;
	float Xmin = 10000.0f; float Ymin = 10000.0f; float Zmin = 10000.0f;
	float Xmax = -10000.0f; float Ymax = -10000.0f; float Zmax = -10000.0f;
	float offSet = 1.0f;  //The bigger this number relative to gets, the smaller the object appers on screen
	Vertex3D C = { 0.0f, 0.0f, 0.0f };
	bool show_tris = false;

	Vertex3D RotateX(Vertex3D &i, float &theta)
			{
				float alpha = cosf(theta);
				float beta = sinf(theta);
				Vertex3D o = { 0.0f, 0.0f, 0.0f };
				o.x = i.x;
				o.y = (alpha * i.y) - (beta * i.z) + (C.y * (1 - alpha)) + (beta * C.z);
				o.z = (beta * i.y) + (alpha * i.z) - (beta* C.z) + (C.z * (1 - alpha));
				return o;
			}


	void Initialize()
	{
		Mesh object(file);
		object.buildMesh();
		Xmin = object.MinX; Xmax = object.MaxX;
		Ymin = object.MinY; Ymax = object.MaxY;
		Zmin = object.MinZ; Zmax = object.MaxZ;
		tris = object.triangles;
		// pass the values in this order--> top, bottom, left, right, near, far
		//Matrix ogm(maxs.y * offSet, mins.y * offSet, mins.x * offSet, maxs.x * offSet, mins.z * offSet, maxs.z * offSet);

		float top = Ymax * offSet; float bottom = Ymin * offSet;
		float left = Xmin * offSet; float right = Xmax * offSet;

		float nearr = 0.5f; //keep this for prespective matrix
		float farr = 10.0f;

		Matrix ogm(top, bottom, left, right, nearr, farr);
		//Matrix ogm(+8.0f, -8.0f, -8.0f, +8.0f, -8.0f, 8.0f);
		ogm.create_PerspectiveMatrix(mat);
		//ogm.create_OrthoMatrix(mat);

	}

	void BeginScene()
	{

		while (true)
		{
			cv::Mat screen = cv::Mat::zeros(cv::Size(width, height), CV_8UC1);

			//Fill(0, 0, ScreenWidth(), ScreenHeight(), PIXEL_SOLID, FG_BLACK);


			fTheta += 1.0f;


			for (int i = 0; i < tris.size(); i++)
			{
				Vertex3D v1 = tris[i].Points[0];
				Vertex3D v2 = tris[i].Points[1];
				Vertex3D v3 = tris[i].Points[2];

				//Vertex3D v1 = RotateZ(p1, fTheta);
				//Vertex3D v2 = RotateZ(p2, fTheta);
				//Vertex3D v3 = RotateZ(p3, fTheta);


				v1 = RotateX(v1, fTheta);
				v2 = RotateX(v2, fTheta);
				v3 = RotateX(v3, fTheta);

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

				Triangle iTri(raster_points1, raster_points2, raster_points3);

				//create a function to draw a triangle and colors

				/*DrawTriangle(raster_points1.x, raster_points1.y,
					raster_points2.x, raster_points2.y,
					raster_points3.x, raster_points3.y, PIXEL_SOLID, FG_WHITE);*/
				DrawTriangle(screen, iTri);

			}
			//Finished drawing all the triangles.!! So we'll imshow the img.!
			cv::imshow("blend", screen);
			char c = (char)cv::waitKey(100);
			if (c == 27)
				break;

		}
	}
	// If we press-key, then destroy the window.!
//cv::destroyAllWindows();

};



int main()
{

	string f = "simplecube.obj";
	GameEngine mygame(f, 400, 400);
	mygame.Initialize();
	int l = mygame.tris.size();

	cout << "NUm_tris are " << l << endl;
	mygame.show_tris = true;
	mygame.BeginScene();

	
	return 0;
}

