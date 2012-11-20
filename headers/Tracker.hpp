#ifndef TRACKER_H
#define TRACKER_H

#include "opencv2/imgproc/imgproc.hpp"

#include "defines.hpp"
#include "Area.hpp"
#include "optionStructure.hpp"
#include "MotionSensor.hpp"
#include "ForegroundExtractor.hpp"
#include "FeatureClassifier.hpp"


/**
 * The class responsible for tracking one agent in one region of the video.
 * It needs to be initialised so that is has it's own Area and label as well as general options.
 * A tracker object can be updated by nextFrame(). This will feed a sub-region of a
 * new frame (with it's time stamp) to the model.
 */
class Tracker
{
    public:
        /**
         * The default constructor.
         */
        Tracker();

        /**
         * Another contructor. A Tracker object cannot be used unless it was constructed in this manner.
         * @param area a predefined Area.
         * @param label a string containing the name of this tracker.
         * @param options an Options structure.
         */
        Tracker(Area area,const std::string  label, Options& options );
        virtual ~Tracker();

        /**
         * A public member function updating motionSensor, foregroundExtractor and  featureClassifier.
         * @param wholeFrame an input matrix of the global video frame.
         * @param time the time stamp relative wholeFrame.
         *
         * It ultimately updates the position of the agent. It performs as follow:
         * 1. Uses the ROI of its Area to define a new matrix header from wholeframe.
         * 2. Preprocesses the resulting images to reduce the noise.
         * 3. Send the resulting image to its MotionSensor and gathers a "motionFrame".
         * 4. Sends the motionFrame to its ForegroundExtractor and gathers contours of detected objects
         * 5. If there are contours and the ForegroundExtractor has been trained enought,
         * then the contours are sent to the FeatureClassifier. If more than one contour were present,
         * it determines a valid one. and keep it's position in memory.
         */
        void nextFrame(const cv::Mat& wholeFrame, double time);

        /**
         * A public member function getter for isPositionValid
         * @return true if an object was detected during last run of nextFrame().
         */
        bool getIsValid(){
            return isPositionValid;
        }

        /**
         * A public member function getter of X position.
         * @return The last valid X position (relative to the dimention of the Area).
         */
        float getX(){
            return (float)position.x/(float)m_area.getROI().width;
        }

        /**
         * A public member function getter of Y position.
         * @return The last valid Y position (relative to the dimention of the Area).
         */
        float getY(){
            return (float)position.y/(float)m_area.getROI().height;
        }

        /**
         * A public member function getter of the likelihood from the FeatureClassifier.
         * @return The likelihood of the last valid object.
         */
        float getLikelihood(){
            return featureClassifier.getL();
        }

        /**
         * A public member function getting the approximate training progress.
         * @return A fraction F.
         *
         * \f$ F = \frac{FET+FCT}{2} \f$ \n
         * Where \f$FET\f$ is the training fraction for the ForegroundExtractor,
         * and  \f$FCT\f$ is the training fraction for the FeatureClassifier.
         */
        float getTrainedFrac(){
            return (featureClassifier.trainedFract()+foregroundExtractor.trainedFract()) /2;
        }

        /**
         * A public member function getter of m_label.
         * @return The label for this object.
         */
        std::string getLabel(){
            return m_label;
        }

        /**
         * A public member function getting the territory in which the agent was.
         * @return The territory of the last valid position.
         */
        int getTerrit(){
            return m_area.inWhichTerritIsPoint(cv::Point2f(position.x,position.y));
        }

        /**
         * A public member function getting isTrained, (i.e. ready to output data).
         * @return true if the model is completely trained.
         */

        bool getIsTrained(){
            return isTrained;
        }

        /**
         * A public member function getting the map of territories for the related Area.
         * @return The territory map.
         */
        cv::Mat& getTerritMap(){
            return m_area.getTerrit();
        }
        /**
         * A public member function getting the ROI of the related Area.
         * @return The bounding rectangle of this object.
         */
        cv::Rect& getROI(){
            return m_area.getROI();
        }

    protected:
        void preprocess(const cv::Mat& in,cv::Mat& out);
    private:

        Area m_area;
        std::string m_label;
        MotionSensor motionSensor;
        ForegroundExtractor foregroundExtractor;
        FeatureClassifier featureClassifier;
        cv::Mat accum;
        bool wasAmbiguous;
        bool isPositionValid;
        cv::Point2f position;
        bool isTrained;
        int m_agentSize;

};

#endif // TRACKER_H
