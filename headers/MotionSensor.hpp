#ifndef MOTIONSENSOR_H
#define MOTIONSENSOR_H

#include "opencv2/video/background_segm.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "defines.hpp"

/**
 * The class responsible for detecting and quantifying short term motion.
 */
class MotionSensor
{
    public:
        /**
         * The default constructor.
         */
        MotionSensor();

        /**
         * Another contructor. A MotionSensor object cannot be used unless it was constructed in this manner.
         * @param a -- ultimately user defined -- value of sensitivity.
         * @param a predefined mask.
         */

        MotionSensor(int sensitivity,cv::Mat& mask);
        virtual ~MotionSensor();

        /**
         * A public member function updating the dynamic background and retruning the difference to the current frame.
         * @param img an input frame.
         * @param out a binary matrix where pixels values are non-zero where motion was detected.
         * @return 1000 times the proportion of non-zero pixels in out.
         *
         * It performs as follow:
         * 1. Computes a runing weighted average of the current frame.
         * 2. Calculates the absolute difference between the current frame and the weighted average.
         * 3. Threshold the result by the value of sensitivity.
         */
        int run(cv::Mat& img, cv::Mat& out);
    protected:
    private:
//    cv::BackgroundSubtractorMOG2 mog;
//    double trainingRate;
    cv::Mat accum;
    cv::Mat m_mask;
    int m_sensitivity;
};

#endif // MOTIONSENSOR_H
