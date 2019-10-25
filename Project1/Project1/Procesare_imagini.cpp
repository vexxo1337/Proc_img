
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
