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
isTrained(false),
m_agentSize(options.agentSize)
{
}

Tracker::~Tracker()
{
}


void Tracker::nextFrame(const cv::Mat& wholeFrame, double time){
        isPositionValid = false;
        cv::Mat img, preprocedImg, motionFrame;
        img = wholeFrame(m_area.getROI());

        this->preprocess(img,preprocedImg);

        int motionQ = motionSensor.run(preprocedImg,motionFrame);
        std::vector<std::vector<cv::Point> > contours(0);
        wasAmbiguous = !foregroundExtractor.run(preprocedImg,motionFrame,motionQ,wasAmbiguous,contours);

        if(foregroundExtractor.isTrained() && contours.size() > 0){
            featureClassifier.run(contours,preprocedImg);
            if(featureClassifier.isTrained() && contours.size() > 0){
                isTrained = true;
                position = featureClassifier.getPosition();
                isPositionValid = true;
//                if(m_label == "06")
 //                   featureClassifier.printFeatures();
                }
            }
        }


void Tracker::preprocess(const cv::Mat& in,cv::Mat& out){
    if(m_agentSize > 5){
        cv::medianBlur(in,out,3);
    }
    else{
        in.copyTo(out);
    }

    double sigma = (double) m_agentSize / 5;
    cv::GaussianBlur(out,out,cv::Size(0,0),sigma);

    if(accum.empty())
        out.convertTo(accum,CV_32F);
    else
        //cv::accumulateWeighted(out,accum,TRACKER_ACCU_WEIGHT,m_area.getTerrit());
        cv::accumulateWeighted(out,accum,1,m_area.getTerrit());

    accum.convertTo(out,CV_8U);
}
