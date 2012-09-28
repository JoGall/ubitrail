#include "MotionSensor.hpp"

MotionSensor::MotionSensor(){
}

MotionSensor::MotionSensor(int sensitivity,cv::Mat& mask):
//mog(MOTION_SENSOR_HISTORY, MOTION_SENSOR_THR, false),
//trainingRate(MOTION_SENSOR_TRAINING_MIN),
m_sensitivity(sensitivity),
m_mask(mask)
{
    //ctor
}

MotionSensor::~MotionSensor()
{
    //dtor
}
int MotionSensor::run(cv::Mat& img, cv::Mat& out){
//    cv::Mat out;

    if(accum.empty())
        img.convertTo(accum,CV_32F);
    else
        cv::accumulateWeighted(img,accum,MOTION_SENSOR_ACCU_WEIGHT,m_mask);

    accum.convertTo(out,CV_8U);
    cv::absdiff(out,img,out);
    cv::cvtColor(out,out,CV_BGR2GRAY);
    cv::medianBlur(out,out,3);



    cv::threshold(out,out,m_sensitivity,255,cv::THRESH_BINARY);

    //TODO remove ?
    cv::dilate(out,out,cv::Mat(),cv::Point(-1,-1),1);
    cv::erode(out,out,cv::Mat(),cv::Point(-1,-1),2);

    return 1000 * (double)cv::countNonZero(out)/(double)(out.rows*out.cols);
}
