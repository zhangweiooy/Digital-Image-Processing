#include<opencv2/opencv.hpp>
#include<iostream>
#include<algorithm>
#include<cmath>
using namespace std;
using namespace cv;
void RGB2CMY(Mat& rgb, float k)
{
	int height = rgb.rows;
	int width = rgb.cols;
	Mat cmy = rgb.clone();
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			float b = rgb.at<Vec3b>(i, j)[0];//在OpenCv中RGB图像是按照B，G，R的顺序存储的
			float g = rgb.at<Vec3b>(i, j)[1];
			float r = rgb.at<Vec3b>(i, j)[2];
			float c, m, y;
			c = 255 - r;
			m = 255 - g;
			y = 255 - b;
			cmy.at<Vec3b>(i, j)[0] = (c * k + 255 * (1 - k));
			cmy.at<Vec3b>(i, j)[1] = (m * k + 255 * (1 - k));
			cmy.at<Vec3b>(i, j)[2] = (y * k + 255 * (1 - k));
		}
	}
	imwrite("2CMY.jpg", cmy);
	return;
}
void RGB2HSI(Mat& rgb, float k)
{
	int height = rgb.rows;
	int width = rgb.cols;
	Mat hsi = rgb.clone();
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			float b = rgb.at<Vec3b>(i, j)[0] / 255.0;
			float g = rgb.at<Vec3b>(i, j)[1] / 255.0;
			float r = rgb.at<Vec3b>(i, j)[2] / 255.0;
			float minRGB = min(min(r, g), b);
			float H, S, I;
			float temp1, temp2, radians, theta;
			temp1 = ((r - g) + (r - b)) / 2;
			temp2 = sqrt((r - g) * (r - g) + (r - b) * (g - b));
			if (temp2 == 0)
			{
				H = 0;
				S = 0;
			}
			else
			{
				radians = acos(temp1 / temp2);
				theta = radians * 180.0 / 3.14;
				if (b > g)
					H = 360.0 - theta;
				else
					H = theta;
			}
			I = (r + g + b) / 3;
			if (I < 0.08)
				S = 0;
			else if (I > 0.92)
				S = 0;
			else
				S = 1.0 - (3.0 * minRGB) / (r + g + b);
			I *= 255;
			S *= 255;
			H = H / 360 * 255;
			hsi.at<Vec3b>(i, j)[0] = H;
			hsi.at<Vec3b>(i, j)[1] = S;
			hsi.at<Vec3b>(i, j)[2] = (I * k);
		}
	}
	imwrite("3HSI.jpg", hsi);
	return;
}
void Transform_RGB(Mat& rgb, float k)
{
	int height = rgb.rows;
	int width = rgb.cols;
	Mat obj = rgb.clone();
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			float b = obj.at<Vec3b>(i, j)[0];//在OpenCv中RGB图像是按照B，G，R的顺序存储的
			float g = obj.at<Vec3b>(i, j)[1];
			float r = obj.at<Vec3b>(i, j)[2];
			r = r * k;
			g = g * k;
			b = b * k;
			obj.at<Vec3b>(i, j)[0] = b;
			obj.at<Vec3b>(i, j)[1] = g;
			obj.at<Vec3b>(i, j)[2] = r;
		}
	}
	imwrite("1RGB.jpg", obj);
	return;
}

int main()
{
	string imagepath("../pictures/0.jpg");
	Mat image = imread(imagepath, IMREAD_COLOR);
	if (image.empty())
	{
		cout << "Open image error" << endl;
	}
	float k = 0.7;
	RGB2CMY(image, k);
	RGB2HSI(image, k);
	Transform_RGB(image, k);
	waitKey();
	return 0;
}