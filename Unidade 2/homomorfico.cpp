#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;
using namespace std;

double    gammaH, gammaL, c, D0;
int       gammaH_slider=20, gammaL_slider=2, c_slider=1, D0_slider=5;
const int gammaH_max=50,   gammaL_max=10,   c_max=600,  D0_max=200;

Mat padded, filtro, complexImage;

/**
 * cria_filtro constrói o filtro homomórfico do tamanho dado
 * param padded: imagem com o tamanho desejado pro filtro
 * param gl, gh, c, d0: variáveis do filtro homomórfico
*/
Mat cria_filtro(double gl, double gh, double c, double d0){
  Mat filtro = Mat(padded.size(), CV_32FC2, Scalar(0)), tmp = Mat(padded.size(), CV_32F);
  int dft_M = padded.rows, dft_N = padded.cols;

  for(int i=0; i<dft_M; i++){
    for(int j=0; j<dft_N; j++){
      //double raio = (i-dft_M/2)*(i-dft_M/2) + (j-dft_N/2)*(j-dft_N/2) // Foi inserido na linha abaixo
      tmp.at<float> (i,j) = (gh - gl)*(1 - exp(-c*(( (i-dft_M/2)*(i-dft_M/2) + (j-dft_N/2)*(j-dft_N/2) ) / (d0*d0) ))) + gl;
    }
  }

  Mat comps[]= {tmp,tmp};
  merge(comps, 2, filtro);
  normalize(tmp,tmp,0,1,CV_MINMAX);
  imshow("filtro", tmp);
  return filtro;
}

/**
 * É ativado em qualquer ação de qualquer slider
*/
void on_trackbar(int, void*){
  gammaH = (double)gammaH_slider/10.0; // gammaH é um real e varia de 0 a 5
  gammaL = (double)gammaL_slider; // gammaL é um real e varia de 0 a 1
  c = (double)c_slider/10; // c é um real e varia de 0 a 60
  D0 = (double)D0_slider;
  if(D0<1.0){
    D0=1.0;
  }
  if(c<1.0){
    c=1.0;
  }
}

// troca os quadrantes da imagem da DFT
void deslocaDFT(Mat& image ){
  Mat tmp, A, B, C, D;

  // se a imagem tiver tamanho impar, recorta a regiao para
  // evitar cópias de tamanho desigual
  image = image(Rect(0, 0, image.cols & -2, image.rows & -2));
  int cx = image.cols/2; 
  int cy = image.rows/2;

  // reorganiza os quadrantes da transformada
  // A B   ->  D C
  // C D       B A
  A = image(Rect(0, 0, cx, cy));
  B = image(Rect(cx, 0, cx, cy));
  C = image(Rect(0, cy, cx, cy));
  D = image(Rect(cx, cy, cx, cy));

  A.copyTo(tmp);  D.copyTo(A);  tmp.copyTo(D);// A <-> D
  C.copyTo(tmp);  B.copyTo(C);  tmp.copyTo(B); // C <-> B
}

int main(int , char**){
  VideoCapture cap;
  Mat multsp; //imagem de dois canais para abrigar números complexos
  Mat  mag; //preenchida com zeros, filtro homomórfico, magnitude
  Mat image, imagegray, tmp; //imagem capturada, capturada em cinza, temporária
  Mat_<float> realInput, zeros;  //imagem com parte real da transformada, imagem preta
  vector<Mat> planos; // imagem com componentes da transformada (parte real e imaginária)
  char TrackbarName[50];
  char key; // guarda tecla capturada

  int dft_M, dft_N; // valores ideais dos tamanhos da imagem para calculo da DFT
  cap.open(0); // abre a câmera default
  if(!cap.isOpened()) return -1;

  cap >> image; // captura uma imagem para recuperar as informacoes de gravação
  
  // identifica os tamanhos otimos para calculo do FFT
  dft_M = getOptimalDFTSize(image.rows);
  dft_N = getOptimalDFTSize(image.cols);

  // realiza o padding da imagem
  copyMakeBorder(image, padded, 0,
                 dft_M - image.rows, 0,
                 dft_N - image.cols,
                 BORDER_CONSTANT, Scalar::all(0));
  zeros = Mat_<float>::zeros(padded.size()); // parte imaginaria da matriz complexa (preenchida com zeros)
  complexImage = Mat(padded.size(), CV_32FC2, Scalar(0)); // prepara a matriz complexa para ser preenchida
  filtro = complexImage.clone(); // a função de transferência (filtro frequencial) deve ter o mesmo tamanho e tipo da matriz complexa
  //tmp = Mat(dft_M, dft_N, CV_32F); // cria uma matriz temporária para criar as componentes real e imaginaria do filtro ideal
  Mat comps[]= {Mat(dft_M, dft_N, CV_32F), Mat(dft_M, dft_N, CV_32F)}; // cria a matriz com as componentes do filtro e junta ambas em uma matriz multicanal complexa
  merge(comps, 2, filtro);

  for(;;){
    cap >> image;
    cvtColor(image, imagegray, CV_BGR2GRAY);
    imshow("original", imagegray);

    // realiza o padding da imagem
    copyMakeBorder(imagegray, padded, 0,
                   dft_M - image.rows, 0,
                   dft_N - image.cols,
                   BORDER_CONSTANT, Scalar::all(0));

    planos.clear(); // limpa o array de matrizes que vao compor a imagem complexa
    
    realInput = Mat_<float>(padded); // cria a componente real
    realInput += Scalar::all(1);
    log(realInput, realInput);
    planos.push_back(realInput); // insere as duas componentes no array de matrizes
    planos.push_back(zeros); //     ^
    merge(planos, complexImage); // combina o array de matrizes em uma unica componente complexa
    
    dft(complexImage, complexImage); // calcula o dft
    deslocaDFT(complexImage); // realiza a troca de quadrantes
    resize(complexImage, complexImage, padded.size());
    normalize(complexImage, complexImage, 0, 1, CV_MINMAX);

    filtro = cria_filtro(gammaH, gammaL, c, D0);

    mulSpectrums(complexImage,filtro,complexImage,0); // aplica o filtro frequencial
    deslocaDFT(complexImage); // troca novamente os quadrantes
    idft(complexImage, complexImage); // calcula a DFT inversa

    planos.clear(); // limpa o array de planos
    split(complexImage, planos); // separa as partes real e imaginaria da imagem filtrada
    exp(planos[0], planos[0]);
    planos[0] -= Scalar::all(1);
    normalize(planos[0], planos[0], 0, 1, CV_MINMAX); // normaliza a parte real para exibicao
    namedWindow("filtrada", CV_WINDOW_FREERATIO);
    imshow("filtrada", planos[0]);

    sprintf(TrackbarName, "GammaH - %d", gammaH_max);
    createTrackbar(TrackbarName, "filtrada", &gammaH_slider, gammaH_max, on_trackbar);

    sprintf(TrackbarName, "GammaL - %d", gammaL_max);
    createTrackbar(TrackbarName, "filtrada", &gammaL_slider, gammaL_max, on_trackbar);

    sprintf(TrackbarName, "c - %d", c_max);
    createTrackbar(TrackbarName, "filtrada", &c_slider, c_max, on_trackbar);

    sprintf(TrackbarName, "D0 - %d", D0_max);
    createTrackbar(TrackbarName, "filtrada", &D0_slider,D0_max, on_trackbar);

    key = (char) waitKey(10);
    if( key == 27 ) break; // esc pressed!
  }
  return 0;
}
