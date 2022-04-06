#define _CRT_SECURE_NO_WARNINGS //必須在include前
#include <cstdio>                   
#include<opencv2\core.hpp>
#include<opencv2\highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <cmath>

using namespace cv;
using namespace std;

//Highboost_Frequency
void fftshift(const Mat& input_img, Mat& output_img)
{
	output_img = input_img.clone();
	int cx = output_img.cols / 2;
	int cy = output_img.rows / 2;
	Mat q1(output_img, Rect(0, 0, cx, cy));
	Mat q2(output_img, Rect(cx, 0, cx, cy));
	Mat q3(output_img, Rect(0, cy, cx, cy));
	Mat q4(output_img, Rect(cx, cy, cx, cy));

	Mat temp;
	q1.copyTo(temp);
	q4.copyTo(q1);
	temp.copyTo(q4);
	q2.copyTo(temp);
	q3.copyTo(q2);
	temp.copyTo(q3);
}

void calculateDFT(Mat& scr, Mat& dst)
{
	// define mat consists of two mat, one for real values and the other for complex values
	Mat planes[] = { scr, Mat::zeros(scr.size(), CV_32F) };
	Mat complexImg;
	merge(planes, 2, complexImg);

	dft(complexImg, complexImg);
	dst = complexImg;
}

Mat construct_H(Mat& scr, float D0)
{
	Mat H(scr.size(), CV_32F, Scalar(1));
	float D = 0;
	float A = 1.5;
	for (int u = 0; u < H.rows; u++)
	{
		float gaussian = 0;
		for (int v = 0; v < H.cols; v++)
		{
			//gaussian low pass filter
			D = sqrt((u - scr.rows / 2) * (u - scr.rows / 2) + (v - scr.cols / 2) * (v - scr.cols / 2));
			gaussian = exp(-D * D / (2 * D0 * D0));
			// unsharping masking
			H.at<float>(u, v) = (A - 1) + gaussian;
		}
	}
	return H;
}

void filtering(Mat& scr, Mat& dst, Mat& H)
{
	fftshift(H, H);
	Mat planesH[] = { Mat_<float>(H.clone()), Mat_<float>(H.clone()) };

	Mat planes_dft[] = { scr, Mat::zeros(scr.size(), CV_32F) };
	split(scr, planes_dft);

	Mat planes_out[] = { Mat::zeros(scr.size(), CV_32F), Mat::zeros(scr.size(), CV_32F) };
	planes_out[0] = planesH[0].mul(planes_dft[0]);
	planes_out[1] = planesH[1].mul(planes_dft[1]);

	merge(planes_out, 2, dst);

}

//Highboost_Space
int mask2[3][3] = { 0, -1, 0, -1, 4, -1, 0, -1, 0 }; //  Laplacian mask 四鄰域
int mask1[3][3] = { -1, -1, -1, -1, 8, -1, -1, -1, -1 }; //  Laplacian mask 八鄰域

void Highboost_Space(Mat src, Mat dst,float A ,int mask[3][3])
{
	for (int i = 1; i < src.rows - 1; i++)
	{
		for (int j = 1; j < src.cols - 1; j++)
		{
			//saturate_cast函數的作用即是:當運算完後，結果為負，則轉為0，結果超出255，則為255
			dst.at<uchar>(i, j) = saturate_cast<uchar>(mask[1][1] * src.at<uchar>(i, j) +
				(mask[0][0] * src.at<uchar>(i - 1, j - 1) + mask[1][0] * src.at<uchar>(i, j - 1) + mask[2][0] * src.at<uchar>(i + 1, j - 1) +
					mask[0][1] * src.at<uchar>(i - 1, j) + mask[2][1] * src.at<uchar>(i + 1, j) +
					mask[0][2] * src.at<uchar>(i - 1, j + 1) + mask[1][2] * src.at<uchar>(i, j + 1) + mask[2][2] * src.at<uchar>(i + 1, j + 1)));
			//將算出來的拉普拉斯與原本值相加存回去，A>=1的浮點數
			dst.at<uchar>(i, j) = saturate_cast<uchar>((A * src.at<uchar>(i, j)) + dst.at<uchar>(i, j));
	
		}
	}
}

int main() {
	/*************************	moon.bmp	***************************/
	Mat src1 = imread("blurry_moon.tif", 0);
	Mat des1 = imread("blurry_moon.tif", 0);
	Mat src2 = imread("blurry_moon.tif", 0);
	//Highboost_frequency
	src2.convertTo(src2, CV_32F);
	// DFT
	Mat DFT_image1;
	calculateDFT(src2, DFT_image1);
	// construct H
	Mat H1;
	H1 = construct_H(src2, 0.5);
	// filtering
	Mat complexIH1;
	filtering(DFT_image1, complexIH1, H1);
	// IDFT
	Mat des2;
	dft(complexIH1, des2, DFT_INVERSE | DFT_REAL_OUTPUT);
	normalize(des2, des2, 0, 1, NORM_MINMAX);
	// Highboost_space
	Highboost_Space(src1, des1, 1.1,mask1);
	// 顯示結果
	imshow("blurry_moon_Original", src1);
	imshow("blurry_moon_Highboost_Space", des1);
	imshow("blurry_moon_Highboost_Frequency", des2);

	/*************************	skeleton_orig.bmp	***************************/
	Mat src3 = imread("skeleton_orig.bmp", 0);
	Mat des3 = imread("skeleton_orig.bmp", 0);
	Mat src4 = imread("skeleton_orig.bmp", 0);
	//Highboost_frequency
	src4.convertTo(src4, CV_32F);
	// DFT
	Mat DFT_image2;
	calculateDFT(src4, DFT_image2);
	// construct H
	Mat H2;
	H2 = construct_H(src4, 0.5);
	// filtering
	Mat complexIH2;
	filtering(DFT_image2, complexIH2, H2);
	// IDFT
	Mat des4;
	dft(complexIH2, des4, DFT_INVERSE | DFT_REAL_OUTPUT);
	normalize(des4, des4, 0, 1, NORM_MINMAX);
	// Highboost_space
	Highboost_Space(src3, des3, 1.3,mask2);
	// 顯示結果
	imshow("skeleton_original", src3);
	imshow("skeleton_Highboost_Space", des3);
	imshow("skeleton_Highboost_Frequency", des4);

	waitKey(0);
	return 0;
}
