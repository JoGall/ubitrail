#include "Tracker.hpp"


//todel


Tracker::Tracker(){

}

Tracker::Tracker(Area area,const std::string  label, Options& options):
m_area(area),
m_label(label),
motionSensor(options.motionSensitivity,m_area.getTerrit()),
featureClassifier(options.MOGTrainingRounds),
foregroundExtractor(options.MOGTrainingRounds),
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
        foregroundExtractor.run(preprocedImg,motionFrame,motionQ,wasAmbiguous,contours);
        if(contours.size() > 1 ){
            wasAmbiguous = true;
        }
        else{
            wasAmbiguous = false;
        }


        if(foregroundExtractor.isTrained() && contours.size() > 0){
            featureClassifier.run(contours,preprocedImg);
            if(featureClassifier.isTrained() && contours.size() > 0){
                isTrained = true;
                position = featureClassifier.getPosition();
                isPositionValid = true;
            }
        }
}

