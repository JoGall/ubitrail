#ifndef AREA_H
#define AREA_H

#include "opencv2/imgproc/imgproc.hpp"


class Area
{
    public:
        Area();
        Area(cv::Rect ROI,cv::Mat bg,cv::Mat mask,int nLines);
        virtual ~Area();
        cv::Mat& getTerrit(){ return territMap;}
        cv::Rect& getROI(){ return m_ROI;}

        int inWhichTerritIsPoint(cv::Point2f p){
            int ter;
            ter = (int) territMap.at<char>(p);
            return ter;
        }


    protected:
        void oneLineTerrit(cv::Mat bg);
    private:

        int surface;
        cv::Mat territMap;
        cv::Mat m_mask;
        cv::Rect m_ROI;

};

#endif // AREA_H
