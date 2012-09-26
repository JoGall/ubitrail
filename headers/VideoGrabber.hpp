#ifndef VIDEOGRABBER_H
#define VIDEOGRABBER_H
#include "opencv2/highgui/highgui.hpp"
#include "optionStructure.hpp"
#include <sys/time.h>

class VideoGrabber
{
    public:
        VideoGrabber();
        VideoGrabber(Options* opts);
        ~VideoGrabber();
        bool reinit(Options* opts);
        bool getFrame(cv::Mat& img,double* time);
        void reset();
        int getFPS(){
            return FPS;
        }

        double getProgress(){
            if(isVideoFile)
                return cap.get(CV_CAP_PROP_POS_FRAMES)/cap.get(CV_CAP_PROP_FRAME_COUNT);
            else
                return -1;
        }


    protected:
        long getTimeDev();
    private:
        unsigned int FPS;
        cv::VideoCapture cap;
        bool isVideoFile;
        timeval start, end;
};


#endif // VIDEOGRABBER_H
