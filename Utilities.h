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
		float y1 = ((y + 1.0f) / 2.0f)*height;
		float z1 = z;
		return { x1, y1,z1 };
	}

};

class OrthoGraphicMatrix
{

private:
	float top, bottom, left, right, near, far;

public:
	//float m[4][4] = {0.0f};
	OrthoGraphicMatrix();
	OrthoGraphicMatrix(float t, float b, float l, float r, float n, float f) :
		top(t), bottom(b), left(l), right(r), near(n), far(f) {}

	void create_matrix(float m[][4])
	{
		m[0][0] = 2 / (right - left); m[0][3] = -((right + left) / (right - left));
		m[1][1] = 2 / (top - bottom); m[0][3] = -((top + bottom) / (top - bottom));
		m[2][2] = 2 / (far - near);   m[0][3] = -((far + near) / (far - near));
		m[3][3] = 1.0f;

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
	//std::vector<Vertex3D>vertices;
	std::vector<Triangle>triangles;
	Mesh();
	Mesh(std::string fname) :file_path(fname) {}

	void buildMesh()
	{
		std::string line;
		std::string v;
		int n = 0;

		std::ifstream myfile(file_path);

		std::vector<Vertex3D>vertices;

		while (!myfile.eof())
		{
			std::getline(myfile, line);
			std::strstream s;
			s << line;
			char junk;
			if (line[0] == 'v')
			{

				Vertex3D v;
				s >> junk >> v.x >> v.y >> v.z;
				//std::cout << v << std::endl;
				vertices.push_back(v);
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

		}

	}

};