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

#include "Processor.hpp"
#include "ROIMaker.hpp"
#include "Area.hpp"
#include "VideoWriter.hpp"

Processor::Processor( Options options, VideoGrabber videoGrab, bool hasGUI,cv::Mat* preview):
    m_options(options),
    m_videoGrab(videoGrab),
    end(false),
    m_hasGUI(hasGUI)
{
#ifdef WITH_GUI
    if(m_hasGUI)
        ROIForGUI = -1;
    NFramesSpeedAssess = 0;
#endif

    m_videoGrab.reset();
    cv::Mat bg;
    this->makeBG(bg);
    assert(!bg.empty());
    bg.copyTo(deco);
    ROIMaker roiMaker(m_options,bg);
    std::vector<cv::Rect> ROIs = roiMaker.getROIs();
    std::vector<cv::Mat> masks = roiMaker.getMasks();
    std::vector<std::string> labels = makeLabels(ROIs);
    trackers.resize(ROIs.size());
    for(unsigned int i = 0; i< ROIs.size(); i++){
        Area area = Area(ROIs[i],bg(ROIs[i]),masks[i],m_options.nLinePerDishes,m_options.usesMaskForTerrit);
        trackers[i] = Tracker(area,labels[i],m_options);
    }
    m_decorator = Decorator(&trackers,bg.size());

    m_decorator.newFrame(deco);

    if(preview != NULL){
        m_decorator.getDecoratedFrame(deco);
        deco.copyTo(*preview);
    }
    else{
        m_resultWriter = new ResultWriter(&trackers,m_options);
        m_videoWriter = VideoWriter(&trackers,m_options,bg.size(),m_videoGrab.getFPS());
    }


     if(m_options.writeFirstPicture && !preview){
//        std::string tmp (;
        m_decorator.getDecoratedFrame(deco);
        cv::imwrite(m_options.outDir+"/FirstPicture.jpg",deco);
    }

}

Processor::~Processor()
{
    //dtor
//    delete m_resultWriter;
}

void Processor::track(){
    end = false;
    cv::Mat frame;
    double time;
    bool first=true;
    refuseToSendDecoratedFrame = false;
    gettimeofday(&startT, NULL);

    while(!end){

    #pragma omp parallel num_threads(trackers.size()) //shared(frame)
        {
            #pragma omp master
            {
                if(first){
                    end  = !m_videoGrab.getFrame(frame,&time);
                }
            }
            #pragma omp for nowait schedule(dynamic)
                for(unsigned int i = 0; i< trackers.size(); i++){
                    if(!first){
                        trackers[i].nextFrame(frame,time);
                        m_decorator.plot(i);
                        m_resultWriter->writeTracker(i,time);
                    }
                }
            #pragma omp master
            {
                if(!first){
                    end  = !m_videoGrab.getFrame(frame,&time);
                }
                if(!end){
                    m_decorator.newFrame(frame);
                    first = false;
                }
            }
            #pragma omp barrier

            #pragma omp sections
            {
                #pragma omp section
                {
                m_resultWriter->flush();
                }
                #pragma omp section
                {

    /*GUI*/
    #ifdef WITH_GUI
    if(m_hasGUI){
                    {
//                Glib::Threads::Mutex::Lock lock (m_mutexDeco);
                Glib::Mutex::Lock lock (m_mutexDeco);
                m_decorator.getDecoratedFrame(deco);
                if(!end)
                    refuseToSendDecoratedFrame = false;
                    }
                    {
//                Glib::Threads::Mutex::Lock lock (m_mutexXYTrTe);
                Glib::Mutex::Lock lock (m_mutexXYTrTe);
                this->updateXYTrainedTerrit();
                    }
    NFramesSpeedAssess++;
    }
    else
        m_decorator.getDecoratedFrame(deco);
#endif
                m_videoWriter.newFrame(deco);
                }
            }
        }
    }
}


void Processor::makeBG(cv::Mat& bg){
    cv::Mat accum;

    for(unsigned int i=0;i < N_AVERAGED_FRAMES_BG; i++){
        double t;
        cv::Mat tmp;
        m_videoGrab.getFrame(tmp,&t);
        if(accum.empty())
            tmp.convertTo(accum,CV_32F);
        else
        cv::accumulate(tmp,accum);

    }
    accum = accum/N_AVERAGED_FRAMES_BG;
    accum.convertTo(bg,CV_8U);
}

std::vector<std::string> Processor::makeLabels(std::vector< cv::Rect > newROIs){


    std::vector< unsigned int > used;
    std::vector< int > labIdx;
    int accu=0;
    double lim=0.5;
    while(used.size() < newROIs.size()){
        unsigned int idx=0;
        double firstLinerY = INT_MAX ;
        /** finds the top of the next row*/
        for(unsigned int i =0; i<newROIs.size();i++){
            bool isUsed=false;
            for(unsigned int j=0; j<used.size();j++){
                if(used[j]==i){
                isUsed = true;
                }
            }
            if(newROIs[i].y < firstLinerY && !isUsed){
                firstLinerY = newROIs[i].y;
                idx = i;
            }
        }
        std::vector< int >ROIsIdxRow;
        ROIsIdxRow.push_back(idx);
        used.push_back(idx);
        for(unsigned int i =0; i<newROIs.size();i++){
            bool isUsed=false;
            for(unsigned int j=0; j<used.size();j++){
                if(used[j]==i){
                isUsed = true;
                }
            }
            if(newROIs[i].y < newROIs[ROIsIdxRow[0]].y + newROIs[ROIsIdxRow[0]].height*lim && !isUsed){
                ROIsIdxRow.push_back(i);
                used.push_back(i);
            }
        }
        std::vector< unsigned int > used2;
        while(used2.size() < ROIsIdxRow.size()){
            int first=0;
            int firstX=INT_MAX;
            for(unsigned int i =0; i<ROIsIdxRow.size();i++){
                bool isUsed=false;
                for(unsigned int j=0; j<used2.size();j++){
                    if(used2[j]==i){
                    isUsed = true;
                    }
                }
                if(newROIs[ROIsIdxRow[i]].x<firstX && !isUsed){
                    first=i;
                    firstX=newROIs[ROIsIdxRow[i]].x;
                }
            }
            used2.push_back(first);
            labIdx.push_back(ROIsIdxRow[first]);
    }
    accu=accu+ROIsIdxRow.size();
    }//end of big while
    std::vector< unsigned int > labss(labIdx.size());
    for(unsigned int i =0; i<used.size();i++){
        labss[labIdx[i]]=i;
    }
    std::vector<std::string> labels;
    for(unsigned int i =0; i<labss.size();i++){
        char buffer [10];
        sprintf(buffer,"%02i",labss[i]);
        std::string ts(buffer);

        labels.push_back(ts);
    }

    return labels;
}


#ifdef WITH_GUI
void Processor::setROIForGUI(std::string ROIName){
    if(ROIName == "")
        ROIForGUI = -1;
    else{
        int idx = -1;
        for(unsigned int i = 0; i < trackers.size(); i++){
            if(trackers[i].getLabel() == ROIName){
                    idx = (int) i;
            }
        }
        ROIForGUI = idx;
    }
    this->updateWHXY_ROI();
}

void Processor::updateWHXY_ROI(){
    if(ROIForGUI < 0){
            m_WHXY_ROI[0] = -1;
            m_WHXY_ROI[1] = -1;
            m_WHXY_ROI[2] = -1;
            m_WHXY_ROI[3] = -1;
    }
    else{
        cv::Rect tmpRec = trackers[ROIForGUI].getROI();
        m_WHXY_ROI[0] = (tmpRec.br()-tmpRec.tl()).x;
        m_WHXY_ROI[1] = (tmpRec.br()-tmpRec.tl()).y;
        m_WHXY_ROI[2] = tmpRec.tl().x;
        m_WHXY_ROI[3] = tmpRec.tl().y;
    }
}

void Processor::updateXYTrainedTerrit(){
    if(ROIForGUI>=0){
        bool tmpB = trackers[ROIForGUI].getIsTrained();
        if(tmpB){
            m_XYTrTe[0] = trackers[ROIForGUI].getX();
            m_XYTrTe[1] = trackers[ROIForGUI].getY();
            m_XYTrTe[2] = 1;
            m_XYTrTe[3] = trackers[ROIForGUI].getTerrit();

        }
        else{
            m_XYTrTe[0] = -1;
            m_XYTrTe[1] = -1;
            m_XYTrTe[2] = trackers[ROIForGUI].getTrainedFrac();
            m_XYTrTe[3] = -1;
        }
    }
}

bool Processor::getDeco(cv::Mat& out){
    if(!refuseToSendDecoratedFrame){
        if(ROIForGUI == -1){
//            Glib::Threads::Mutex::Lock lock (m_mutexDeco);
            Glib::Mutex::Lock lock (m_mutexDeco);
            deco.copyTo(out);
        }
        else{
//            Glib::Threads::Mutex::Lock lock (m_mutexDeco);
            Glib::Mutex::Lock lock (m_mutexDeco);
            deco(trackers[ROIForGUI].getROI()).copyTo(out);
        }
        refuseToSendDecoratedFrame = true;
        return true;
    }
    else{
        return false;
    }
}
#endif
