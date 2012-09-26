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
//
class Processor
{
    public:
        Processor(Options options, VideoGrabber videoGrab, bool hasGUI = false,cv::Mat* preview = NULL);
        virtual ~Processor();
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
        void makeBG(cv::Mat& bg);
        std::vector<std::string> makeLabels(std::vector< cv::Rect > newROIs);

#ifdef WITH_GUI
        void updateWHXY_ROI();
        void updateXYTrainedTerrit();
#endif
    private:
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
