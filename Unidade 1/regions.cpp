/*
Descrição do exercício 1: https://agostinhobritojr.github.io/tutorial/pdi/#_exerc%C3%ADcios
*/

#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

/** aquisicao() solicita as coordenadas de dois pontos ao usuário
 * e retorna um vetor com dois pontos, o superior esquerdo na pos. 0
 * e o inferior direito na posição 1.
 */
vector<Point> aquisicao(){
    Point p1, p2; // Pontos dados pelo usuário, não sabemos qual é o mais a esquerda
    vector<Point> p;
    cout<<"Digite as coordenadas X e Y do primeiro ponto: ";
    cin>>p1.x>>p1.y;
    cout<<"Digite as coordenadas X e Y do segundo ponto: ";
    cin>>p2.x>>p2.y;

    if(p1.x < p2.x){ // p1 é o ponto superior esquerdo
        p.push_back(p1);
        p.push_back(p2);
    } else{ // p1 é o inferior direito
        p.push_back(p2);
        p.push_back(p1);
    }
    return p;
}

int main(){
    vector<String> endImg;
    Point ini, fin; // Ponto inicial e final, respectivamente
    vector<Point> pontos;
    //Carregando uma imagem localizada numa outra pasta qualquer
    glob("/home/victor/Área de Trabalho/OpenCV_Codes/img/biel.png", endImg, false);
    Mat imagem = imread(endImg[0], CV_LOAD_IMAGE_GRAYSCALE);

    pontos = aquisicao();
    ini = pontos[0];
    fin = pontos[1];

    cout<<"Ponto superior esquerdo:\n";
    cout<<ini<<endl;
    cout<<"Ponto inferior direito:\n";
    cout<<fin<<endl;

    for(int l=ini.x; l<fin.x; l++){
        for(int c=ini.y; c<fin.y; c++){
            imagem.at<uchar>(c,l) = 255 - imagem.at<uchar>(c,l);
        }
    }
    imshow("Resultado", imagem);
    waitKey();

    return 0;
}
