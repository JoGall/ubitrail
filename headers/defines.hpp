#ifndef MY_DEFINES
#define MY_DEFINES
#define WITH_GUI


#define N_AVERAGED_FRAMES_BG 20

#define TRACKER_ACCU_WEIGHT 0.5

#define MOTION_SENSOR_ACCU_WEIGHT 0.2
#define MOTION_SENSOR_HISTORY 100
#define MOTION_SENSOR_THR 16
#define MOTION_SENSOR_TRAINING_MIN 0.001
#define MOTION_SENSOR_TRAINING_DECAY 0.8

#define FOREGROUND_EXTRACTOR_HISTORY 1000
#define FOREGROUND_EXTRACTOR_THR 16
#define FOREGROUND_EXTRACTOR_TRAINING_SINGLE_BLOB 0.00001
#define FOREGROUND_MODIF_MANY_BLOBS 1000
#define FOREGROUND_EXTRACTOR_STARTING_TRAINING_RATE 0.1
//#define FOREGROUND_EXTRACTOR_TRAINING_ROUNDS 500
#define FOREGROUND_EXTRACTOR_GROUPING_FACTOR_THRETHOLD 0.8


#define N_FEATURES 8
#define FEATURE_CLASSIF_ACCU_WEIGHT 0.2
//#define FEATURE_CLASSIF_TRAINING_ROUNDS 500

#define FEATURE_AREA_IDX 0
#define FEATURE_DIST_FROM_FORMER_IDX 1
#define FEATURE_BMEAN_IDX 2
#define FEATURE_BSD_IDX 3
#define FEATURE_GMEAN_IDX 4
#define FEATURE_GSD_IDX 5
#define FEATURE_RMEAN_IDX 6
#define FEATURE_RSD_IDX 7
//#define FEATURE_RMED_IDX 5

#define SQRT2PI 2.506628

#define VIDEO_WRITER_CODEC CV_FOURCC('D','I','V','X')


#define DISPLAY_REFRESH_TIME 30
#define PROGRESS_REFRESH_TIME 1000

//#define VIDEO_WRITER_CODEC CV_FOURCC('M','J','P','G')
//if prototyping:
#include "opencv2/highgui/highgui.hpp"
#include <iostream>


#endif
