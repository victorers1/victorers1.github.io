/*
Descrição do execício 2: https://agostinhobritojr.github.io/tutorial/pdi/#_exerc%C3%ADcios
*/

#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

int main(){
    vector<String> end;
    glob("/home/victor/Área de Trabalho/OpenCV_Codes/img/biel.png", end, false);

    Mat original = imread(end[0], CV_LOAD_IMAGE_COLOR);
    Mat result(Size(original.rows, original.cols), CV_8UC3);
    // Declarando um retângulo:  Rect retangulo(x,y, largura, altura);
    //Declarando as regiões da imagem original
    Mat SE(original, Rect(0,0,original.cols/2, original.rows/2)); // Região superior esquerda
    Mat SD(original, Rect(original.cols/2, 0, original.cols/2, original.rows/2)); // Região superior direita
    Mat IE(original, Rect(0, original.rows/2, original.cols/2, original.rows/2)); // Região inferior esquerda
    Mat ID(original, Rect(original.cols/2, original.rows/2, original.cols/2, original.rows/2)); // Região inferior direita

    /*
    SE = Scalar(0, 255, 0); // define a cor desta região como verde
    ID = Scalar(255, 0, 0); // define a cor desta regiã ocomo azul (lembrar do padrão B.G.R.)
    SD = Scalar(0, 0, 0);
    IE = Scalar(0, 0, 255);
    */

    ID.copyTo(result(Rect(0, 0, ID.cols, ID.rows))); // ID -> SE
    SE.copyTo(result(Rect(original.cols/2, original.rows/2, SE.cols, SE.rows))); // SE -> ID
    IE.copyTo(result(Rect(original.cols/2, 0, IE.cols, IE.rows))); // IE -> SD
    SD.copyTo(result(Rect(0, original.rows/2, SD.cols, SD.rows))); // SD -> IE
    
    namedWindow("Original", CV_WINDOW_AUTOSIZE); // Janela de tamanho fixo
    namedWindow("Regiões Trocadas", CV_WINDOW_FREERATIO); // Janela ajustável

    imshow("Original", original);
    imshow("Regiões Trocadas", result);
    waitKey();

    imwrite("resultado.png", result);

    return 0;
}