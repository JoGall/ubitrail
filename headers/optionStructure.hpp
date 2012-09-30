
#ifndef OPTIONSTRUCTURE_HPP_INCLUDED
#define OPTIONSTRUCTURE_HPP_INCLUDED

#include <iostream>
//#include "opencv2/highgui/highgui.hpp"

struct Options {
        std::string videoFile; // -v PATH
        std::string outDir; // -o directory
        int webCamIdx; // -w N
        unsigned int nDishes; // -d N
        int nLinePerDishes; // -l N
        int motionSensitivity; // -s N
        int MOGTrainingRounds; // -t N
        bool resultFile; // -r
        bool videosOutput; // -i
        bool allFrameOutput; // -a
        bool writeFirstPicture; // -p
        bool hasGUI; // -G
        bool hasAssistant; // -A
        std::string maskFile; //-m

        Options() :
        videoFile(""),
        outDir(""),
        webCamIdx(-1),
        nDishes(0),
        nLinePerDishes(0),
        motionSensitivity(2),
        MOGTrainingRounds(500),
        resultFile(false),
        videosOutput(false),
        allFrameOutput(false),
        writeFirstPicture(false),
        hasGUI(false),
        hasAssistant(false)
        {
        }
};




#endif // OPTIONSTRUCTURE_HPP_INCLUDED
