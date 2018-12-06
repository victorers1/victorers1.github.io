#include<iostream>
#include<stdlib.h>
#include<opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main(){
    vector<String> end;
    glob("/home/victor/Área de Trabalho/victorers1.github.io/img/biel.png", end, false);
    Mat imagem = imread(end[0], CV_LOAD_IMAGE_GRAYSCALE);
    Mat arte(imagem.size(), CV_8UC1, Scalar(255));

    int cx, cy, cor;

    for(int i=0; i<10000; i++){
        cx = rand()%256;
        cy = rand()%256;
        cor = imagem.at<uchar>(cy,cx);
        circle(arte, Point(cx,cy), 3, Scalar(cor), -1, CV_AA);
    }
    imshow("arte", arte);
    waitKey();
    return 0;

}