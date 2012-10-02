/*
    Copyright Quentin Geissmann 2012

    This file is part of Ubitrail

    Ubitrail is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Ubitrail is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Ubitrail.  If not, see <http://www.gnu.org/licenses/>.
*/


#include <sstream>
#include "VideoWriter.hpp"

VideoWriter::VideoWriter()
{
}

VideoWriter::VideoWriter(std::vector<Tracker>* trackers, Options& options,cv::Size globalFrameSize, int fps):
m_options(options)
{
    m_trackers = new std::vector<Tracker>;
    m_trackers = trackers;
    globalFrame = m_options.allFrameOutput;
    singleAreas = m_options.videosOutput;

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
        cv::Size size((*trackers)[i].getROI().width,(*trackers)[i].getROI().width);

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
    #pragma omp parallel for schedule (dynamic) num_threads(m_trackers->size())
    for(unsigned int i = 0; i < cvWriters.size(); i++){
            cvWriters[i].write(frame(ROIs[i]));
    }
}
