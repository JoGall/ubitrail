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


#include "Decorator.hpp"

Decorator::Decorator(){

}

Decorator::Decorator(std::vector<Tracker>* trackers, cv::Size imgSize):
pos(trackers->size()),
posIsValid(trackers->size()),
staticOverlay_BGR(3),
frameCopy_BGR(3)
{
    //ctor
    m_trackers = new std::vector<Tracker>;
    m_trackers = trackers;
    this->makeStaticOverlay(imgSize);
    for(unsigned int i = 0; i<pos.size();i++){
        pos[i] = cv::Point(-1,-1);
    }


}

Decorator::~Decorator()
{
    //dtor
//    delete
}

void Decorator::getDecoratedFrame(cv::Mat& frame){
    cv::merge(frameCopy_BGR,frame);
//    for(unsigned int i = 0; i<pos.size();i++){
//        if(posIsValid[i]){
//            cv::circle(frame, pos[i], 3, cv::Scalar(255,0,0),3);
//            cv::circle(frame, pos[i], 2, cv::Scalar(0,255,255),2);
//        }
//        else if (pos[i].x > -1){
//            cv::circle(frame, pos[i], 3, cv::Scalar(0,255,255),3);
//            cv::circle(frame, pos[i], 2, cv::Scalar(0,0,255),2);
//        }
//
//    }

}

void Decorator::makeStaticOverlay(cv::Size imgSize){

    cv::Mat staticOverlay(imgSize,CV_8UC3,cv::Scalar(0,0,0));
    for(unsigned int i = 0; i< m_trackers->size(); i++){
        cv::rectangle(staticOverlay,(*m_trackers)[i].getROI().tl(),(*m_trackers)[i].getROI().br(),cv::Scalar(0,255,0,0),2,CV_AA);

        double min, max;
        cv::minMaxLoc((*m_trackers)[i].getTerritMap(), &min,&max);
        for(unsigned int j = 1; j <= max; j++){
            cv::Mat tmp;
            std::vector<std::vector<cv::Point> > conts;
            (*m_trackers)[i].getTerritMap().copyTo(tmp);
            cv::inRange(tmp, j,j, tmp);
            cv::findContours(tmp,conts,CV_RETR_EXTERNAL,CV_CHAIN_APPROX_NONE);
            cv::drawContours(staticOverlay, conts,0, cv::Scalar((double)(j-1)*255/max,0,255-(double)(j-1)*255/max), 2, CV_AA,cv::noArray(),INT_MAX,(*m_trackers)[i].getROI().tl());
        }

        cv::Point ori((*m_trackers)[i].getROI().tl().x +2,(*m_trackers)[i].getROI().br().y - 2);
        cv::putText(staticOverlay, (*m_trackers)[i].getLabel(),ori , cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(125,255,125), 2,CV_AA,false );

        cv::cvtColor(staticOverlay,staticOverlayMask,CV_BGR2GRAY );
        cv::split(staticOverlay,staticOverlay_BGR);

    }
}


void Decorator::newFrame(cv::Mat& frame){
    cv::split(frame,frameCopy_BGR);
    unsigned int nc=frame.cols;
    unsigned int nl=frame.rows;
    int st = staticOverlayMask.step;
    int es = staticOverlayMask.elemSize();

        for(unsigned int j=0; j<nl;j++){
            for(unsigned int i=0; i<nc;i++){
                for(unsigned int k=0; k<3;k++){

                if(*(staticOverlayMask.data+j*st+i*es)>0){

                    double transp = DECORATOR_TRANSPAR ;// * (1-(255.0 - val)/255.0) ;
                    *(frameCopy_BGR[k].data+j*st+i*es) =
                        *(staticOverlay_BGR[k].data+j*st+i*es) * (1.0 - transp)
                        + *(frameCopy_BGR[k].data+j*st+i*es) * transp ;
                }
            }
        }
    }
}

void Decorator::plot(int i){
    if((*m_trackers)[i].getIsValid()){
        float x = (*m_trackers)[i].getX() * (*m_trackers)[i].getROI().width + (*m_trackers)[i].getROI().x;
        float y = (*m_trackers)[i].getY() * (*m_trackers)[i].getROI().height + (*m_trackers)[i].getROI().y;
        pos[i] = cv::Point2f(x,y);
        posIsValid[i] = true;
    }
    else
        posIsValid[i] = false;

}

