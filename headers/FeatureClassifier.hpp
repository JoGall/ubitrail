#ifndef FEATURECLASSIFIER_H
#define FEATURECLASSIFIER_H
#include "opencv2/imgproc/imgproc.hpp"
#include "defines.hpp"
class FeatureClassifier
{
    public:
        FeatureClassifier();
        FeatureClassifier(int roundToTrain);
        virtual ~FeatureClassifier();
        void run(std::vector<std::vector<cv::Point> >& contours,cv::Mat& img);
        bool isTrained(){
            if(roundTrained < m_roundToTrain)
                return false;
            else
                return true;
        }

        float trainedFract(){
            return (float) roundTrained/ (float)m_roundToTrain;
        }

        float getL(){
            return L;
        }
        cv::Point2f getPosition(){
            return realPosition;
        }

    protected:
        void makeMiniImg(std::vector<cv::Point>& contour,  cv::Mat& miniIMG, cv::Mat& img);
        void makeFeatures(std::vector<double>&,std::vector<cv::Point>& contour,  cv::Mat& miniIMG);
        cv::Point2f updatePosition (cv::Point2f center);
        float calcLikelyhood(std::vector<double>& vect);
        void updateMeanSD(std::vector<double>& vect);
    private:
        cv::Mat featuresMeansSds;
        cv::Mat positionsAccum;
        unsigned long roundTrained;
        int m_roundToTrain;
        cv::Point2f position,realPosition;
        float L;
};

#endif // FEATURECLASSIFIER_H
