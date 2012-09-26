#ifndef MOTIONSENSOR_H
#define MOTIONSENSOR_H

#include "opencv2/video/background_segm.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "defines.hpp"


class MotionSensor
{
    public:
        MotionSensor();
        MotionSensor(int sensitivity,cv::Mat& mask);
        virtual ~MotionSensor();
        double run(cv::Mat& img, cv::Mat& out);
//        double run(cv::Mat& img, cv::Mat& out,bool wasAmbig);
    protected:
    private:
    cv::BackgroundSubtractorMOG2 mog;
    double trainingRate;
    cv::Mat accum;
    cv::Mat m_mask;
    int m_sensitivity;
};

#endif // MOTIONSENSOR_H
