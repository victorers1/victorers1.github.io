#include <iostream>
#include <opencv2/opencv.hpp>
#include <cmath>
using namespace cv;
using namespace std;

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


int main(int argvc, char** argv){
  VideoCapture cap;
  Mat quadro, saida;
  cap.open("/home/victor/Área de Trabalho/victorers1.github.io/img/tiltcond.mp4");
  if(!cap.isOpened()) return -1;
  else cout<<"abriu leitor\n";

  //VideoWriter(const string& filename, int fourcc, double fps, Size frameSize, bool isColor=true)
  VideoWriter video("tiltsaida.mp4", CV_FOURCC('M','J','P','G'), 30, quadro.size(), true);
  if(!video.isOpened()) return -1;
  else cout<<"abriu escritor\n";

  while(true){
    cap>>quadro;
    if(!quadro.data) break;
    saida = tiltshift(quadro, quadro.rows/4, 20);
    imshow("saida", saida);
    waitKey(33);
    video.write(saida);
  }

  return 0;
}
