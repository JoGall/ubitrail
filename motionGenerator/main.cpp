// to compile :
// g++ main.cpp  $(pkg-config --libs --cflags opencv) -O2 -o movMake


// BACKUP DEFINES : do NOT change
//#define MEAN_NOISE 128
//#define SIGMA_NOISE 128
//#define ATTENUATE_NOISE_SIGMA 1
//#define AGENT_COLOUR cv::Scalar(40,125,200)
//#define AGENT_RAD 3
//#define TEMPORAL_BLUR 0.99


#include <sstream>
#include <iostream>
#include <fstream>

#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"


#define FPS 50

//CHANGE THESE ONES

#define MEAN_NOISE 128
#define SIGMA_NOISE 128
#define ATTENUATE_NOISE_SIGMA 1
#define AGENT_COLOUR cv::Scalar(69,120,180)
#define AGENT_RAD 3
#define TEMPORAL_BLUR 0.99


void makeBG(cv::Mat& bg);


/** args:
 * 1) Input result File TAB SEPARATED
 * 2) Output video
 * 3) Dimension of the video
 */
int main(int argc, char **argv){

    if(argc != 4){
        std::cerr<<"Exactly three arguments are needed"<<std::endl;
        return 0;
    }
    else{
        std::stringstream tss1,tss2;
        tss1<<argv[1];
        tss2<<argv[2];
        int size = atoi(argv[3]);
        std::ifstream source;
        source.open(tss1.str().c_str(),std::ios_base::in);
        cv::Mat bg(size,size,CV_8UC3,cv::Scalar(123,123,123));
        cv::Mat display;
        cv::VideoWriter myVideo(tss2.str(), CV_FOURCC('D','I','V','X'), FPS,bg.size() );
        std::string line;

        //main loop
        while(getline(source, line)){
            std::istringstream in(line);
            float x,y;
            in >> x;
            in >> y;
            makeBG(bg);
            bg.copyTo(display);
            int absX = ((x+1) / 2) * bg.cols;
            int absY = ((y+1) / 2) * bg.cols;
            cv::circle(display,cv::Point(absX,absY),AGENT_RAD,AGENT_COLOUR,-1,CV_AA);
            myVideo<<display;

        }
        return 1;
    }
}


void makeBG(cv::Mat& bg){
    std::vector<cv::Mat> bg_vect(3);
    cv::Mat bg2;
    cv::resize(bg, bg2, cv::Size(0,0), 0.2, 0.2, cv::INTER_NEAREST );
    cv::split(bg2,bg_vect);
    for(size_t i = 0; i < 3;i++ ){
        cv::Mat noise(bg2.size(), CV_8U);
        cv::randn(noise,cv::Scalar::all(MEAN_NOISE),cv::Scalar::all(SIGMA_NOISE));
        cv::GaussianBlur(noise,bg_vect[i],cv::Size(0,0),ATTENUATE_NOISE_SIGMA);
    }
    cv::merge(bg_vect,bg2);
    cv::resize(bg2,bg2, bg.size(),0,0, cv::INTER_NEAREST );
    cv::addWeighted(bg, TEMPORAL_BLUR, bg2, 1-TEMPORAL_BLUR,0,bg);
}

