#ifndef TRACKER_H
#define TRACKER_H

#include "opencv2/imgproc/imgproc.hpp"

#include "defines.hpp"
#include "Area.hpp"
#include "optionStructure.hpp"
#include "MotionSensor.hpp"
#include "ForegroundExtractor.hpp"
#include "FeatureClassifier.hpp"



class Tracker
{
    public:
        Tracker();
        Tracker(Area,const std::string  label, Options& options );
        virtual ~Tracker();
        void nextFrame(cv::Mat wholeFrame, double time);
        bool getIsValid(){
            return isPositionValid;
        }
        float getX(){
            return (float)position.x/(float)m_area.getROI().width;
        }
        float getY(){
            return (float)position.y/(float)m_area.getROI().height;
        }
        float getLikelyhood(){
            return featureClassifier.getL();
        }

        float getTrainedFrac(){
            return (featureClassifier.trainedFract()+foregroundExtractor.trainedFract()) /2;
        }
        std::string getLabel(){
            return m_label;
        }

        int getTerrit(){
            return m_area.inWhichTerritIsPoint(cv::Point2f(position.x,position.y));
        }
        bool getIsTrained(){
            return isTrained;
        }
        cv::Mat& getTerritMap(){
            return m_area.getTerrit();
        }
        cv::Rect& getROI(){
            return m_area.getROI();
        }

    protected:
    private:

        Area m_area;
        std::string m_label;
        MotionSensor motionSensor;
        ForegroundExtractor foregroundExtractor;
        FeatureClassifier featureClassifier;
        cv::Mat accum;
        bool wasAmbiguous;
        bool isPositionValid;
        cv::Point2f position;
        bool isTrained;

};

#endif // TRACKER_H
