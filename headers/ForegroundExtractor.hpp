#ifndef FOREGROUNDEXTRACTOR_H
#define FOREGROUNDEXTRACTOR_H


#include "opencv2/video/background_segm.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "defines.hpp"

/**
 * The class responsible for extracting foreground (i.e. the agent).
 * It relies on opencv's "cv::BackgroundSubtractorMOG2" (Gaussian Mixture-based Background/Foreground Segmentation Algorithm).
 * However, the learning rate of the algorhithm is made dynamic. If only one object is detected, the learning rate remains low.
 * Whilst, if anomalies are detected (for instance multiple objects are found), the leaning rate increases.
 */
class ForegroundExtractor
{
    public:
        /**
         * The default constructor.
         */
        ForegroundExtractor();

        /**
         * Another contructor. A ForegroundExtractor object cannot be used unless it was constructed in this manner.
         * @param roundToTrain the number of succesfull foreground extractions to be done before the model should be used.
         */
        ForegroundExtractor(int roundToTrain);
        virtual ~ForegroundExtractor();

        /**
         * A public member function updating the current mog and returning a list of "valid" contours
         * @param img.
         * @param mot
         * @param motionQ
         * @param wasAmbig
         * @param validContours
         *
         */
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
