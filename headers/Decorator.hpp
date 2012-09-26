#ifndef DECORATOR_H
#define DECORATOR_H

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include "Tracker.hpp"
class Decorator
{
    public:
        Decorator();
        Decorator( std::vector<Tracker>* trackers, cv::Size imgSize );
        virtual ~Decorator();
        void getDecoratedFrame(cv::Mat& frame);
        void newFrame(cv::Mat& frame);
        void plot(int i);

    protected:
        void makeStaticOverlay();
    private:
        std::vector<Tracker> *m_trackers;
        std::vector<cv::Point2f > pos;
        std::vector<bool> posIsValid;
        cv::Mat staticOverlay,staticGrayTip,frameCopy;
};

#endif // DECORATOR_H
