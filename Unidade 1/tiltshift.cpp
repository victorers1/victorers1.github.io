#include <iostream>
#include <opencv2/opencv.hpp>
#include <cmath> // tanh(x)
using namespace cv;
using namespace std;

float larg, deca;
int larg_slider = 1;
int larg_slider_max = 500;
int dec_slider = 2;
int dec_slider_max = 40;
char TrackbarName[50];
Mat tiltado, original;

/**
 * tiltshift recebe uma imagem e os reais 'l' e 'd'.
 * 'l' é a largura do borrão, 'd' é a taxa de decaimento do borrão
 * Retorna a imagem dada com o filtro tiltshift aplicado
 */
Mat tiltshift(Mat imagem, float l, float d){
  Mat result, borrada, orig_depois, borr_depois; // faltou-me criatividade nessa hora
  GaussianBlur(imagem, borrada, Size(7, 7), 0, 0);
  Mat pOrig(imagem.size(), CV_32F); // ponderaOriginal
  Mat pBorr(imagem.size(), CV_32F); // ponderaBorrada
  float l1=l, l2=imagem.rows-l1, tmp;

  for (int x = 0; x < pOrig.rows; x++){
    tmp = 1 / 2.0 * (tanh((x - l1) / d) - tanh((x - l2) / d));
    for (int y = 0; y < pOrig.cols; y++){
      pOrig.at<float>(x, y) = tmp;
      pBorr.at<float>(x, y) = (1 - tmp);
    }
  }
  Mat canal_orig[3], canal_borr[3]; // Separamos os canais da imagem colorida para fazer a multiplicação em cada um
  split(imagem, canal_orig);
  split(borrada, canal_borr);
  for(int i=0; i<3; i++){
    multiply(canal_orig[i], pOrig, canal_orig[i], 1, CV_8UC3); // Matriz resultante será do tipo CV_8UC3
    multiply(canal_borr[i], pBorr, canal_borr[i], 1, CV_8UC3); // 8UC3C = 3 canais de unsigned char de 8 bits
  }
  merge(canal_orig, 3, orig_depois);
  merge(canal_borr, 3, borr_depois);
  //addWeighted(fonte1, peso1, fonte2, peso2, gamma, destino)
  addWeighted(orig_depois, 1, borr_depois, 1, 0, result);
  return result;
}

void on_trackbar_l(int, void*){
  if(larg_slider<1 || larg_slider>original.rows/2) return;
  larg = (float) larg_slider;
  cout<<"larg="<<larg<<"\n";
  tiltado = tiltshift(original, larg, deca);
  imshow("resultado", tiltado);
}

void on_trackbar_d(int, void*){
  if(dec_slider<1) return; // deca não pode ser 0
  deca = (float) dec_slider;
  cout<<"deca="<<deca<<"\n";
  tiltado = tiltshift(original, larg, deca);
  imshow("resultado", tiltado);
}

int main(int argvc, char** argv){
  vector<String> end;
  glob("/home/victor/Área de Trabalho/victorers1.github.io/img/tiltpraia.jpg", end, false);
  original = imread(end[0], CV_LOAD_IMAGE_COLOR);
  larg_slider_max = original.rows/2;

  namedWindow("resultado", CV_WINDOW_FREERATIO);
  imshow("resultado", original);

  sprintf(TrackbarName, "Largura do borrão");
  createTrackbar(TrackbarName, "resultado",
				  &larg_slider,
				  larg_slider_max,
				  on_trackbar_l); // Largura do borramento
  
  sprintf(TrackbarName, "Decaimento do borrão");
  createTrackbar(TrackbarName, "resultado",
				  &dec_slider,
				  dec_slider_max,
				  on_trackbar_d); // Decaimento do borramento

  waitKey();
  return 0;
}