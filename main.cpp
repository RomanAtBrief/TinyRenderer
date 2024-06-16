#include <vector>
#include <cmath>
#include "tgaimage.h"
#include "model.h"
#include "geometry.h"

const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red = TGAColor(255, 0, 0, 255);
Model *model = NULL;
const int width = 1000;
const int height = 1000;

// Lesson 1: Bresenham's Line Drawing Algorithm
// First Attempt
// void line(int x0, int y0, int x1, int y1, TGAImage &image, TGAColor color)
// {
// 	for (float t = 0.0; t < 1.0; t += 0.01)
// 	{
// 		int x = x0 + (x1 - x0) * t;
// 		int y = y0 + (y1 - y0) * t;
// 		image.set(x, y, color);
// 	}
// }

// Second Attempt, drawing on each x but not y. Will have gaps
// void line(int x0, int y0, int x1, int y1, TGAImage &image, TGAColor color)
// {
// 	for (int x = x0; x <= x1; x++)
// 	{
// 		float t = (x - x0) / (float)(x1 - x0);
// 		int y = y0 * (1.0 - t) + y1 * t;
// 		image.set(x, y, color);
// 	}
// }

// Attempt Three. Swaping axis :)
// void line(int x0, int y0, int x1, int y1, TGAImage &image, TGAColor color)
// {
// 	bool steep = false;
// 	if (std::abs(x0 - x1) < std::abs(y0 - y1)) // if line is steep, we transpose(flip x becomes y) the image
// 	{
// 		std::swap(x0, y0);
// 		std::swap(x1, y1);
// 		steep = true;
// 	}
// 	if (x0 > x1) // make it left-to-right, original
// 	{
// 		std::swap(x0, x1);
// 		std::swap(y0, y1);
// 	}
// 	for (int x = x0; x <= x1; x++)
// 	{
// 		float t = (x - x0) / (float)(x1 - x0);
// 		int y = y0 * (1.0 - t) + y1 * t;

// 		if (steep)
// 		{
// 			image.set(y, x, color); // if transpose, de-transpose
// 		}
// 		else
// 		{
// 			image.set(x, y, color);
// 		}
// 	}
// }

// Final Solution
void line(int x0, int y0, int x1, int y1, TGAImage &image, TGAColor color)
{
	bool steep = false;
	if (std::abs(x0 - x1) < std::abs(y0 - y1))
	{
		std::swap(x0, y0);
		std::swap(x1, y1);
		steep = true;
	}
	if (x0 > x1)
	{
		std::swap(x0, x1);
		std::swap(y0, y1);
	}
	int dx = x1 - x0;
	int dy = y1 - y0;
	int derror2 = std::abs(dy) * 2;
	int error2 = 0;
	int y = y0;
	for (int x = x0; x <= x1; x++)
	{
		if (steep)
		{
			image.set(y, x, color);
		}
		else
		{
			image.set(x, y, color);
		}
		error2 += derror2;
		if (error2 > dx)
		{
			y += (y1 > y0 ? 1 : -1);
			error2 -= dx * 2;
		}
	}
}

int main(int argc, char **argv)
{
	if (2 == argc)
	{
		model = new Model(argv[1]);
	}
	else
	{
		model = new Model("obj/friend.obj");
	}

	TGAImage image(width, height, TGAImage::RGB);
	for (int i = 0; i < model->nfaces(); i++)
	{
		std::vector<int> face = model->face(i);
		for (int j = 0; j < 3; j++)
		{
			Vec3f v0 = model->vert(face[j]);
			Vec3f v1 = model->vert(face[(j + 1) % 3]);
			int x0 = (v0.x + 1.0) * width / 2.0;
			int y0 = (v0.y + 1.0) * height / 2.0;
			int x1 = (v1.x + 1.0) * width / 2.0;
			int y1 = (v1.y + 1.0) * height / 2.0;
			line(x0, y0, x1, y1, image, white);
		}
	}

	image.flip_vertically(); // i want to have the origin at the left bottom corner of the image
	image.write_tga_file("output.tga");
	delete model;
	return 0;
}
