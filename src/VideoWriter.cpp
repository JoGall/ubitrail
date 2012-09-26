#include <sstream>
#include "VideoWriter.hpp"

VideoWriter::VideoWriter()
{
    //ctor
}
VideoWriter::VideoWriter(std::vector<Tracker>* trackers, Options& options,cv::Size globalFrameSize, int fps):
m_options(options)
{
    //ctor
    m_trackers = new std::vector<Tracker>;
    m_trackers = trackers;
    globalFrame = m_options.allFrameOutput;
    singleAreas = m_options.videosOutput;


//    cvWriters.resize(nWriter);

    if(globalFrame){
        std::stringstream tss;
        tss<<m_options.outDir<<"/"<<"globalFrame.avi";
        cvWriters.push_back(cv::VideoWriter(tss.str(), VIDEO_WRITER_CODEC, fps,globalFrameSize));
        cv::Rect rect(cv::Point(0,0),globalFrameSize);
        ROIs.push_back(rect);
    }

    if(singleAreas){
       for(unsigned int i = 0; i < trackers->size();i++){
        std::stringstream tss;
        tss<<m_options.outDir<<"/"<<(*trackers)[i].getLabel()<<".avi";
        cv::Size size((*trackers)[i].getROI().width,(*trackers)[i].getROI().height);

        cvWriters.push_back(cv::VideoWriter(tss.str(), VIDEO_WRITER_CODEC, fps,size));
         ROIs.push_back((*trackers)[i].getROI());
        }
    }

    for(unsigned int i = 0; i < cvWriters.size();i++){
        assert(cvWriters[i].isOpened());

    }
}

VideoWriter::~VideoWriter()
{
    //dtor
}


void VideoWriter::newFrame(cv::Mat& frame){
//    std::cout<<"srdsdwqadf "<<cvWriters.size() <<std::endl;
    #pragma omp parallel for schedule (dynamic) num_threads(m_trackers->size())
    for(unsigned int i = 0; i < cvWriters.size(); i++){
//        if(globalFrame){
            cvWriters[i].write(frame(ROIs[i]));
//        }
    }

}
