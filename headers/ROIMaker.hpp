#ifndef ROIMAKER_H
#define ROIMAKER_H

#include "opencv2/imgproc/imgproc.hpp"
#include "optionStructure.hpp"

class ROIMaker
{
    public:
        ROIMaker(Options& options, cv::Mat& bg);
        virtual ~ROIMaker();
        std::vector<cv::Mat>& getMasks(){
            return m_masks;}
        std::vector<cv::Rect>& getROIs(){
            return m_ROIs;}
    protected:
        void isodiametricCirclesAsROIs();
        double scoreSetOfCircles(std::vector<cv::Vec3f> circles);
        bool filterCircles(std::vector<cv::Vec3f> & circles, int w, int h);
        void wholeFrameAsROI();

    private:
        Options m_options;
        cv::Mat m_bg;
        std::vector<cv::Mat> m_masks;
        std::vector<cv::Rect> m_ROIs;

};

#endif // ROIMAKER_H
