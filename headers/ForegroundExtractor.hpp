#ifndef FOREGROUNDEXTRACTOR_H
#define FOREGROUNDEXTRACTOR_H


#include "opencv2/video/background_segm.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "defines.hpp"


class ForegroundExtractor
{
    public:
        ForegroundExtractor();
        ForegroundExtractor(int roundToTrain);
        virtual ~ForegroundExtractor();
        int run(cv::Mat& img,cv::Mat& mot, int motionQ,bool wasAmbig,std::vector<std::vector<cv::Point> >& validContours);

        bool isTrained(){
            if(roundTrained < m_roundToTrain)
                return false;
            else
                return true;
        }
        float trainedFract(){
            return (float) roundTrained/ (float)m_roundToTrain;
        }
    protected:
        void updateTrainingRate();
        void mergeContours(cv::Mat& img, std::vector<std::vector<cv::Point> >& contours);
        void removeLargeContours(std::vector<std::vector<cv::Point> >& contours,double max);
    private:
        cv::BackgroundSubtractorMOG2 mog;
        unsigned long roundTrained;
        unsigned int m_roundToTrain;
        double trainingRate;
};

#endif // FOREGROUNDEXTRACTOR_H
