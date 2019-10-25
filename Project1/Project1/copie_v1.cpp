#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <queue>
#include <vector>
#include <string>
#include <windows.h>
#include <direct.h>
#include <stdio.h>



using namespace cv;
using namespace std;

vector<Mat> extragere_regiuni(Mat imagine_originala, String cale_salvare) {

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
vector<Mat> extragere_cuvinte(Mat imagine_originala, String cale_salvare) {

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

bool identifica_spatiu(int coord_dr, int coord_st,int medie)
{
	if (coord_dr - coord_st > medie)
		return true;
	else
		return false;
}
vector<int> marimi_spatii(vector<Rect> dr_sort)
{
	cout << "Rand nou";
	vector<int> spatiu;

	int k1 = 0, k2 = 1;
	while (k2 < (dr_sort.size())) {
		while (dr_sort[k2].height == 0)
			k2++;
		spatiu.push_back(dr_sort[k2].x - (dr_sort[k1].x + dr_sort[k1].width));
		k1 = k2;
		k2++;
	}
	cout << "\n TOTAL " << spatiu.size() << "\n";
	for (int i = 0; i < spatiu.size(); i++)
		cout << "SPATIU " << spatiu[i] << " \n";

	return spatiu;
}
int find_pos(vector<int> lista, int valoare) {
	int ret = -1;
	for (int j = 0; j < lista.size(); j++)
		if (lista[j] == valoare)
			return j;
	return ret;
}
int medie(vector<int> dist)
{
	int min = 0, max = 0;

	for (int i = 0; i < dist.size(); i++)
	{
		if (min > dist[i])min = dist[i];
		if (max < dist[i]) max = dist[i];
	}

	return (min + max) / 2;

}
int insereaza_spatiu(Rect dr1, Rect dr2, int limita)
{
	//dr1 - rect curent
	//dr2 - rect urmator

	if (dr2.x - (dr1.x + dr1.width) > limita)
		return 1;
	else
		return 0;
	

}


vector<Rect> sortare_dr(vector<int> lista, vector<vector<Point> > contours,Mat imagine) {
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

Mat incadrare(Mat cropped)
{
	Mat bigImage(28, 28, CV_8UC1);
	bigImage.setTo(Scalar(0, 0, 0));
	Mat crop;
	//daca inaltimea depaseste 28 se face resize la 28 si se incadreaza intr-un dreptunghi de 28x28 pastrand latimea originala

	if (cropped.size().width <= 28 && cropped.size().height > 28)
		resize(cropped, crop, cv::Size(cropped.size().width, 28));
	else
		//daca latimea depaseste 28 se face resize la 28 si se incadreaza intr-un dreptunghi de 28x28 pastrand inaltimea originala
		if (cropped.size().width >= 28 && cropped.size().height < 28)
			resize(cropped, crop, cv::Size(28, cropped.size().height));
		else
			// daca si inaltimea si latimea sunt mai mari de 28 se face resize la 28x28 si se incadreaza
			if (cropped.size().width > 28 && cropped.size().height > 28)
				resize(cropped, crop, cv::Size(28, 28));
			else
				//daca si inaltimea si latimea sunt mai mici decat 28x28 nu se face resize si se incadreaza cu marimile originale
				if (cropped.size().width <= 28 && cropped.size().height <= 28)
					resize(cropped, crop, cv::Size(cropped.size().width, cropped.size().height));

	int x = crop.size().width;
	int y = crop.size().height;
	Mat insertImage(bigImage, Rect(0, 0, x, y));
	crop.copyTo(insertImage);

	return bigImage;
}
void extragere_litere(Mat imagine, int r, String cale) {
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;
	vector<Rect> dr_sortat;
	findContours(imagine, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE, Point(0, 0));
	const int total = contours.size() - 1;

	
	Rect dreptunghi;
	Mat src;
	Mat outimage(cv::Size(28, 28), CV_8UC1);

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
	cout << "MEIDDIEIEI :" << medie2 << "*****";
	int img_cnt = 0;
	// Pentru fiecare contur identificat incerc sa fac un dreptungi care sa il incadreze, iar cu ajutorul lui fac extrag din imaginea originala
	for (int i = 0; i < dr_sortat.size(); i++) // iterate through each contour.
	{
		// Creez o clona a imaginii originale pentru dimensiuni si setez fundal la 255
		src = imagine.clone();
		src.setTo(Scalar(255, 255, 255));
		
		Mat smallImage(10, 10, CV_8UC1, Scalar(255, 0, 0));
		
		
		dreptunghi = dr_sortat[i];


		if ((dreptunghi.size().width > 0) && (dreptunghi.size().height > 0)) {
			Mat cropped = Mat(imagine, dreptunghi);
			Mat bigImage = incadrare(cropped);
			imwrite(cale_salvare + "\\img" + to_string(img_cnt) + ".jpg", bigImage);
			img_cnt++;
		}
		if ((i < dr_sortat.size() - 1) && (dist.at(i) > (medie2))) {
			Mat smallImage(28, 28, CV_8UC3);
			smallImage.setTo(Scalar(0, 0, 0));
			imwrite(cale_salvare + "\\img" + to_string(img_cnt) + ".jpg", smallImage);
			img_cnt++;
		}
			
	
		}
	}



int main(int argc, char** argv)

{

	Mat imagine_originala;
	Mat image_output;

	String path_input("D:\\work\\licenta\\imagini\\");
	String path_output("D:\\work\\licenta\\imagini_out\\v2\\");
	String fisier(path_input + "imgtest.png");

	system(("rmdir /Q /S "+(path_output)).c_str());
	//system("rm -r D:\\work\\licenta\\imagini_out\\v2\\");

	/*_rmdir((path_output).c_str());
	*/
	CreateDirectoryA((path_output).c_str(),NULL);

	imagine_originala = imread(fisier);
	namedWindow("Display window", WINDOW_AUTOSIZE);
	imshow("Display window 1", imagine_originala);

	Mat hist = imread(fisier, IMREAD_GRAYSCALE);

	// https://docs.opencv.org/2.4/doc/tutorials/imgproc/erosion_dilatation/erosion_dilatation.html
	int dilation_size = 2;
	Mat element = getStructuringElement(MORPH_RECT,
		Size(2 * dilation_size + 1, 2 * dilation_size + 1),
		Point(dilation_size, dilation_size));

	erode(imagine_originala, image_output, element);
	imshow("Display window 2", image_output);




	cv::Mat im_gray;   // no nead to load the Mat with anything when declaring it.
	cv::cvtColor(image_output, im_gray, COLOR_RGB2GRAY);
	// INSTEAD of the above two lines you could have cv::Mat im_gray = imread("img1.png",CV_LOAD_IMAGE_GRAYSCALE);

	// the following is an alternative to Mat img_bw = im_gray > 128
	cv::Mat img_bw;
	cv::threshold(im_gray, img_bw, 105, 255.0, THRESH_BINARY || THRESH_OTSU);

	cv::imshow("image_bw.jpg", img_bw);

	// Extrag randurile din imagine
	vector<Mat> r_img = extragere_regiuni(img_bw, path_output);
	//extragere cuvinte
	extragere_cuvinte(img_bw, path_output);

	// Pentru fiecare imagine salvata anterior in folderul numit rand, extrag litera cu litera
	for (int i = 0; i < r_img.size(); i++) {
		extragere_litere(r_img[i], i, path_output);
	}

	waitKey(0);
	return 0;

}
