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
         * A public member function updating the current MoG and returning a list of "valid" contours.
         * @param img an input frame.
         * @param mot an input "motion-frame". It has non-zero pixels where motion was detected.
         * @param motionQ the amount of motion in the last motion frame (between 0 and 1000)
         * @param wasAmbig a boolean being true if ambiguity was present in the last run.
         * @param validContours an output vector of contours.
         * @param true if no ambiguity was detected.
         *
         * It performs as follow:
         * 1. Assesses if the last frame was ambiguous. and increase the learning rate of the MoG if yes.
         * 2. Applies the MoG to the current frame.
         * 3. Defined seeds as regions that are non-zero in both mot and the result of MoG.
         * 4. Keeps as valid contours only the regions of MoG containing, at least, one seed.
         * 5. Agregate contours that are close to eachother (relative to there size).
         * 6. Removes the contours that are two large.
         */
        bool run(const cv::Mat& img,const cv::Mat& mot, int motionQ,bool wasAmbig,std::vector<std::vector<cv::Point> >& validContours);

        /**
         * A public member function assessing whether or not ForegroundExtractor is trained.
         * @return true if the model if the ForegroundExtractor is trained.
         */
        bool isTrained(){
            return !(roundTrained < m_roundToTrain);
        }

        /**
         * A public member function getting the training progress for ForegroundExtractor.
         * @return the proportion ot training.
         */
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
