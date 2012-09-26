#include "ROIMaker.hpp"


//todel
#include "opencv2/highgui/highgui.hpp"


ROIMaker::ROIMaker(Options& options, cv::Mat& bg):
m_options(options),
m_bg(bg)
{

    if(m_options.nDishes > 1){
        this->isodiametricCirclesAsROIs();
    }
    else{
        this->wholeFrameAsROI();
    }

}

ROIMaker::~ROIMaker()
{
}

void ROIMaker::wholeFrameAsROI(){
    m_ROIs.push_back(cv::Rect(0,0,m_bg.cols,m_bg.rows));
    m_masks.push_back(cv::Mat(cv::Size(m_bg.cols,m_bg.rows),CV_8U,cv::Scalar(255)));
}

void ROIMaker::isodiametricCirclesAsROIs(){
    cv::Mat img,mask;
    std::vector<cv::Vec3f> bestCircles;
    int maxRad = m_bg.cols/m_options.nDishes+m_bg.rows/m_options.nDishes;
    int minRad = m_bg.cols/(5*m_options.nDishes);
    cv::bilateralFilter(m_bg, img, 5, 900,900);
    cv::Mat g1, g2;
    cv::GaussianBlur(img, g1, cv::Size(0, 0), 3);
    cv::GaussianBlur(img, g2, cv::Size(0, 0), 7);



    cv::cvtColor(img,img, CV_BGR2GRAY);
    cv::equalizeHist(img,img);
    double bestSsem=0;
    assert (m_options.nDishes>1);
    #pragma omp parallel for ordered schedule(dynamic)
    for(int e=5;e<100;e = e+3){
        cv::Mat tmp;
        std::vector<cv::Vec3f> circles;
        cv::HoughCircles(img, circles, CV_HOUGH_GRADIENT, 1, minRad*2, e,5, minRad, maxRad );
        if(filterCircles(circles,img.cols,img.rows)){
            double ssem = this->scoreSetOfCircles(circles);

            if( bestCircles.size()<m_options.nDishes ||  ssem> bestSsem){
                 #pragma omp critical(updateBestCircles)
                {
                    bestCircles=circles;
                    bestSsem=ssem;
                }
            }
        }
    }
    assert (bestCircles.size()>=m_options.nDishes);
//    ROIs.clear();
    double sumOfRads=0;
    for(unsigned int i = 0; i<m_options.nDishes; i++){
        sumOfRads = sumOfRads+bestCircles[i][2];
    }
    double meanRad = sumOfRads/m_options.nDishes;
    minRad = meanRad*0.9;
    maxRad = meanRad*1.1;
    bestCircles.clear();
    #pragma omp parallel for ordered schedule(dynamic)
       for(int e=4;e<100;e = e+1){
        cv::Mat tmp;
        std::vector<cv::Vec3f> circles;
        cv::HoughCircles(img, circles, CV_HOUGH_GRADIENT, 1, minRad*2, (double)e,5, minRad, maxRad );
        if(filterCircles(circles,img.cols,img.rows)){
            double ssem = this->scoreSetOfCircles(circles);
            if( bestCircles.size()<m_options.nDishes ||  ssem> bestSsem){
                 #pragma omp critical(updateBestCircles)
                {
                    bestCircles=circles;
                    bestSsem=ssem;
                cv::Mat img2;
                m_bg.copyTo(img2);
                 for(unsigned int i = 0; i<m_options.nDishes; i++){
                    cv::circle(img2,cv::Point(bestCircles[i][0],bestCircles[i][1]),bestCircles[i][2],cv::Scalar(255),2,CV_AA);
                 }
                }
            }
        }
    }



    assert (bestCircles.size()>=m_options.nDishes);
    for(unsigned int i = 0; i<m_options.nDishes; i++){
        m_ROIs.push_back(cv::Rect(bestCircles[i][0]-bestCircles[i][2],bestCircles[i][1]-bestCircles[i][2],2*bestCircles[i][2]+1,2*bestCircles[i][2]+1));
    }

    for(unsigned int i = 0; i<m_options.nDishes; i++){
        mask=cv::Mat(cv::Size(m_bg.cols,m_bg.rows),CV_8U,cv::Scalar(0));
        cv::circle(mask,cv::Point(bestCircles[i][0],bestCircles[i][1]),bestCircles[i][2],cv::Scalar(255),-1);
        m_masks.push_back(mask(m_ROIs[i]));
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
        if(j<m_options.nDishes && (circles[i][0] - circles[i][2]) > 0 && (circles[i][1] - circles[i][2]) > 0 && (circles[i][0] + circles[i][2]) < w && (circles[i][1] + circles[i][2]) < h){
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
