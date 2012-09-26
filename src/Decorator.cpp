#include "Decorator.hpp"

Decorator::Decorator(){

}

Decorator::Decorator(std::vector<Tracker>* trackers, cv::Size imgSize):
staticOverlay(imgSize,CV_8UC4,cv::Scalar(0,0,0,0)),
pos(trackers->size()),
posIsValid(trackers->size())
{
    //ctor
    m_trackers = new std::vector<Tracker>;
    m_trackers = trackers;
    this->makeStaticOverlay();
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
    frameCopy = frameCopy + staticOverlay;
    for(unsigned int i = 0; i<pos.size();i++){
        if(posIsValid[i]){

            cv::circle(frameCopy, pos[i], 3, cv::Scalar(255,0,0,255),3);
            cv::circle(frameCopy, pos[i], 2, cv::Scalar(0,255,255,255),2);
        }
        else if (pos[i].x > -1){
            cv::circle(frameCopy, pos[i], 3, cv::Scalar(0,255,255,255),3);
            cv::circle(frameCopy, pos[i], 2, cv::Scalar(0,0,255,255),2);
        }

    }

    cv::cvtColor(frameCopy,frame,CV_RGBA2BGR);
}

void Decorator::makeStaticOverlay(){
    for(unsigned int i = 0; i< m_trackers->size(); i++){
//        std::vector<std::vector<cv::Point> > contours;
//        cv::Mat mask;
//        (*m_trackers)[i].getTerritMap().copyTo(mask);
//        cv::cvtColor(mask,mask,CV_GRAY2BGR);

        cv::rectangle(staticOverlay,(*m_trackers)[i].getROI().tl(),(*m_trackers)[i].getROI().br(),cv::Scalar(0,255,0,0),2,CV_AA);

        double min, max;
        cv::minMaxLoc((*m_trackers)[i].getTerritMap(), &min,&max);
        for(unsigned int j = 1; j <= max; j++){
            cv::Mat tmp;
            std::vector<std::vector<cv::Point> > conts;
            (*m_trackers)[i].getTerritMap().copyTo(tmp);
            cv::inRange(tmp, j,j, tmp);
            cv::findContours(tmp,conts,CV_RETR_EXTERNAL,CV_CHAIN_APPROX_NONE);
            cv::drawContours(staticOverlay, conts,0, cv::Scalar((double)(j-1)*255/max,0,255-(double)(j-1)*255/max,0), 2, CV_AA,cv::noArray(),INT_MAX,(*m_trackers)[i].getROI().tl());
        }

        cv::Point ori((*m_trackers)[i].getROI().tl().x +2,(*m_trackers)[i].getROI().br().y - 2);
        cv::putText(staticOverlay, (*m_trackers)[i].getLabel(),ori , cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(125,255,125,0), 2,CV_AA,false );

//        cv::cvtColor(staticOverlay,staticGrayTip,CV_BGR2GRAY );
//        staticGrayTip = staticGrayTip * 255;
//        cv::cvtColor(staticGrayTip,staticGrayTip,CV_GRAY2BGR);

    }
}


void Decorator::newFrame(cv::Mat& frame){
//    cv::Mat tmp;
    cv::cvtColor(frame,frameCopy,CV_BGR2RGBA);
//    frameCopy = (frame -staticGrayTip)+ staticOverlay;
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

