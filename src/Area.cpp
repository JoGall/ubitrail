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

#include "Area.hpp"
#include "opencv2/highgui/highgui.hpp"


Area::Area(){
}

Area::Area(cv::Rect ROI,cv::Mat bg,cv::Mat mask,int nLines,bool useMaskForTerrit):
m_ROI (ROI)
{

    mask.copyTo(m_mask);
    surface = cv::countNonZero(m_mask);

    assert(nLines < 2);

    //todel :
    useMaskForTerrit = true;
    if(useMaskForTerrit){
        territMap = m_mask;
    }

    else if(nLines == 1)
        this->oneLineTerrit(bg);
    else
        cv::threshold(m_mask, territMap,0,1,cv::THRESH_BINARY);
}

Area::~Area(){
}


void Area::oneLineTerrit(cv::Mat bg){
    cv::Mat bbg,edges,binMask;
    std::vector<cv::Vec4i> lines;

    cv::cvtColor(bg,bbg,CV_BGR2GRAY);

    cv::Laplacian(bbg, edges, CV_8U,3);
    cv::adaptiveThreshold(edges,edges,255,cv::ADAPTIVE_THRESH_MEAN_C,cv::THRESH_BINARY,25,-10);

    cv::erode(m_mask,binMask,cv::Mat(),cv::Point(0,0),m_mask.cols/10);
    edges=edges-(255-binMask);

    cv::HoughLinesP( edges, lines, 1, CV_PI/3600, 10, edges.cols/5,edges.cols/10 );
    if(lines.size()>0){
        double x1 = lines[0][0],
        y1 = lines[0][1],
        x2 = lines[0][2],
        y2 = lines[0][3];
        double a,b; //y=ax+b
        a = (y1-y2)/(x1-x2);
        b= y1-a*x1;
        cv::Point2f P1(0,b),P2(edges.cols,a*edges.cols+b);
        m_mask.copyTo(territMap);
        for(int x = 0; x<territMap.cols;x++)
            for(int y = 0; y<territMap.cols;y++)
                if((uchar)territMap.at<uchar>(y,x)>0){
                    if( y > a*x+b) territMap.at<uchar>(y,x) = 1;
                    else territMap.at<uchar>(y,x) = 2;
                }
    }
    else{
        cv::threshold(m_mask, territMap,0,1,cv::THRESH_BINARY);
    }

}
