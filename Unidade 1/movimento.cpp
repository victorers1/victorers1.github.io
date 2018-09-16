/**
 * Descrição do exercício: https://agostinhobritojr.github.io/tutorial/pdi/#_exerc%C3%ADcios_3
 */

#include <opencv2/opencv.hpp>
#include <iostream>
#include <cmath>

using namespace std;
using namespace cv;

Mat criaHist(Mat const imagem, int bins){
    int histSize[] = {bins}; // 

    float lranges[] = {0, 256}; // Vetor de float com os valores lranges[0] = 0.0 e lranges[1] = 256.0
    const float* ranges[] = {lranges}; // Ponteiro para um vetor de float inicializado com o valor ranges[0] = &lranges

    Mat hist;
    int channels[]={0}; // Vetor de inteiros inicializado com channels[0] = 0 e só

    
    calcHist(&imagem, 1, channels, Mat(), hist, 1, histSize, ranges, true, false);
    return hist;
}

Mat3b histImage(Mat const hist, int bins){
    int const hist_height = 256;
    Mat3b hist_image = Mat3b::zeros(hist_height, bins); // Matriz na qual cada pixels tem 3 Bytes inicializada com 0 em todas posições
                                               //bins = largura do histograma

    double max_val = 0;
    minMaxLoc(hist, 0, &max_val);

    // Imprimindo cada retângulo no fundo preto
    for(int b=0; b<256; b++){
        float const binVal = hist.at<float>(b);
        int const height = cvRound(binVal*hist_height/max_val);
        line(hist_image, Point(b, hist_height-height),
         Point(b, hist_height), Scalar::all(255));  
    }
    return hist_image;
}

int* histAcum(Mat const hist, int t){ // Retorna o histograma acumulado. Recebe um vetor de inteiros e seu tamanho
    int* vetor = new int[t]; // Aloca dinamicamente um vetor de inteiros com tamanho 't'
    vetor[0] = hist.at<float>(0);
    for(int i=1; i<t; i++) vetor[i] = hist.at<float>(i) + vetor[i-1];
    
    return vetor;
}

float relacao(Mat const hist1, Mat const hist2){
    float temp=0.0;
    for(int i=0; i<hist1.cols; i++){
        temp += pow(hist1.at<float>(i)-hist2.at<float>(i), 2);
    }

    return temp/hist1.cols;
}

int main(){
    Mat imagem; // Imagem capturada pela webcam
    int width=0, height=0; // Largura e altura da captura, respesctivamente
    VideoCapture cap; // Objeto capturador

    cap.open(0);
    if(!cap.isOpened()){
        cout << "cameras indisponiveis";
        return -1;
    }
    width  = cap.get(CV_CAP_PROP_FRAME_WIDTH);
    height = cap.get(CV_CAP_PROP_FRAME_HEIGHT);
    cout << "largura = " << width << endl;
    cout << "altura  = " << height << endl;
    cap>>imagem;

    Mat hist_novo = criaHist(imagem, 256);
    Mat hist_velho = criaHist(imagem, 256); // Só para ter mesmo tamanho e tipo de hist_novo
    while(1){
        cap >> imagem;
        cvtColor(imagem, imagem, CV_RGB2GRAY); // Transforma "imagem" numa imagem em escala de cinza

        hist_novo.copyTo(hist_velho);
        hist_novo = criaHist(imagem, 256);
        
        float rel = relacao(hist_novo, hist_velho);
        cout<<"relacao entre novo e velho= "<<rel<<endl;

        imshow("Original", imagem);
        imshow("Histograma do original", histImage(hist_novo, 256));
        if(waitKey(30) >= 0) break;
    }           
    return 0;
}