// cannypoints2.cpp : Este arquivo contém a função 'main'. A execução do programa começa e termina ali.
//

#include "pch.h"
#include <iostream>
#include <opencv2/opencv.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include <fstream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <numeric>
#include <ctime>
#include <cstdlib>

using namespace std;
using namespace cv;

#define STEP_MAX 20
#define JITTER_MAX 10
#define RAIO_MAX 10
int top_slider = 10;
int top_slider_max = 100;
int STEP = 5;
int JITTER = 3;
int RAIO = 3;

Mat image, frame, points;

int width, height, gray;
int x, y;

void on_trackbar_cannypoints(int, void*) {
	vector<int> yrange;
	vector<int> xrange;

	srand(time(0));

	width = image.cols;
	height = image.rows;

	xrange.resize(height / STEP);
	yrange.resize(width / STEP);

	iota(xrange.begin(), xrange.end(), 0);
	iota(yrange.begin(), yrange.end(), 0);

	for (uint i = 0; i < xrange.size(); i++) {
		xrange[i] = xrange[i] * STEP + STEP / 2;
	}

	for (uint i = 0; i < yrange.size(); i++) {
		yrange[i] = yrange[i] * STEP + STEP / 2;
	}

	points = Mat(height, width, CV_8U, Scalar(255));

	random_shuffle(xrange.begin(), xrange.end());

	for (auto i : xrange) {
		random_shuffle(yrange.begin(), yrange.end());
		for (auto j : yrange) {
			x = i + rand() % (2 * JITTER) - JITTER + 1;
			y = j + rand() % (2 * JITTER) - JITTER + 1;
			gray = image.at<uchar>(x, y);
			circle(points,
				cv::Point(y, x),
				RAIO,
				CV_RGB(gray, gray, gray),
				-1,
				CV_AA);
		}
	}
	imwrite("points.jpg", points);

	int execs = 5;

	for (int i = 0; i < execs; i++) {
		Mat border;
		int ratio = 5;
		int kernel_size = 5;
		Canny(image, border, top_slider, ratio*top_slider, kernel_size);
		imshow("canny", border);
		for (int j = 0; j < height; j++) {
			for (int k = 0; k < width; k++) {
				if (border.at<uchar>(j, k) > 0) {
					gray = image.at<uchar>(j, k);
					circle(points, Point(k, j), RAIO, CV_RGB(gray, gray, gray), -1, CV_AA);
				}
			}
		}
	}
	imshow("cannypoints", points);
	waitKey();
}

int main(int argc, char** argv) {

	image = imread("paisagem.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	namedWindow("cannypoints", 1);
	namedWindow("canny", 1);

	createTrackbar("Threshold inferior", "cannypoints",
		&top_slider,
		top_slider_max, 
		on_trackbar_cannypoints);

	/*createTrackbar("Raio", "cannypoints",
		&RAIO,
		RAIO_MAX,
		on_trackbar_cannypoints);

	createTrackbar("Jitter", "cannypoints",
		&JITTER,
		JITTER_MAX,
		on_trackbar_cannypoints);

	createTrackbar("passo", "cannypoints",
		&STEP,
		STEP_MAX,
		on_trackbar_cannypoints);
*/
	on_trackbar_cannypoints(top_slider, 0);

	imwrite("cannypoints.jpg", points);
	return 0;
}

// Executar programa: Ctrl + F5 ou Menu Depurar > Iniciar Sem Depuração
// Depurar programa: F5 ou menu Depurar > Iniciar Depuração

// Dicas para Começar: 
//   1. Use a janela do Gerenciador de Soluções para adicionar/gerenciar arquivos
//   2. Use a janela do Team Explorer para conectar-se ao controle do código-fonte
//   3. Use a janela de Saída para ver mensagens de saída do build e outras mensagens
//   4. Use a janela Lista de Erros para exibir erros
//   5. Ir Para o Projeto > Adicionar Novo Item para criar novos arquivos de código, ou Projeto > Adicionar Item Existente para adicionar arquivos de código existentes ao projeto
//   6. No futuro, para abrir este projeto novamente, vá para Arquivo > Abrir > Projeto e selecione o arquivo. sln
