#include <opencv2/opencv.hpp>
#include <iostream>
#include <cmath>
using namespace std;
using namespace cv;

/**
 * Recebe uma imagem e a quantidade debarras do histograma desejado
 * Retorna o histograma
 */
Mat criaHist(Mat const imagem, int bins){
    int histSize[] = {bins}; // 

    float lranges[] = {0, 256}; // Vetor de float com os valores lranges[0] = 0.0 e lranges[1] = 256.0
    const float* ranges[] = {lranges}; // Ponteiro para um vetor de float inicializado com o valor ranges[0] = &lranges

    Mat hist;
    int channels[]={0}; // Vetor de inteiros inicializado com channels[0] = 0 e só

    
    calcHist(&imagem, 1, channels, Mat(), hist, 1, histSize, ranges, true, false);
    return hist;
}

/**
 * Recebe um histograma e a quantidade de barras nele
 * Retorna uma imagem do histograma
 */
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

int main(){
    Mat imagem, hist_novo, hist_anterior; // Imagem capturada pela webcam
    int temp=0;
    float comparacao;
    VideoCapture cap; // Objeto capturador
    cap.open(0);
    if(!cap.isOpened()){
        cout << "cameras indisponiveis";
        return -1;
    }

    cap >> imagem;
    hist_novo = criaHist(imagem, 256);
    while(1){
        hist_novo.copyTo(hist_anterior);
        cap >> imagem;
        cvtColor(imagem, imagem, CV_RGB2GRAY); //Imagem agora é monocromática
        hist_novo = criaHist(imagem, 256);
        
        comparacao = compareHist(hist_novo, hist_anterior, CV_COMP_BHATTACHARYYA);
        if(comparacao >= 0.04) cout<<++temp<<" movimento detectado\n";
        
        imshow("webcam", imagem);
        if(waitKey(30) >= 0) break;
    }
    return 0;
}