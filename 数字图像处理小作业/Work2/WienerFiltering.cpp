#include<opencv2/opencv.hpp>
#include<iostream>
#include<string>
#include<complex>
using namespace std;
using namespace cv;
Mat GaussianNoise(Mat& noise_image)
{
	Mat image = noise_image.clone();
	Mat noise = noise_image.clone();
	randn(noise, 10, 15);
	add(image, noise, image);
	imwrite("I_n.jpg", image);
	return image;
}
void WienerFilter(Mat& ori_image, Mat& blurred_image, Mat& blurred_noise_image, float snr)
{
	//原图傅里叶变换     
	Mat padded(ori_image.rows, ori_image.cols, CV_32FC1);
	int m = getOptimalDFTSize(ori_image.rows);
	int n = getOptimalDFTSize(ori_image.cols);
	copyMakeBorder(ori_image, padded, 0, m - ori_image.rows, 0, n - ori_image.cols, BORDER_REPLICATE, Scalar::all(0));
	Mat planes[] = { Mat_<float>(padded),Mat::zeros(padded.size(),CV_32F) };
	Mat ori_image_fft;
	merge(planes, 2, ori_image_fft);
	dft(ori_image_fft, ori_image_fft, DFT_COMPLEX_OUTPUT);

	//模糊图傅里叶变换
	Mat padded2(blurred_image.rows, blurred_image.cols, CV_32FC1);
	copyMakeBorder(blurred_image, padded2, 0, m - blurred_image.rows, 0, n - blurred_image.cols, BORDER_REPLICATE, Scalar::all(0));
	Mat planes2[] = { Mat_<float>(padded2),Mat::zeros(padded2.size(),CV_32F) };
	Mat blurred_image_fft;
	merge(planes2, 2, blurred_image_fft);
	dft(blurred_image_fft, blurred_image_fft, DFT_COMPLEX_OUTPUT);

	//模糊+噪声图傅里叶变换
	Mat padded3(blurred_noise_image.rows, blurred_noise_image.cols, CV_32FC1);
	copyMakeBorder(blurred_noise_image, padded3, 0, m - blurred_noise_image.rows, 0, n - blurred_noise_image.cols, BORDER_REPLICATE, Scalar::all(0));
	Mat planes3[] = { Mat_<float>(padded3),Mat::zeros(padded3.size(),CV_32F) };
	Mat blurred_noise_image_fft;
	merge(planes3, 2, blurred_noise_image_fft);
	dft(blurred_noise_image_fft, blurred_noise_image_fft, DFT_COMPLEX_OUTPUT);

	//获取退化函数H
	Mat H(ori_image_fft.size(), CV_32FC2);
	for (int i = 0; i < H.rows; i++)
	{
		for (int j = 0; j < H.cols; j++)
		{
			complex<float> t1(blurred_image_fft.at<Vec2f>(i, j)[0], blurred_image_fft.at<Vec2f>(i, j)[1]);
			complex<float> t2(ori_image_fft.at<Vec2f>(i, j)[0], ori_image_fft.at<Vec2f>(i, j)[1]);
			H.at<Vec2f>(i, j)[0] = (t1 / t2).real();
			H.at<Vec2f>(i, j)[1] = (t1 / t2).imag();
		}
	}

	//维纳滤波去模糊
	Mat S(blurred_noise_image_fft.size(), CV_32FC2);
	complex<float> snrComplex(snr, 0);
	for (int i = 0; i < blurred_noise_image_fft.rows; i++)
	{
		for (int j = 0; j < blurred_noise_image_fft.cols; j++)
		{
			complex<float> gComplex(blurred_noise_image_fft.at<Vec2f>(i, j)[0], blurred_noise_image_fft.at<Vec2f>(i, j)[1]);
			complex<float> hComplex(H.at<Vec2f>(i, j)[0] + 0.0001f, H.at<Vec2f>(i, j)[1] + 0.0001f);
			complex<float> t1 = hComplex * conj(hComplex);
			complex<float> t2 = (t1 / hComplex) / (t1 + snrComplex);
			complex<float> result = gComplex * t2;
			S.at<Vec2f>(i, j)[0] = result.real();
			S.at<Vec2f>(i, j)[1] = result.imag();
		}
	}
	//傅里叶反变换
	Mat res;
	idft(S, res, DFT_REAL_OUTPUT);
	normalize(res, res, 0, 1, NORM_MINMAX);//归一化
	res.convertTo(res, CV_16UC3, 255);
	imwrite("I_d.jpg", res);
	//	imshow("result", res);
}
int main()
{
	string imagepath("C:/Users/11039/Documents/Visual Studio 2019/pictures/I_o.jpg");
	Mat img = imread(imagepath, IMREAD_GRAYSCALE);//存储加载的图像的数据
//    resize(img, img, Size(0, 0), 0.5, 0.5, INTER_NEAREST);
	if (img.empty())
	{
		cout << "couldn't find the image" << endl;
	}
	Mat blurred_image, blurred_noise_image;
	GaussianBlur(img, blurred_image, Size(15, 15), 3, 3);//添加高斯模糊
	blurred_noise_image = GaussianNoise(blurred_image);//添加高斯噪声
	WienerFilter(img, blurred_image, blurred_noise_image, 0.33);
	//    imwrite("C:/Users/11039/Documents/Visual Studio 2019/pictures/203blurred.jpg", blurred_image);
	waitKey();
	return 0;
}