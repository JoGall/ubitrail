/*
    Copyright Quentin Geissmann 2012

    This file is part of Ubitrail

    Ubitrail is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Ubitrail is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Ubitrail.  If not, see <http://www.gnu.org/licenses/>.
*/


#include "MotionSensor.hpp"

MotionSensor::MotionSensor(){
}

MotionSensor::MotionSensor(int sensitivity,cv::Mat& mask):
m_mask(mask),
m_sensitivity(sensitivity)
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
