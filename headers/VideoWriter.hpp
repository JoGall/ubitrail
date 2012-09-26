#ifndef VIDEOWRITER_H
#define VIDEOWRITER_H

#include "defines.hpp"
#include "Tracker.hpp"
#include "optionStructure.hpp"


class VideoWriter
{
    public:
        VideoWriter();
        VideoWriter(std::vector<Tracker>* trackers, Options& options,cv::Size globalFrameSize, int fps);
        virtual ~VideoWriter();
        void newFrame(cv::Mat& frame);
    protected:
    private:
        std::vector<Tracker>* m_trackers;
        std::vector<cv::VideoWriter> cvWriters;
        std::vector<cv::Rect> ROIs;
        Options m_options;
        bool globalFrame;
        bool singleAreas;
};

#endif // VIDEOWRITER_H
