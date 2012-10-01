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


#include "Tracker.hpp"

Tracker::Tracker(){

}

Tracker::Tracker(Area area,const std::string  label, Options& options):
m_area(area),
m_label(label),
motionSensor(options.motionSensitivity,m_area.getTerrit()),
foregroundExtractor(options.MOGTrainingRounds),
featureClassifier(options.MOGTrainingRounds),
wasAmbiguous(false),
isTrained(false)
{
    //ctor

}

Tracker::~Tracker()
{
    //dtor
}


void Tracker::nextFrame(const cv::Mat& wholeFrame, double time){
        isPositionValid = false;
        cv::Mat img,preprocedImg,motionFrame;
        img = wholeFrame(m_area.getROI());
        cv::medianBlur(img,preprocedImg,3);
        cv::GaussianBlur(img,preprocedImg,cv::Size(7,7),5);
        if(accum.empty())
            preprocedImg.convertTo(accum,CV_32F);
        else
            cv::accumulateWeighted(preprocedImg,accum,TRACKER_ACCU_WEIGHT,m_area.getTerrit());
        accum.convertTo(preprocedImg,CV_8U);
        int motionQ = motionSensor.run(preprocedImg,motionFrame);
        std::vector<std::vector<cv::Point> > contours(0);
        wasAmbiguous = !foregroundExtractor.run(preprocedImg,motionFrame,motionQ,wasAmbiguous,contours);


        if(foregroundExtractor.isTrained() && contours.size() > 0){
            featureClassifier.run(contours,preprocedImg);
            if(featureClassifier.isTrained() && contours.size() > 0){
                isTrained = true;
                position = featureClassifier.getPosition();
                isPositionValid = true;
            }
        }
}

