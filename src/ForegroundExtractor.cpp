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
    along with Foobar.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "ForegroundExtractor.hpp"

ForegroundExtractor::ForegroundExtractor(){
}

ForegroundExtractor::ForegroundExtractor(int roundToTrain):
mog(FOREGROUND_EXTRACTOR_HISTORY, FOREGROUND_EXTRACTOR_THR, false),
roundTrained(0),
trainingRate(FOREGROUND_EXTRACTOR_TRAINING_SINGLE_BLOB),
m_roundToTrain(roundToTrain)
{
}

ForegroundExtractor::~ForegroundExtractor()
{
}
bool ForegroundExtractor::run(const cv::Mat& img, const cv::Mat& mot, int motionQ,bool wasAmbig,std::vector<std::vector<cv::Point> >& validContours){

    cv::Mat seeds;
    cv::Mat out;

    if(motionQ > 0){ //todo rational
        this->updateTrainingRate();
        //
        double modifier;
        if(!wasAmbig || roundTrained < m_roundToTrain){
            modifier = 1;
        }
        else{
            modifier = FOREGROUND_MODIF_MANY_BLOBS;
        }

        mog(img, out,trainingRate * modifier);

        cv::dilate(out,out,cv::Mat(),cv::Point(-1,-1),1);
        cv::erode(out,out,cv::Mat(),cv::Point(-1,-1),2);


        cv::bitwise_and(out,mot,seeds);
        std::vector<std::vector<cv::Point> > contours;
        cv::findContours(out, contours,CV_RETR_EXTERNAL,CV_CHAIN_APPROX_SIMPLE);
        validContours.resize(contours.size());
        size_t idx = 0;
        for(size_t i = 0; i < contours.size(); i++){
            if(contours[i].size() > 5){
                cv::Rect boundRect =  cv::boundingRect(contours[i]);
                cv::Mat miniMask(boundRect.height,boundRect.width,CV_8U,cv::Scalar(0));
                cv::approxPolyDP(contours[i],contours[i],5,true);
                cv::drawContours(miniMask,contours,i,cv::Scalar(255),-1,8,cv::noArray(),INT_MAX ,cv::Point(-boundRect.x,-boundRect.y));
                cv::bitwise_and(miniMask,seeds(boundRect),miniMask);
                if (cv::countNonZero(miniMask) > 0){
                    validContours[idx] = contours[i]; //todo copyTo
                    idx++;
                }
            }
        }

        validContours.resize(idx);

        this->mergeContours(seeds,validContours);
        unsigned int beforeLargeRemoval = validContours.size();
        this->removeLargeContours(validContours, (img.cols+img.rows)/5);//magic number

        if(validContours.size() >1 || beforeLargeRemoval >  validContours.size())
            return false;
        else
            return true;
    }
    else{
        validContours.resize(0);
        return false;
    }
}
void ForegroundExtractor::updateTrainingRate(){
        if(roundTrained < m_roundToTrain){
            trainingRate = FOREGROUND_EXTRACTOR_STARTING_TRAINING_RATE -
            (FOREGROUND_EXTRACTOR_STARTING_TRAINING_RATE - FOREGROUND_EXTRACTOR_TRAINING_SINGLE_BLOB)*
            ((double)roundTrained/m_roundToTrain);
            roundTrained++;
        }
        else{
            trainingRate = FOREGROUND_EXTRACTOR_TRAINING_SINGLE_BLOB;
        }
}
void ForegroundExtractor::mergeContours(cv::Mat& img, std::vector<std::vector<cv::Point> >& contours){
    if(contours.size()>1){
        std::vector<double>longestChord(contours.size());
        for(unsigned int i = 0; i < contours.size(); i++){
            double longest = 1;
            for(unsigned int j = 0; j < contours[i].size(); j++){
                for(unsigned int k = j; k < contours[i].size(); k++){
                    cv::Point dif = contours[i][j] - contours[i][k];
                    double dist = sqrt((double)(dif.x*dif.x)+(double)(dif.y*dif.y));
                    if(dist >longest )
                        longest = dist;
                }
            }
            longestChord[i] = longest;
        }
        std::vector<std::vector<cv::Point> > validLines;
        img.setTo(0);
        for(unsigned int i = 0; i < contours.size(); i++){
           for(unsigned int j = i+1; j < contours.size(); j++){
                double smallestDist = INT_MAX;
                std::vector<cv::Point> validPair(2) ;
                for(unsigned int k = 0; k < contours[i].size(); k++){
                    for(unsigned int l = 0; l < contours[j].size(); l++){
                        cv::Point dif = contours[i][k] - contours[j][l];
                        double dist = sqrt((double)(dif.x*dif.x)+(double)(dif.y*dif.y));
                        if(dist < smallestDist ){
                            smallestDist = dist;
                            validPair[0] = contours[i][k];
                            validPair[1] = contours[j][l];
                        }

                    }


                }

                double chordCoeff = sqrt(longestChord[i] * longestChord[j]);
                if(smallestDist/chordCoeff < FOREGROUND_EXTRACTOR_GROUPING_FACTOR_THRETHOLD)
                    validLines.push_back(validPair);
            }
        }
        cv::drawContours(img,contours,-1,cv::Scalar(255),-1,8);//200 ? why ?
        cv::drawContours(img,validLines,-1,cv::Scalar(255),-1,8);//200 ? why ?
        cv::Mat img2;
        img.copyTo(img2);
        cv::findContours(img2, contours,CV_RETR_EXTERNAL,CV_CHAIN_APPROX_SIMPLE);
    }
}
void ForegroundExtractor::removeLargeContours(std::vector<std::vector<cv::Point> >& contours, double max){
        std::vector<std::vector<cv::Point> > validConts(contours.size());
        int idx = 0;
        for(unsigned int i = 0; i < contours.size(); i++){
            cv::RotatedRect rrect;
            rrect = cv::minAreaRect(contours[i]);
            cv::Point2f vertices[4];
            rrect.points(vertices);
            cv::Point dif = vertices[0] - vertices [2];
            double dist = sqrt(dif.x*dif.x + dif.y*dif.y);
            if( dist < max){
                validConts[idx] = contours[i];
                idx++;
            }
        }
    validConts.resize(idx);
    contours = validConts;
}


