#include<opencv2/opencv.hpp>
#include<iostream>
#include<cmath>
using namespace std;
using namespace cv;
int Calculate(Vec3f src, Vec3f* VectorSpace)//ͨ��ŷ�Ͼ��������ӽ�������
{
	float distance;
	int NearestVector = 0;//��ӽ���ƽ����ɫ����
	distance = sqrtf(powf((src[0] - VectorSpace[0][0]), 2) + powf((src[1] - VectorSpace[0][1]), 2) + powf((src[2] - VectorSpace[0][2]), 2));
	for (int i = 1; i < 8; i++)
	{
		float temp = sqrtf(powf((src[0] - VectorSpace[i][0]), 2) + powf((src[1] - VectorSpace[i][1]), 2) + powf((src[2] - VectorSpace[i][2]), 2));
		if (distance > temp)
		{
			distance = temp;
			NearestVector = i;
		}
	}
	return NearestVector;
}
int main()
{
	string imagepath("C:/Users/11039/Desktop/Img.jpg");
	Mat img = imread(imagepath);//�洢���ص�ͼ�������
//    resize(img, img, Size(0, 0), 0.5, 0.5, INTER_NEAREST);
	if (img.empty())
	{
		cout << "couldn't find the image" << endl;
	}
	//��ʼ��RGB�����ռ�,��ģ��ΪBGR��
	Vec3f* RGBVectorSpace = new Vec3f[8];
	RGBVectorSpace[0] = Vec3f(0, 0, 0);//��ɫ
	RGBVectorSpace[1] = Vec3f(1.0, 0, 0);//��ɫ
	RGBVectorSpace[2] = Vec3f(0, 1.0, 0);//��ɫ
	RGBVectorSpace[3] = Vec3f(0, 0, 1.0);//��ɫ
	RGBVectorSpace[4] = Vec3f(1.0, 1.0, 0);//��ɫ
	RGBVectorSpace[5] = Vec3f(1.0, 0, 1.0);//��ɫ
	RGBVectorSpace[6] = Vec3f(0, 1.0, 1.0);//��ɫ
	RGBVectorSpace[7] = Vec3f(1.0, 1.0, 1.0);//��ɫ
	Mat image;
	img.convertTo(image, CV_32FC3, 1 / 255.0);
	Mat result(img.size(), CV_32FC3, Scalar(1.0, 1.0, 1.0));
	//��������
	int NearestVector;
	for (int i = 0; i < img.rows; i++)
	{
		for (int j = 0; j < img.cols; j++)
		{
			NearestVector = Calculate(image.at<Vec3f>(i, j), RGBVectorSpace);
			result.at<Vec3f>(i, j) = RGBVectorSpace[NearestVector];
		}
	}
	result.convertTo(result, CV_16UC3, 255);
	imwrite("Seg.jpg", result);
	waitKey();
	return 0;
}