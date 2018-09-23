#include <iostream>
#include <opencv2/opencv.hpp>
#include <cmath>
using namespace cv;
using namespace std;

float l1, l2, d=2.0, tmp;

int main(int argvc, char** argv){
  Mat pOrig(Size(10, 10), CV_32F);
  l1 = pOrig.rows/5.0;
  l2 = pOrig.rows - l1;

  for(int x=0; x<pOrig.rows; x++){
    tmp = 1/2.0*(tanh((x-l1)/d) - tanh((x-l2)/d));
    for(int y=0; y<pOrig.cols; y++){
      pOrig.at<float>(x, y) = tmp*255;
      setprecision(2);
      //cout<<fixed<<tmp<<", ";
      cout<<fixed<<pOrig.at<float>(x, y)<<" ";
    }
    cout<<endl;
  }
  namedWindow("opa", CV_WINDOW_FREERATIO);
  imshow("opa", pOrig);
  waitKey();

  return 0;
}
