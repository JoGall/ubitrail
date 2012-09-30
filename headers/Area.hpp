#ifndef AREA_H
#define AREA_H

#include "opencv2/imgproc/imgproc.hpp"

/**
 * The class defines what an "area" is.
 * That is, a ROI (i.e. abounding rectangle),
 * a mask (i.e. a binary image) of the same size than ROI.
 * and a map of territory (an image in which non-zero values are equal to the territory).
 * It also has member functions to define territories automatically.
 */
class Area
{
    public:
        /**
         * The default constructor.
         */
        Area();

        /**
         * Another contructor. An Area object cannot be used unless it was constructed in this manner.
         * @param ROI the bounding rectangle for this area.
         * @param bg a background image that can be used to define territories.
         * @param mask a binary matrix. Non-zero values correspond to active regions.
         * @param nLines the number of lines to detect in each regions. Each line delimitates territories from each other.
         */
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
