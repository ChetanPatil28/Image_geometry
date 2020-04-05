#include<iostream>
#include<math.h>
#include<vector>
#include<string>
#include<fstream>
#include<strstream>

class Vertex2D
{
public:
	float x, y;

	unsigned int length = 2;

	//constructing using member-list initializer.
	Vertex2D() : x(0.0f), y(0.0f) {}
	Vertex2D(float X, float Y) : x(X), y(Y) {}

	//defining operator-overloading for this class.
	Vertex2D operator+(Vertex2D p)
	{
		return { x + p.x, y + p.y };
	}

	Vertex2D operator-(Vertex2D p)
	{
		return { x - p.x, y - p.y };
	}

	Vertex2D operator*(float p)
	{
		return { x * p, y * p };
	}

	Vertex2D operator/(float p)
	{
		return { x / p, y / p };
	}

	Vertex2D operator+=(Vertex2D p)
	{
		x += p.x;  y += p.y;
		return *this;
	}

	Vertex2D operator-=(Vertex2D p)
	{
		x -= p.x;  y -= p.y;
		return *this;
	}

	Vertex2D operator*=(float p)
	{
		x *= p;  y *= p;
		return *this;
	}

	Vertex2D operator /=(float p)
	{
		x /= p;  y /= p;
		return *this;
	}

	friend std::ostream& operator<<(std::ostream& print, Vertex2D p)
	{
		return print << "(" << p.x << "," << p.y << ")" << std::endl;
	}
};


class Vertex3D
{
public:
	float x, y, z;

	unsigned int length = 3;

	//constructing using member-list initializer.
	Vertex3D() : x(0.0f), y(0.0f), z(0.0f) {}
	Vertex3D(float X, float Y, float Z) : x(X), y(Y), z(Z) {}

	//defining operator-overloading for this class.
	Vertex3D operator+(Vertex3D p)
	{
		return { x + p.x, y + p.y, z + p.z };
	}

	Vertex3D operator-(Vertex3D p)
	{
		return { x - p.x, y - p.y, z - p.z };
	}

	Vertex3D operator*(float p)
	{
		return { x * p, y * p, z * p };
	}

	Vertex3D operator/(float p)
	{
		return { x / p, y / p, z / p };
	}

	Vertex3D operator+=(Vertex3D p)
	{
		x += p.x;  y += p.y, z += p.z;
		return *this;
	}

	Vertex3D operator-=(Vertex3D p)
	{
		x -= p.x;  y -= p.y, z -= p.z;
		return *this;
	}

	Vertex3D operator*=(float p)
	{
		x *= p;  y *= p, z *= p;
		return *this;
	}

	Vertex3D operator/=(float p)
	{
		x /= p;  y /= p, z /= p;
		return *this;
	}

	friend std::ostream& operator<<(std::ostream& print, Vertex3D p)
	{
		return print << "(" << p.x << "," << p.y << "," << p.z << ")" << std::endl;
	}


	// Defining methods for a 3-d vector.
	Vertex3D normalise()
	{
		float sqr = sqrtf(x*x + y * y + z * z);
		return { x / sqr, y / sqr, z / sqr };
	}

	float dot(Vertex3D p)
	{
		return x * p.x + y * p.y + z * p.z;
	}

	Vertex3D cross(Vertex3D p)
	{
		return { y*p.z - z * p.y, z*p.x - x * p.z, x*p.y - y * p.x };
	}

	const float operator[](unsigned int i)
	{
		return (&x)[i];
	}

	Vertex3D Ndc_to_Raster(const int width, const int height)
	{
		//        Vertex3D = {1.}
		float x1 = ((x + 1.0f) / 2.0f)*width;
		float y1 =(1- ((y + 1.0f) / 2.0f))*height;
		float z1 = z;
		return { x1, y1,z1 };
	}

};

class Matrix
{

private:
	float top, bottom, left, right, near, far;

public:
	//float m[4][4] = {0.0f};
	Matrix();

	Matrix(float t, float b, float l, float r, float n, float f) :
		top(t), bottom(b), left(l), right(r), near(n), far(f) {}

	void create_OrthoMatrix(float m[][4])
	{
		m[0][0] = 2 / (right - left); m[0][3] = -((right + left) / (right - left));
		m[1][1] = 2 / (top - bottom); m[1][3] = -((top + bottom) / (top - bottom));
		m[2][2] = 2 / (far - near);   m[2][3] = -((far + near) / (far - near));
		m[3][3] = 1.0f;

	}

	void create_PerspectiveMatrix(float m[][4])
	{
		m[0][0] = (2 * near) / (right - left); m[0][3] = -((near*(right + left)) / (right - left));
		m[1][1] = (2 * near) / (top - bottom); m[1][3] = -((near*(top + bottom)) / (top - bottom));
		m[2][2] = -((far + near) / (far - near));   m[2][3] = - (2* far* near) / (far - near);
		m[3][2] = -1.0f;

		//float fNear = 0.1f;
		//float fFar = 200.0f;
		//float fFov = 90.0f;
		//float fAspectRatio = 1.0f;
		//float fFovRad = 1.0f / tanf(fFov * 0.5f / 180.0f * 3.14159f);

		//m[0][0] = fAspectRatio * fFovRad;
		//m[1][1] = fFovRad;
		//m[2][2] = fFar / (fFar - fNear);
		//m[2][3] = (-fFar * fNear) / (fFar - fNear);
		//m[3][2] = -1.0f;
		//m[3][3] = 0.0f;
	
	
	}


};


struct Triangle
{
	std::vector<Vertex3D>Point;
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

	Vertex3D Min = {10.000f, 10.000f , 10.000f };
	Vertex3D Max = {10.000f, -10.000f , -10.000f };

	Mesh();
	Mesh(std::string fname) :file_path(fname) {}

	void buildMesh()
	{
		std::string line;
		std::string v;
		int n = 0;

		std::ifstream myfile(file_path);

		std::vector<Vertex3D>vertices;
		int lenn = 0;
		while (!myfile.eof())
		//while(lenn<20)
		{
			std::getline(myfile, line);
			std::strstream s;
			s << line;
			char junk;
			if (line[0] == 'v')
			{

				Vertex3D v;
				float X, Y, Z;

				s >> junk >> X >> Y >> Z;




				if (X > MaxX) { MaxX = X; }
				if (X < MinX) { MinX = X; }

				if (Y > MaxY) { MaxY = Y; }
				if (Y < MinY) { MinY = Y; }

				if (Z > MaxZ) { MaxZ = Z; }
				if (Z < MinZ) { MinZ = Z; }

				//std::cout << v << std::endl;
				vertices.push_back({X, Y, Z});
			}

			if (line[0] == 'f')
			{
				Triangle tri;
				int faces[3];
				s >> junk >> faces[0] >> faces[1] >> faces[2];
				tri.Point.push_back(vertices[faces[0] - 1]);
				tri.Point.push_back(vertices[faces[1] - 1]);
				tri.Point.push_back(vertices[faces[2] - 1]);
				triangles.push_back(tri);

			}
			lenn += 1;

		}

	}

};
