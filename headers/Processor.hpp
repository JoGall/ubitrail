#ifndef PROCESSOR_H
#define PROCESSOR_H

#include "defines.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "optionStructure.hpp"
#include "VideoGrabber.hpp"
#include "VideoWriter.hpp"
#include "Tracker.hpp"
#include "ResultWriter.hpp"
#include "Decorator.hpp"

#ifdef WITH_GUI
#include <glibmm/threads.h>
#endif
/**
 * The class that is responsible for the processing.
 * The class starts from a set of predefined options and a capture device.
 * When constructed, a Processor object make instances of Tracker class.
 * The member function run() can then be called once to perform the whole analysis.
 */
class Processor
{
    public:
        /**
         * The contructor.
         * It builds Areas (via ROIMaker), make a label relative each area (via makeLabels()).
         * Uses each area and its label to make a Tracker objects.
         * It also uses these informations to initialises an instances Decorator,
         * VideoWriter and ResultWriter.
         * @param options a structre containing a all parameters.
         * @param videoGrab an initialised instance of VideoGrabber.
         * @param hasGUI a boolean indication whether or not a GUI will be used by the program
         * @param preview an optional pointer to a cv::Mat. If it is non NULL, the constructor will output
         * a preview of the decorated frame.
         */
        Processor(Options options, VideoGrabber videoGrab, bool hasGUI = false,cv::Mat* preview = NULL);

        /**
        * The destructor.
        */
        virtual ~Processor();

        /**
        * A public member function performing the tracking from A to Z.
        * It loops through the video input:
        * 1. Decodes a frame
        * 2. Sends the frame to each tracker
        * 3. Buffers results from each tracker
        * 4. Write the results for all trackers
        * 5. Sends the frame to the Decorator
        * 6. Gets the decorated frame
        * 7. Append the decorated frame to optional videos
        * This process takes great advantage of multicore architectures.
        */
        void track();

#ifdef WITH_GUI
        void setROIForGUI(std::string ROIName);

        bool getDeco(cv::Mat& out);

        float* getTrackerXYTrainedTerrit(){
            Glib::Threads::Mutex::Lock lock (m_mutexXYTrTe);
            return m_XYTrTe;
        }

        int* getTrackerWHXY_ROI(){
            std::cout<<__LINE__<<std::endl;
            return m_WHXY_ROI;
        }

        void setIsFinished(bool b){
            end = b;
        }

        bool getIsFinished(){
            return end;
        }
        double getProgress(){
            return m_videoGrab.getProgress();
        }

        long getTime(){
            gettimeofday(&endT, NULL);
            return (long)((endT.tv_sec  - startT.tv_sec) * 1000 + (long)(endT.tv_usec - startT.tv_usec)/1000.0) + 0.5;
        }


        std::vector<std::string> getROILabels(){
            std::vector<std::string> labs(trackers.size());
            for(unsigned int i = 0; i < trackers.size(); i++){
                labs[i] = trackers[i].getLabel();
            }
            return labs;
        }
#endif
/**/
    protected:
        /**
        * A protected member used to make an initial background image.
        * It simply average frames into the background image. It also controls that frames are actualy readable.
        * @param bg an output cv::Mat.
        */
        void makeBG(cv::Mat& bg);

        /**
        * A protected member used to sort labels in reading (human) order.
        * For instance, if automatic detection of area is used on image of an array of circle: \n
        * ooo \n ooo \n ooo \n
        * the index of the Area are going to be arbitrary, for instance: \n
        * 3,4,8 \n 0,5,1 \n 2,6,7 \n
        * This function will attemps to defines labels in natural human reaqding order instead: \n
        * 0,1,2 \n 3,4,5 \n 6,7,8 \n
        * This is done by only using relative coordinate and dimentions of the areas
        * @param newROIs, an input vector of ROIs.
        * @return a vector of strings the same size than newROIs containing labels for each newROIs.
        */
        std::vector<std::string> makeLabels(std::vector< cv::Rect > newROIs);

#ifdef WITH_GUI
        void updateWHXY_ROI();
        void updateXYTrainedTerrit();
#endif
    private:
        //a comment here
        Options m_options;
        VideoGrabber m_videoGrab;
        std::vector< Tracker > trackers;
        ResultWriter *m_resultWriter;
        Decorator m_decorator;
        VideoWriter m_videoWriter;
        cv::Mat deco;
        timeval startT,endT;
        bool refuseToSendDecoratedFrame;
        bool end;

#ifdef WITH_GUI
        Glib::Threads::Mutex m_mutexDeco,m_mutexXYTrTe;
        int ROIForGUI;
        float m_XYTrTe[4];
        int m_WHXY_ROI[4];
        bool m_hasGUI;
#endif
};

#endif // PROCESSOR_H
