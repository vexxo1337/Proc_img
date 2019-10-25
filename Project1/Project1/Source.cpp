#include "Procesare_imagini.h"


//extern "C" {_declspec(dllexport) int imagine(const char *input)
int main()
{

	Mat imagine_originala;
	Mat image_output;
	String litere = { 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z' };
	String path_input = "D:\\work\\licenta\\imagini\\test2805.jpg";
	String path_output= "D:\\work\\licenta\\imagini_out\\v2\\";
	String path_output_l = "D:\\work\\licenta\\imagini_out\\v1\\";
	String out_csv;
	
	
	for (int i = 2;i < 3;i++) {
		//cout << "LITERA:" << litere[i] << endl;
		out_csv = "D:\\work\\licenta\\input.csv";

		//+litere[i] + ".jpg"

		String fisier(path_input);

		system(("rmdir /Q /S " + (path_output)).c_str());
		CreateDirectoryA((path_output).c_str(), NULL);
		system(("rmdir /Q /S " + (path_output_l)).c_str());
		CreateDirectoryA((path_output_l).c_str(), NULL);

		imagine_originala = imread(fisier);
		//namedWindow("Display window");
		Mat cpy;
		resize(imagine_originala, cpy, Size(1080, 720));
		imshow("Display window 1", cpy);
		Mat new_image = Mat::zeros(imagine_originala.size(), imagine_originala.type());
		double alpha = 1.0; /*< Simple contrast control */
		int beta = 5;       /*< Simple brightness control */

		imagine_originala.convertTo(new_image, -1, alpha, beta);
		//Mat cropedImage = imagine_originala(Rect(250,250, 2300, 3250));
		//resize(cropedImage, cropedImage, Size(1600, 2200));
		//imshow("THE NAME", cropedImage);
		Mat den;
		//imshow("NE", new_image);
		/*// Reaimshod image
		Mat im = imread(path_input + "test1.png", IMREAD_GRAYSCALE);

		// Set up the detector with default parameters.
		SimpleBlobDetector detector;

		// Detect blobs.
		std::vector<KeyPoint> keypoints;
		detector.detect(im, keypoints);

		// Draw detected blobs as red circles.
		// DrawMatchesFlags::DRAW_RICH_KEYPOINTS flag ensures the size of the circle corresponds to the size of blob
		Mat im_with_keypoints;
		drawKeypoints(im, keypoints, im_with_keypoints, Scalar(0, 0, 255), DrawMatchesFlags::DRAW_RICH_KEYPOINTS);

		// Show blobs
		imshow("keypoints", im_with_keypoints);

		*/





		Procesare_imagini pi;




		Mat im_gray;   // no nead to load the Mat with anything when declaring it.
		cvtColor(imagine_originala, im_gray, COLOR_RGB2GRAY);
		// INSTEAD of the above two lines you could have cv::Mat im_gray = imread("img1.png",CV_LOAD_IMAGE_GRAYSCALE);

		// the following is an alternative to Mat img_bw = im_gray > 128
		Mat img_bw;
		threshold(im_gray, img_bw, 230, 255.0, THRESH_BINARY || THRESH_OTSU);
		resize(img_bw, cpy, Size(1080, 720));
		imshow("image_bw.jpg", cpy);

		double dilation_size = 1;
		Mat element = getStructuringElement(MORPH_RECT,
			Size(2 * dilation_size + 1, 2 * dilation_size + 1),
			Point(dilation_size, dilation_size));

		dilate(img_bw, img_bw, element);
		resize(img_bw, cpy, Size(1080, 720));
		imshow("image_dil.jpg", cpy);
		// Extrag randurile din imagine
		vector<Mat> r_img = pi.extragere_regiuni(img_bw, path_output);
		//extragere cuvinte
		pi.extragere_cuvinte(img_bw, path_output);
		//imshow("TEST", img_bw);
		// Pentru fiecare imagine salvata anterior in folderul numit rand, extrag litera cu litera
		for (int i = 0; i < r_img.size(); i++) {

			pi.extragere_litere(r_img[i], i, path_output_l);
		}
	}
	cout << "gata";
	waitKey(0);
	
	return 0;
}

//}