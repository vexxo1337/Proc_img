// convolution.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include "Procesare_imagini.h"
void convolution(){

int DELAY_CAPTION = 1500;
int DELAY_BLUR = 100;
int MAX_KERNEL_LENGTH = 31;
Mat src; Mat dst;
Mat maxpool(Mat);
char window_name[] = "Smoothing Demo";
int display_caption(const char* caption);
int display_dst(int delay);

	//"D:\\work\\licenta\\imagini\\testA.png"

	const char* filename = "D:\\work\\licenta\\imagini_out\\v2\\litere_0\\img34.jpg";
	src = imread(filename, CV_64F);
	if (src.empty()) {
		printf(" Error opening image\n");

		
	}

	ofstream csv("");

	GaussianBlur(src, dst, Size(5, 5), 0, 0);
	imshow("GAUSS", dst);
	//cout << cv::format(dst, cv::Formatter::FMT_CSV) << std::endl;
	Mat dst1;
	Mat dst2;
	Mat dst3;
	maxpool(dst).copyTo(dst1);
	imshow("MAXPOOL", dst1);
	cout << dst1;
	GaussianBlur(dst1, dst2, Size(1, 1), 0, 0);
	imshow("GAUSS", dst2);
	//cout << cv::format(dst, cv::Formatter::FMT_CSV) << std::endl;

	maxpool(dst2).copyTo(dst3);
	imshow("MAXPOOL", dst3);
	cout << cv::format(dst3, cv::Formatter::FMT_CSV) << std::endl;
	cout << dst3;

	imwrite("D:\\work\\licenta\\imagini\\testA1.png", dst3);
	cout << endl;
	for (int i = 0; i < dst3.size().height;i = i++)
		for (int j = 0; j < dst3.size().width;j = j++)
		{
			cout << dst3.at<double>(i, j);
			if (!(i == dst3.size().height - 1 && j == dst3.size().height - 1))cout << ",";
		}

}

Mat maxpool(Mat img)
{
	int w, h;
	double maxVal;
	int k, l;
	w = img.size().width / 2;
	h = img.size().height / 2;
	Mat out(Size(w, h), CV_64F);
	cout << w << h;






	k = 0;
	l = 0;


	cv::Mat source = img;


	for (int i = 0; i < img.size().height;i = i + 2)
		for (int j = 0; j < img.size().width;j = j + 2)
		{


			Rect roi(i, j, 2, 2);

			// Crop
			Mat crop = img(roi);

			// Show result
			imshow("Original", img);
			imshow("Crop", crop);
			//cout << cv::format(crop, cv::Formatter::FMT_CSV) << std::endl;
			//cout << crop.size().width;
			//cout << crop.size().height;

			double min, max;
			minMaxIdx(crop, &min, &max);



			if (k < w)
			{
				if (l < h)
				{
					cout << max << "MAX" << endl;

					out.at<double>(l, k) = max;
					l++;
				}
				if (l == w)
				{
					k++;
					l = 0;
				}
			}

		}
	return out;
}