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



#include "ROIMaker.hpp"
#include "opencv2/highgui/highgui.hpp"


ROIMaker::ROIMaker(Options& options, cv::Mat& bg):
m_options(options),
m_bg(bg),
isInitialised(false)
{

    if(m_options.nDishes > 1){
        isInitialised = this->isodiametricCirclesAsROIs(0.1);//magic number


    }

    else if(m_options.maskFile != ""){
        cv::Mat img = cv::imread(m_options.maskFile,0);
        isInitialised = ROIsFromImg(img);

    }
    else{
        this->wholeFrameAsROI();
        isInitialised = true;
    }
}

ROIMaker::~ROIMaker()
{
}

void ROIMaker::wholeFrameAsROI(){
    m_ROIs.push_back(cv::Rect(0,0,m_bg.cols,m_bg.rows));
    m_masks.push_back(cv::Mat(cv::Size(m_bg.cols,m_bg.rows),CV_8U,cv::Scalar(255)));
}

bool ROIMaker::isodiametricCirclesAsROIs(double isodiametricity){
    cv::Mat img,mask;
    double maxRad = (double)m_bg.cols/(double)m_options.nDishes+m_bg.rows/m_options.nDishes;
    double minRad = (double)m_bg.cols/(double)(5*m_options.nDishes);
    cv::bilateralFilter(m_bg, img, 7, 1500,1500);
    cv::Mat g1, g2;
    cv::GaussianBlur(img, g1, cv::Size(0, 0), 3);
    cv::GaussianBlur(img, g2, cv::Size(0, 0), 9);
    cv::absdiff(g1,g2,g2);
    img = img - g2;
    cv::cvtColor(img,img, CV_BGR2GRAY);
    cv::equalizeHist(img,img);

    // First pass:
    std::vector<cv::Vec3f> bestCircles = bruteForceHough(img,minRad,maxRad,5,100,3);
    // Updating min and maxRad according to the results:
    int sumOfRads = 0;
    for(unsigned int i = 0; i<m_options.nDishes; i++){
        sumOfRads = sumOfRads+bestCircles[i][2];
    }

    //Second pass:
    double meanRad = (double)sumOfRads/(double)m_options.nDishes;
    minRad = meanRad*(1-isodiametricity);
    maxRad = meanRad*(1+isodiametricity);
    bestCircles = bruteForceHough(img,minRad,maxRad,5,100,2);
    m_ROIs.clear();
    m_masks.clear();

    if(bestCircles.size() != m_options.nDishes)
        return false;
    else{
        m_ROIs.resize(m_options.nDishes);
        m_masks.resize(m_options.nDishes);
        for(unsigned int i = 0; i<m_options.nDishes; i++){
            m_ROIs[i] = cv::Rect(bestCircles[i][0]-bestCircles[i][2],bestCircles[i][1]-bestCircles[i][2],2*bestCircles[i][2]+1,2*bestCircles[i][2]+1);
            cv::Mat mask(cv::Size(m_bg.cols,m_bg.rows),CV_8U,cv::Scalar(0));
            cv::circle(mask,cv::Point(bestCircles[i][0],bestCircles[i][1]),bestCircles[i][2],cv::Scalar(255),-1);
            mask(m_ROIs[i]).copyTo(m_masks[i]);
        }
        return true;
    }
}

double ROIMaker::scoreSetOfCircles(std::vector<cv::Vec3f> circles){
    double ssem=0, sum = 0;
    double mean;
    for(unsigned int j=0;j<m_options.nDishes;j++){
        sum = sum + circles[j][2];
    }
    mean = sum/m_options.nDishes;
    for(unsigned int j=0;j<m_options.nDishes;j++){
        double em = mean - circles[j][2];
        ssem = 1/(ssem + em*em);
    }
    return ssem;
}

bool ROIMaker::filterCircles(std::vector<cv::Vec3f> & circles, int w, int h){
    std::vector<cv::Vec3f> circlesToKeep(m_options.nDishes);
    if(circles.size()<m_options.nDishes)
        return false;

    unsigned int j=0;
    for(unsigned int i=0;i < circles.size(); i++){
        bool overlap(false);

        if(j > 0)
            for(unsigned int k=j;k < 0 ; k--){
                double distX,distY, dist;
                distX = (circles[i][0] - circlesToKeep[j][0]);
                distX=distX*distX;
                distY = (circles[i][1] - circlesToKeep[j][1]);
                distY=distY*distY;
                dist = sqrt(distX + distY);
                if (dist < circles[j][2] +  circlesToKeep[j][2])
                    overlap = true;
            }
        if(!overlap && j<m_options.nDishes && (circles[i][0] - circles[i][2]) > 0 && (circles[i][1] - circles[i][2]) > 0 && (circles[i][0] + circles[i][2]) < w && (circles[i][1] + circles[i][2]) < h){



            circlesToKeep[j] = circles[i];
            j++;
        }
    }

    if(circlesToKeep.size() < m_options.nDishes){
        return false;
    }
        circles=circlesToKeep;
        return true;

}

std::vector<cv::Vec3f> ROIMaker::bruteForceHough(cv::Mat img,double minRad, double maxRad, int start,int end,int resol ){
    std::vector<std::vector<cv::Vec3f> > circlesList;
    std::vector< double > ssemList;
    circlesList.resize(1+(end-start)/resol);
    ssemList.resize(1+(end-start)/resol);

    #pragma omp parallel for schedule(dynamic)
    for(int e=start; e<end; e += resol){
        cv::Mat tmp;
        std::vector<cv::Vec3f> circles;
        cv::HoughCircles(img, circles, CV_HOUGH_GRADIENT, 1, minRad*2, e,5, minRad, maxRad );
        bool areCirclesOK(filterCircles(circles,img.cols,img.rows));
        double ssem;

        if(areCirclesOK)
            ssem = this->scoreSetOfCircles(circles);
        else
            ssem = 0;
        int i = (e - start)/resol;
        circlesList[i] = circles;
        ssemList[i] = ssem;
        }
    double bestSsem=0;
    std::vector<cv::Vec3f> bestCircles;
    for(int e=start; e<end; e += resol){
        int i = (e - start)/resol;
        if( bestCircles.size()<m_options.nDishes ||  ssemList[i]> bestSsem){
            bestCircles=circlesList[i];
            bestSsem=ssemList[i];
        }
    }
    return bestCircles;
}


bool ROIMaker::ROIsFromImg(cv::Mat& img){
    std::vector<std::vector<cv::Point> > contours;
    cv::findContours(img, contours,CV_RETR_EXTERNAL,CV_CHAIN_APPROX_SIMPLE);
    img = cv::Scalar::all(0);

    if(contours.size() == 0)
        return false;

    m_ROIs.resize(contours.size());
    m_masks.resize(contours.size());

    for(size_t i = 0; i < contours.size(); i++){
        assert(contours[i].size() > 4);
        cv::Rect boundRect =  cv::boundingRect(contours[i]);
        cv::Mat miniMask(boundRect.height,boundRect.width,CV_8U,cv::Scalar(0));
        cv::drawContours(miniMask,contours,i,cv::Scalar(255),-1,8,cv::noArray(),INT_MAX ,cv::Point(-boundRect.x,-boundRect.y));
        m_ROIs[i] = boundRect;
        miniMask.copyTo(m_masks[i]);
    }
    return true;
}
