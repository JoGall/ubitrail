
#include "OptionParser.hpp"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <cstdio>
#include <iostream>
#include <sstream>


OptionParser::OptionParser(){

}

OptionParser::OptionParser(int argc, char **argv):
valid(true) //Assumes that argument are valid
{
std::stringstream tss;
std::string ts;

//if no argument, we put -A so that Ubitrai uses the assistant
if (argc <=1 ){
    opts.hasAssistant= true;
}

char c=0;
    while ((c = getopt (argc, argv, "GAhraipd:v:l:w:t:s:o:")) != -1)

         switch (c)
           {

        case 'h':
             valid = false;
             break;

        case 'G':
             opts.hasGUI= true;
             break;
        case 'A':
             opts.hasAssistant= true;
             break;
        case 'r':
             opts.resultFile = true;
             break;
        case 'a':
             opts.allFrameOutput = true;
             break;
        case 'i':
             opts.videosOutput = true;
             break;
        case 'p':
             opts.writeFirstPicture = true;
             break;
        case 'd':
             opts.nDishes = atoi(optarg);
             break;
        case 'v':
            tss<<optarg;
            tss>>ts;
            //TODO erase tss & ts
            opts.videoFile = ts;
            break;
        case 'l':
            opts.nLinePerDishes = atoi(optarg);
            break;
        case 'w':
            opts.webCamIdx = atoi(optarg);
            break;
        case 's':
            opts.motionSensitivity = atoi(optarg);
            break;
        case 't':
            opts.MOGTrainingRounds = atoi(optarg);
            break;
        case 'o':
            opts.outDir = optarg;
            break;

           case '?':
            valid = false;
            if (optopt == 'c')
               fprintf (stderr, "Option -%c requires an argument.\n", optopt);
            else if (isprint (optopt))
                fprintf (stderr, "Unknown option `-%c'.\n", optopt);
            else
                fprintf (stderr,"Unknown option character `\\x%x'.\n", optopt);
                break;
           default:
                break;
           }
}

bool OptionParser::checkOptions(){
    if(opts.hasAssistant)
        return true;

    if(opts.videoFile == "" && opts.webCamIdx == -1){
        std::cerr<<"You have not selected any input (file or webcam)."<<std::endl
        <<"Use -v <FILE> for a video file"<<std::endl
        <<"Or -w <N> to grep frames from the webcam N"<<std::endl;
        valid = false;
    }
    if(opts.videoFile != "" && opts.webCamIdx > -1){
        std::cerr<<"You cannot select both a webcam and a video file."<<std::endl
        <<"Use -v <FILE> for a video file"<<std::endl
        <<"Or -w <N> to grep frames from the webcam N"<<std::endl;
        valid = false;
    }

    if(opts.nLinePerDishes < 0 ){
        std::cerr<<"Wrong argument for option -l"<<std::endl
        <<"The number of line per dished must be positive!"<<std::endl;
        valid = false;
    }

    if(opts.motionSensitivity < 0 || opts.motionSensitivity > 1000){
        std::cerr<<"Wrong argument for option -s"<<std::endl
        <<"The motion sensitivity has to be in the 0:1000 range!"<<std::endl;
        valid = false;
    }

    if(opts.MOGTrainingRounds < 0 ){
        std::cerr<<"Wrong argument for option -t"<<std::endl
        <<"The minimal training value has to be positive!"<<std::endl;
        valid = false;
    }

    if(opts.outDir == ""){
        if(opts.resultFile){
            std::cerr<<"Wrong combinaison of arguments"<<std::endl
            <<"Option \"-r\"(write a result file(text)) requiers option \"-o\"(output directory) to have a value "<<std::endl;
            valid = false;
        }
        if(opts.videosOutput){
            std::cerr<<"Wrong combinaison of arguments"<<std::endl
            <<"Option \"-v\"(write individual videos) requiers option \"-o\"(output directory) to have a value "<<std::endl;
            valid = false;
        }
        if(opts.allFrameOutput){
            std::cerr<<"Wrong combinaison of arguments"<<std::endl
            <<"Option \"-a\"(write global video) requiers option \"-o\"(output directory) to have a value "<<std::endl;
            valid = false;
        }
        if(opts.writeFirstPicture){
            std::cerr<<"Wrong combinaison of arguments"<<std::endl
            <<"Option \"-p\"(save the first picture) requiers option \"-o\"(output directory) to have a value "<<std::endl;
            valid = false;
        }
    }


    if(!valid){
        this->help();
        return false;
    }
    else
        return true;
}

void OptionParser::help(){

std::cout<<std::endl<<std::endl<<
"USAGE"<<std::endl<<
std::endl<<std::endl<<
"OPTIONS"<<std::endl<<
"\t-A Start the GUI assistant and ignaore the other options"<<std::endl<<
"\t-G Start the Program in GUI mode"<<std::endl<<
"\t-o DIRECTORY\t An existing output directory path to store outputs"<<std::endl<<
    "\t\t-r\t Make a text output in DIRECTORY"<<std::endl<<
    "\t\t-i\t Make individual video outputs (one per area) in DIRECTORY"<<std::endl<<
    "\t\t-a\t Make a global video output in DIRECTORY."<<std::endl<<
    "\t\t-p\t Saves the first global frame in DIRECTORY"<<std::endl<<
std::endl<<
"\t-d N\t N circular dishes will be detected and used as areas (N must be greater then two)"<<std::endl<<
    "\t\t-l M\t in each of the N dishes, M lines will be detected in order to define territoties (so far, only 0 or 1 are supported) "<<std::endl<<
std::endl<<
"\t-v FILE\t The program will process the video FILE. See NOTES for information about the suported formats"<<std::endl<<
std::endl<<
"\t-w I\t The program will process data from the capture device of index I (I is a positive or null integer)."<<std::endl<<
std::endl<<
"\t-s N\t The motion sensitivity value (between 0 and 255). The lower N is, the more sensitive the program will be. The default is 5."<<std::endl<<
std::endl<<
"\t-t N\t The number of frames used to train the background. The default is 500."<<std::endl<<
std::endl<<
"\t-h\t Print the help"<<std::endl<<
std::endl<<std::endl<<
"NOTES"<<std::endl<<
std::endl<<std::endl<<
"EXAMPLES"<<std::endl<<
std::endl<<std::endl<<
"COPYING"<<std::endl;

}



