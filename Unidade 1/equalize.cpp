/**
 * Descrição do exercício: https://agostinhobritojr.github.io/tutorial/pdi/#_exerc%C3%ADcios_3
 */

#include <opencv2/opencv.hpp>
#include <iostream>

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
    vector<String> end;
    glob("/home/victor/Área de Trabalho/OpenCV_Codes/img/Ursos.jpg", end, false);

    Mat imagem = imread(end[0], CV_LOAD_IMAGE_GRAYSCALE);
    Mat equalizado;
    equalizeHist(imagem, equalizado);

    imshow("Original", imagem); // Mostra original
    imshow("Histograma do original", histImage(criaHist(imagem, 256), 256)); // Cria o vetor histograma e depois a imagem dele
    
    imshow("Imagem Equalizada", equalizado);
    imshow("Histograma da equalizada", histImage(criaHist(equalizado, 256), 256));
    waitKey();
    return 0;
}