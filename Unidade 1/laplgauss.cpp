#include <iostream>
#include <opencv2/opencv.hpp>
using namespace cv;
using namespace std;

void printmask(Mat &m){
    for (int i = 0; i < m.size().height; i++)
    {
        for (int j = 0; j < m.size().width; j++)
        {
            cout << m.at<float>(i, j) << ",";
        }
        cout << endl;
    }
}

void menu(){
    cout << "\npressione a tecla para ativar o filtro: \n"
            "o - original\n"
            "a - calcular modulo\n"
            "m - media\n"
            "g - gauss\n"
            "v - vertical\n"
            "h - horizontal\n"
            "l - laplaciano\n"
            "x - Laplaciano do Gaussiano\n"
            "esc - sair\n";
}

int main(int argvc, char **argv){
    VideoCapture video;
    float original[] = {0, 0, 0,
                        0, 1, 0,
                        0, 0, 0};
    float media[] = {1/9.0, 1/9.0, 1/9.0,
                     1/9.0, 1/9.0, 1/9.0,
                     1/9.0, 1/9.0, 1/9.0};
    float gauss[] = {1/16.0, 2/16.0, 1/16.0,
                     2/16.0, 4/16.0, 2/16.0,
                     1/16.0, 2/16.0, 1/16.0};
    float horizontal[] = {-1, 0, 1,
                          -2, 0, 2,
                          -1, 0, 1};
    float vertical[] = {-1, -2, -1,
                        0, 0, 0,
                        1, 2, 1};
    float laplacian[] = {0, -1, 0,
                         -1, 4, -1,
                         0, -1, 0};

    Mat cap, frame, frame32f, frameFiltered;
    Mat mask(3, 3, CV_32F), mask1(3, 3, CV_32F), mask2(3, 3, CV_32F);
    Mat result, result1;
    int absolut;
    char key=-1, temp=-1;

    video.open(0);
    if (!video.isOpened())
        return -1;
    mask = Mat(3, 3, CV_32F, original);
    absolut = 1; // calcs abs of the image

    menu();
    for (;;){
        video >> cap;
        cvtColor(cap, frame, CV_BGR2GRAY);
        flip(frame, frame, 1);
        frame.convertTo(frame32f, CV_32F);
        
        temp = (char)waitKey(10);
        
        if (temp != -1){
            key = temp;
            switch (key){
            case 'a':
                absolut = !absolut;
                break;
            case 'o':
                mask = Mat(3, 3, CV_32F, original);
                printmask(mask);
                break;
            case 'm':
                mask = Mat(3, 3, CV_32F, media);
                printmask(mask);
                break;
            case 'g':
                mask = Mat(3, 3, CV_32F, gauss);
                printmask(mask);
                break;
            case 'h':
                mask = Mat(3, 3, CV_32F, horizontal);
                printmask(mask);
                break;
            case 'v':
                mask = Mat(3, 3, CV_32F, vertical);
                printmask(mask);
                break;
            case 'l':
                mask = Mat(3, 3, CV_32F, laplacian);
                printmask(mask);
                break;
            case 'x':
                mask = Mat(3, 3, CV_32F, gauss);
                printmask(mask);
                mask = Mat(3, 3, CV_32F, laplacian);
                printmask(mask);
                break;
            default:
                break;
            }
        }
        if (key == 27) 
            break; // esc pressed!
        else if((char)key == 'x'){
            mask1 = Mat(3, 3, CV_32F, gauss);
            filter2D(frame32f, frame32f,
                     frame32f.depth(), mask1, Point(1, 1), 0);
        }

        filter2D(frame32f, frameFiltered,
                 frame32f.depth(), mask, Point(1, 1), 0);
        if (absolut) frameFiltered = abs(frameFiltered);
        frameFiltered.convertTo(result, CV_8U);
        imshow("filtroespacial", result);
    }
    return 0;
}