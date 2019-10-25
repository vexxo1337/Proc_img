#pragma once


#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include <opencv2/features2d.hpp>
#include <opencv2/photo.hpp>
#include <iostream>
#include <queue>
#include <vector>
#include <string>
#include <windows.h>
#include <direct.h>
#include <stdio.h>
#include <fstream>


using namespace cv;
using namespace std;
_declspec(dllexport) Mat convolution(Mat);
_declspec(dllexport) int preia_extrem(Mat,string);
class Procesare_imagini
{

	public:
		
		
		_declspec(dllexport)Procesare_imagini();
		
		_declspec(dllexport)vector<Mat> extragere_regiuni(Mat , String );
		_declspec(dllexport)vector<Mat> extragere_cuvinte(Mat , String );
		_declspec(dllexport)bool identifica_spatiu(int , int , int );
		_declspec(dllexport)int find_pos(vector<int> , int );
		_declspec(dllexport)int medie(vector<int> );
		_declspec(dllexport)int insereaza_spatiu(Rect , Rect , int );
		_declspec(dllexport)vector<Rect> sortare_dr(vector<int> , vector<vector<Point> > , Mat );
		_declspec(dllexport)Mat incadrare(Mat );
		_declspec(dllexport)void extragere_litere(Mat , int , String );
		_declspec(dllexport)vector<int> marimi_spatii(vector<Rect> );
		
};

