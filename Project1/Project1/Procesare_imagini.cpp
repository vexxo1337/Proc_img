
#include "Procesare_imagini.h"

Procesare_imagini::Procesare_imagini()
{

}


vector<Mat> Procesare_imagini::extragere_regiuni(Mat imagine_originala, String cale_salvare) {

	// Identific regiunile conectate pe orizontala
	Mat reginiuni;
	// Pentru extragerea randurilor din imagine folosesc o incadrare de tip rectangular (druptunghi cu latimea imaginii originale)
	Mat sablon = getStructuringElement(MORPH_RECT, Size(imagine_originala.size().width, 1));
	// Aplic incadrarea (pattern-ul, sablonul) peste imaginea originala pentru a identifica numarul de randuri din imagine
	morphologyEx(imagine_originala, reginiuni, MORPH_CLOSE, sablon);

	vector<vector<Point> > contur;
	vector<Vec4i> ierarhie;
	// In cadrul fiecarei regiuni identific conturul
	findContours(reginiuni, contur, ierarhie, RETR_CCOMP, CHAIN_APPROX_SIMPLE, Point(0, 0));

	// Creez un nou director in care sa depun randurile
	CreateDirectoryA((cale_salvare + "randuri").c_str(), NULL);

	vector<Mat> rand_img;
	// Pentru fiecare contur identificat salvez o noua imagine
	for (int i = 0; i < contur.size(); i++) {

		Rect dreptunghi = boundingRect(contur[i]);
		Mat rand = Mat(imagine_originala, dreptunghi);

		imwrite(cale_salvare + "randuri\\contur" + to_string(contur.size() - i) + ".jpg", rand);
		rand_img.push_back(rand);

	}
	reverse(rand_img.begin(), rand_img.end());

	return rand_img;
}
vector<Mat> Procesare_imagini::extragere_cuvinte(Mat imagine_originala, String cale_salvare) {

	// Identific regiunile conectate pe orizontala
	Mat cuvant;
	// Pentru extragerea randurilor din imagine folosesc o incadrare de tip rectangular (druptunghi cu latimea imaginii originale)
	Mat sablon = getStructuringElement(MORPH_RECT, Size(20, 1));
	// Aplic incadrarea (pattern-ul, sablonul) peste imaginea originala pentru a identifica numarul de randuri din imagine
	morphologyEx(imagine_originala, cuvant, MORPH_CLOSE, sablon);

	vector<vector<Point> > contur;
	vector<Vec4i> ierarhie;
	// In cadrul fiecarei regiuni identific conturul
	findContours(cuvant, contur, ierarhie, RETR_CCOMP, CHAIN_APPROX_SIMPLE, Point(0, 0));

	// Creez un nou director in care sa depun randurile
	CreateDirectoryA((cale_salvare + "cuvant").c_str(), NULL);

	vector<Mat> cuvant_img;
	// Pentru fiecare contur identificat salvez o noua imagine
	for (int i = 0; i < contur.size(); i++) {

		Rect dreptunghi = boundingRect(contur[i]);
		Mat cuvant = Mat(imagine_originala, dreptunghi);

		imwrite(cale_salvare + "cuvant\\contur" + to_string(contur.size() - i) + ".jpg", cuvant);
		cuvant_img.push_back(cuvant);

	}
	reverse(cuvant_img.begin(), cuvant_img.end());

	return cuvant_img;
}

bool Procesare_imagini::identifica_spatiu(int coord_dr, int coord_st, int medie)
{
	if (coord_dr - coord_st > medie)
		return true;
	else
		return false;
}
vector<int> Procesare_imagini::marimi_spatii(vector<Rect> dr_sort)
{
	//cout << "Rand nou";
	vector<int> spatiu;

	int k1 = 0, k2 = 1;
	while (k2 < (dr_sort.size())) {
		while (dr_sort[k2].height == 0)
			k2++;
		spatiu.push_back(dr_sort[k2].x - (dr_sort[k1].x + dr_sort[k1].width));
		k1 = k2;
		k2++;
	}
	//cout << "\n TOTAL " << spatiu.size() << "\n";
	for (int i = 0; i < spatiu.size(); i++)
		//cout << "SPATIU " << spatiu[i] << " \n";

	return spatiu;
}
int Procesare_imagini::find_pos(vector<int> lista, int valoare) {
	int ret = -1;
	for (int j = 0; j < lista.size(); j++)
		if (lista[j] == valoare)
			return j;
	return ret;
}
int Procesare_imagini::medie(vector<int> dist)
{
	int min = 0, max = 0;

	for (int i = 0; i < dist.size(); i++)
	{
		if (min > dist[i])min = dist[i];
		if (max < dist[i]) max = dist[i];
	}

	return (min + max) / 2;

}
int Procesare_imagini::insereaza_spatiu(Rect dr1, Rect dr2, int limita)
{
	//dr1 - rect curent
	//dr2 - rect urmator

	if (dr2.x - (dr1.x + dr1.width) > limita)
		return 1;
	else
		return 0;


}


vector<Rect> Procesare_imagini::sortare_dr(vector<int> lista, vector<vector<Point> > contours, Mat imagine) {
	vector<Rect> tmp;
	vector<Rect> res;
	Rect dreptunghi;
	for (int i = 0; i < contours.size(); i++)
		tmp.push_back(dreptunghi);
	for (int i = 0; i < contours.size(); i++)
	{
		dreptunghi = boundingRect(contours[i]);
		if (dreptunghi.height > (0.20*imagine.size().height)) {
			dreptunghi.y = 0;
			dreptunghi.height = imagine.size().height;

			int pos = find_pos(lista, dreptunghi.x);
			tmp[pos] = dreptunghi;

		}


	}

	for (int k = 0; k < tmp.size(); k++)
		if (tmp[k].height > 0)
			res.push_back(tmp[k]);

	return res;
}

Mat Procesare_imagini::incadrare(Mat cropped)
{
	Mat bigImage(50, 50, CV_8UC1);
	bigImage.setTo(Scalar(0, 0, 0));
	Mat crop;
	int wp,w,h,hp;
	wp = (50 * 100)/cropped.size().width;
	hp = (50 * 100)/cropped.size().height;
	w = (wp* cropped.size().width) / 100;
	h = (hp* cropped.size().height) / 100;
	resize(cropped, crop, cv::Size(w, h));
	
	
	//daca inaltimea depaseste 50 se face resize la 50 si se incadreaza intr-un dreptunghi de 28x28 pastrand latimea originala
    
	if (cropped.size().width <= 50 && cropped.size().height > 50)
		resize(cropped, crop, cv::Size(cropped.size().width, 50));
	else
		//daca latimea depaseste 50 se face resize la 50 si se incadreaza intr-un dreptunghi de 28x28 pastrand inaltimea originala
		if (cropped.size().width >= 50 && cropped.size().height < 50)
			resize(cropped, crop, cv::Size(50, cropped.size().height));
		else
			// daca si inaltimea si latimea sunt mai mari de 50 se face resize la 28x28 si se incadreaza
			if (cropped.size().width > 50 && cropped.size().height > 50)
				resize(cropped, crop, cv::Size(50, 50));
			else
				//daca si inaltimea si latimea sunt mai mici decat 28x28 nu se face resize si se incadreaza cu marimile originale
				if (cropped.size().width <= 50 && cropped.size().height <= 50)
					resize(cropped, crop, cv::Size(cropped.size().width, cropped.size().height));
					

	int x = crop.size().width;
	int y = crop.size().height;
	Mat insertImage(bigImage, Rect((50 - x) / 2, (50 - y) / 2, x, y));
	crop.copyTo(insertImage);

	return bigImage;
}


void Procesare_imagini::extragere_litere(Mat imagine, int r, String cale) {
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;
	vector<Rect> dr_sortat;
	findContours(imagine, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE, Point(0, 0));
	const int total = contours.size() - 1;



	Rect dreptunghi;
	Mat src;
	Mat outimage(cv::Size(50, 50), CV_8UC1);
	Mat label(cv::Size(26, 1), CV_8UC1);


	String cale_salvare = cale + "litere_" + to_string(r);
	// Creez un nou director in care sa depun literele
	CreateDirectoryA(cale_salvare.c_str(), NULL);


	vector<int> list_start;
	vector<int> list_stop;
	for (int i = 0; i < contours.size(); i++) // iterate through each contour. 
	{
		dreptunghi = boundingRect(contours[i]);
		list_start.push_back(dreptunghi.x);
		list_stop.push_back(dreptunghi.x + dreptunghi.width);

	}
	sort(list_start.begin(), list_start.end());

	dr_sortat = sortare_dr(list_start, contours, imagine);






	// Calcul medie spatiere intre litere
	vector<int> dist = marimi_spatii(dr_sortat);

	int medie2 = medie(dist);
	//cout << "MEIDDIEIEI :" << medie2 << "*****";
	int img_cnt = 0;
	int cuv_cnt = 0;
	// Pentru fiecare contur identificat incerc sa fac un dreptungi care sa il incadreze, iar cu ajutorul lui fac extrag din imaginea originala
	for (int i = 0; i < dr_sortat.size(); i++) // iterate through each contour.
	{
		// Creez o clona a imaginii originale pentru dimensiuni si setez fundal la 255
		src = imagine.clone();
		//imshow("IMG", src);
		src.setTo(Scalar(255, 255, 255));

		Mat smallImage(10, 10, CV_8UC1, Scalar(255, 0, 0));

		
		dreptunghi = dr_sortat[i];
		
		if (true)
		{
			if ((dreptunghi.size().width > 5) && (dreptunghi.size().height > 20)) {
				Mat cropped = Mat(imagine, dreptunghi);



				Mat bigImage = incadrare(cropped);
				//int n_ext = preia_extrem(bigImage, to_string(img_cnt));
				//Mat csv_mat;
				//bigImage.copyTo(csv_mat);
				//ofstream f("D:\\work\\licenta\\input0.csv", std::ofstream::app);

				/*for (int i = 0; i < csv_mat.size().height;i = i++)
					for (int j = 0; j < csv_mat.size().width;j = j++)
					{
						f << csv_mat.at<double>(i, j);
						if (!(i == csv_mat.size().height - 1 && j == csv_mat.size().height - 1))f << ",";
					}
				f << "\n";
				*/
				CreateDirectoryA((cale_salvare + "\\cuv" + to_string(cuv_cnt)).c_str(), NULL); 
				imwrite(cale_salvare + "\\cuv" + to_string(cuv_cnt)+"\\img" + to_string(img_cnt) + ".jpg", bigImage);
				img_cnt++;
			}
			if ((i < dr_sortat.size() - 1) && (dist.at(i)*90/100 > (medie2))) {
				/*Mat smallImage(50, 50, CV_8UC3);
				smallImage.setTo(Scalar(0, 0, 0));

				imwrite(cale_salvare + "\\img" + to_string(img_cnt) + ".jpg", smallImage);
				img_cnt++;*/
				cuv_cnt++;
				img_cnt = 0;
			}


		}

	}
}
int preia_extrem(Mat frame,string index)
{
	
	
	imshow("src1", frame);
	vector< vector <Point> > contours; // Vector for storing contour
	vector< Vec4i > hierarchy;
	Mat dst(frame.rows, frame.cols, CV_8UC1, Scalar::all(0)); //create destination image
	int count = 0;

	cout << "Numeber of contour = " << count << endl;
	imshow("src", frame);




	//Holes after the bitwise AND Operation.
	//imwrite("D:\\work\\licenta\\imgfill\\Result2.jpg" + index + ".jpg", multipleLevelHoles);

	/*Mat img, img2, img3;
	Mat src, src_gray;
	int thresh = 150;
	int max_thresh = 255;

	string source_window = "Source image";
	string corners_window = "Corners detected";
	img = frame;
	img2 = frame;
	Mat dst, dst_norm, dst_norm_scaled;
	dst = Mat::zeros(frame.size(), CV_32FC1);

	/// Detector parameters
	int blockSize = 2;
	int apertureSize = 3;
	double k = 0.04;
	

	/// Detecting corners
	cornerHarris(frame, dst, blockSize, apertureSize, k, BORDER_DEFAULT);

	/// Normalizing
	normalize(dst, dst_norm, 0, 255, NORM_MINMAX, CV_32FC1, Mat());
	convertScaleAbs(dst_norm, dst_norm_scaled);
	Mat gol = Mat::zeros(dst_norm_scaled.size(), CV_32FC1);
	int cnt = 0;
	/// Drawing a circle around corners
	for (int j = 0; j < dst_norm.rows; j++)
	{
		for (int i = 0; i < dst_norm.cols; i++)
		{
			if ((int)dst_norm.at<float>(j, i) > thresh)
			{
				gol.at<float>(i, j) = 255;
				cnt++;
			}
		}
	}
	cout << "COLTURI" << cnt << endl;
	/// Showing the result
	namedWindow(corners_window, WINDOW_AUTOSIZE);
	imwrite("D:\\work\\da\\i"+index+".jpg" , gol);
	imshow(corners_window,gol);

	//finding contours
	vector<vector<Point>> Contours;
	vector<Vec4i> hier;
	
	//finding the contours required
	findContours(img2, Contours, hier, RETR_CCOMP, CHAIN_APPROX_NONE, Point(0, 0));
	cout << Contours.size() << endl;

	//finding the contour of largest area and storing its index
	int lrgctridx = 0;
	int maxarea = 0;
	for (int i = 0; i < Contours.size(); i++)
	{
		double a = contourArea(Contours[i]);
		if (a > maxarea)
		{
			maxarea = a;
			lrgctridx = i;
		}
	}
	//convex hulls
	vector<vector<Point> >hull(Contours.size());
	vector<vector<int> > hullsI(Contours.size());
	vector<vector<Vec4i>> defects(Contours.size());
	for (int i = 0; i < Contours.size(); i++)
	{
		
			convexHull(Contours[i], hull[i], false);
			convexHull(Contours[i], hullsI[i], false);
			if (hullsI[i].size() > 5)
			{
				convexityDefects(Contours[i], hullsI[i], defects[i]);
			}
		
	}
	//REQUIRED contour is detected,then convex hell is found and also convexity defects are found and stored in defects

	if (maxarea > 100) {
		drawContours(frame, hull, lrgctridx, Scalar(2555, 0, 255), 3, 8, vector<Vec4i>(), 0, Point());

		/// Draw convexityDefects
		for (int j = 0; j < defects[lrgctridx].size(); ++j)
		{
			
			
				const Vec4i& v = defects[lrgctridx][j];
				float depth = v[3] / 256;
				if (depth > 10) //  filter defects by depth
				{
					int startidx = v[0]; Point ptStart(Contours[lrgctridx][startidx]);
					int endidx = v[1]; Point ptEnd(Contours[lrgctridx][endidx]);
					int faridx = v[2]; Point ptFar(Contours[lrgctridx][faridx]);

					line(frame, ptStart, ptEnd, Scalar(0, 255, 0), 1);
					line(frame, ptStart, ptFar, Scalar(0, 255, 0), 1);
					line(frame, ptEnd, ptFar, Scalar(0, 255, 0), 1);
					circle(frame, ptFar, 4, Scalar(0, 255, 0), 2);
				}
		}
		
	}

	imshow("output", frame);*/
	return 4;
}
Mat convolution(Mat imagine) {
	using namespace cv;
	using namespace std;
	int DELAY_CAPTION = 1500;
	int DELAY_BLUR = 100;
	int MAX_KERNEL_LENGTH = 31;
	Mat src; Mat dst;
	Mat maxpool(Mat);


	//"D:\\work\\licenta\\imagini\\testA.png"

	const char* filename = "D:\\work\\licenta\\imagini_out\\v2\\litere_0\\img34.jpg";
	src = imagine;
	if (src.empty()) {
		printf(" Error opening image\n");

		
	}
	
	

	ofstream csv("");
	int erosion_type = MORPH_RECT;
	double erosion_size = 0.5;
	Mat element = getStructuringElement(erosion_type,
		Size(2 * erosion_size + 1, 2 * erosion_size + 1),
		Point(erosion_size, erosion_size));
	Mat eroded;
	erode(src, eroded, element);
	imshow("ERODED", eroded);
	GaussianBlur(eroded, dst, Size(3, 3), 0, 0);
	imshow("GAUSS", dst);
	//cout << cv::format(dst, cv::Formatter::FMT_CSV) << std::endl;
	Mat dst1;
	Mat dst2;
	Mat dst3;
	maxpool(dst).copyTo(dst1);
	imshow("MAXPOOL", dst1);
	cout << dst1;
	erode(dst1, eroded, element);
	imshow("ERODED2", eroded);
	GaussianBlur(eroded, dst2, Size(1, 1), 0, 0);
	imshow("GAUSS", dst2);
	//cout << cv::format(dst, cv::Formatter::FMT_CSV) << std::endl;

	maxpool(dst2).copyTo(dst3);
	imshow("MAXPOOL3", dst3);
	
	cout << dst3;

	imwrite("D:\\work\\licenta\\imagini\\testA1.png", dst3);
	cout << endl;
	for (int i = 0; i < dst3.size().height;i = i++)
		for (int j = 0; j < dst3.size().width;j = j++)
		{
			cout << dst3.at<double>(i, j);
			if (!(i == dst3.size().height - 1 && j == dst3.size().height - 1))cout << ",";
		}
	return dst3;
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