#include "VideoGrabber.hpp"
#include <exception>
VideoGrabber::VideoGrabber(): isVideoFile(true)
{
}
VideoGrabber::VideoGrabber(Options* opts): isVideoFile(true){
    this->reinit(opts);
//    }
}
bool VideoGrabber::reinit(Options* opts){
    if(cap.isOpened())
        cap.release();

    if(opts->videoFile != ""){
        cap = cv::VideoCapture(opts->videoFile);
        if(!cap.isOpened())
            return false;

        isVideoFile = true;
    }
    else if(opts->webCamIdx >0){
        cap = cv::VideoCapture(opts->webCamIdx);
        isVideoFile = false;
        gettimeofday(&start, NULL);
    }

        FPS = 0;
        FPS =  cap.get(CV_CAP_PROP_FPS );
        //todo
        if(FPS == 0)
            FPS = 30;
    return true;
}

VideoGrabber::~VideoGrabber(){
    cap.release();
}

bool VideoGrabber::getFrame(cv::Mat& img,double* time){
    bool success = true;
    if(cap.isOpened()){
        if(isVideoFile){
            success = cap.read(img);
            *time =  cap.get(CV_CAP_PROP_POS_MSEC );
        }

        else{
            cap.grab();
            *time = (double) this->getTimeDev();
            success = cap.retrieve(img);
        }
        return success;
    }
    else
        return false;
}

void VideoGrabber::reset(){

    if(isVideoFile){

        cap.set(CV_CAP_PROP_POS_FRAMES,0);
    }
    else{
        gettimeofday(&start, NULL);
    }
}

long VideoGrabber::getTimeDev(){
    gettimeofday(&end, NULL);
    return ((long) (end.tv_sec  - start.tv_sec) * 1000 + (long)(end.tv_usec - start.tv_usec)/1000.0) + 0.5;
}
