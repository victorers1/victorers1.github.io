/**
 * Exercício 3: https://agostinhobritojr.github.io/tutorial/pdi/#_exerc%C3%ADcios_2
 */
#include <opencv2/opencv.hpp>
#include <iostream>
#include <stack>

using namespace std;
using namespace cv;

/**
 * floodfill recebe uma imagem, um ponto e uma escala de cinza.
 * A função pinta a imagem com a cor dada a partir do ponto dado.
 * A extensão da pintura é definida pela região de mesma cor em 
 * que o ponto se encontra antes da execução do algoritmo.
 */
void floodfill(Mat im, CvPoint p, int cor){
    stack<CvPoint> pilha;
    pilha.push(p); // Inicia pilha de pontos com o passado no 3º argumento
    int corAnt = im.at<uchar>(p);
    CvPoint temp; // Ponto temporário. Representa sempre o que está sendo processado atualmente.

    while(!pilha.empty()){ // Atualiza pilha
        temp = pilha.top(); // Recupera  topo
        pilha.pop(); // Remove topo

        /*
        if(pilha.size()%10000==0){
            cout<<"Tamanho da pilha: "<<pilha.size()<<endl;
        }*/

        if (im.at<uchar>(temp)==corAnt){
            im.at<uchar>(temp) = cor; // Recebe nova cor
            if(temp.x >= 1)        pilha.push(CvPoint(temp.x-1, temp.y)); // Vizinho de cima
            if(temp.x+1 < im.rows) pilha.push(CvPoint(temp.x+1, temp.y)); // Vizinho de baixo
            if(temp.y >= 1)        pilha.push(CvPoint(temp.x,   temp.y-1)); // Vizinho da esquerda
            if(temp.y+1 < im.cols) pilha.push(CvPoint(temp.x,   temp.y+1)); // Vizinho da direita
        }
    }
}

/**
 * Retira bolhas da bordas
 */
void filtraBordas(Mat im){
    for(int c=0; c<im.cols; c++){ // Percorre borda superior
        im.at<uchar>(0, c) = 255;
    }
    for(int c=0; c<im.cols; c++){ // Percorre borda inferior
        im.at<uchar>(im.cols-1, c) = 255;
    }
    for(int l=0; l<im.rows; l++){ // Percorre borda esquerda
        // Nesse FOR, l pode começar a partir de 1, pois a primeira linha já está pintada
        // Também podemos fazer l ir só até im.rows-2, pois a última linha já está pintada
        // Para simplicidade, faremos a varredura completa
        im.at<uchar>(l, 0) = 255;
    }
    for(int l=0; l<im.rows; l++){ // Percorre borda direita
        im.at<uchar>(l, im.rows-1) = 255;
    }
    floodfill(im, CvPoint(0,0), 0); // Pinta borda de "im" de preto
}

int main(){
    vector<String> end; // endereço da imagem
    glob("/home/victor/Área de Trabalho/OpenCV_Codes/img/bolhas.png", end, false);
    Mat imagem = imread(end[0], CV_LOAD_IMAGE_GRAYSCALE);
    Mat original = imread(end[0], CV_LOAD_IMAGE_GRAYSCALE);
    filtraBordas(imagem); // Tira as bolhas tocando a borda da imagem
    int qtdBolha=0, qtdBuraco=0;

    imshow("Sem bordas", imagem); // Mostra a imagem sem as bolhas nas bordas
    imwrite("semborda.png", imagem);
    for(int y=0; y<imagem.cols; y++){
        for(int x=0; x<imagem.rows; x++){
            if(imagem.at<uchar>(x, y)==255){
                qtdBolha++;
                floodfill(imagem, CvPoint(y, x), qtdBolha*5);
            }
        }
    }

    imshow("Original", original);
    imshow("Bolhas", imagem);
    imwrite("resultBolhas.png", imagem);
    cout<<"Foram encontradas "<<qtdBolha<<" bolhas, ";

    floodfill(imagem, CvPoint(0,0), 255); // Pinta o fundo da imagem de branco, para podermos contar a qtd. de buracos dentro de bohlas

    for(int y=0; y<imagem.cols; y++){ // A matriz está sendo percorrida da maneira convencional, de coluna à coluna a partir da linha 0
        for(int x=0; x<imagem.rows; x++){
            if(imagem.at<uchar>(x, y)==0){ // Aqui é usado (x,y), não (y,x), porque ...
                qtdBuraco++;
                floodfill(imagem, CvPoint(y, x), qtdBuraco*10);
            }
        }
    }

    imshow("Buracos", imagem);
    imwrite("buracos.png", imagem);
    cout<<qtdBuraco<<" com buraco."<<endl;
    waitKey();

    return 0;
}