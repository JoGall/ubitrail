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


#include <assert.h>
#include "ResultWriter.hpp"
#include <time.h>

ResultWriter::ResultWriter(){

}
//
ResultWriter::ResultWriter(std::vector<Tracker>* trackers, Options& options,int width,int height):
m_options(options),
toSTD(false),
m_width(width),
m_height(height)
{
    if(m_options.resultFile){
        std::string tmp = m_options.outDir+"/Result.csv";
        outputFile.open(tmp.c_str());
        assert(outputFile.is_open());
        toSTD = false;
	}
	else{
        toSTD = true;
	}
    m_trackers = new std::vector<Tracker>;
    m_trackers = trackers;
    tss = new  std::stringstream[m_trackers->size()];
    writeHead();

}

ResultWriter::~ResultWriter(){
    //dtor
    delete tss;
}

void ResultWriter::writeTracker(int i,long time){

    if((*m_trackers)[i].getIsValid()){
        this->writeResultRow((*m_trackers)[i].getLabel(),
                                      (*m_trackers)[i].getTerrit(),
                                      (*m_trackers)[i].getX(),
                                      (*m_trackers)[i].getY(),
                                      time,
                                      (*m_trackers)[i].getLikelihood(),i);
    }

}

void ResultWriter::flush(){

    for(unsigned int i =0; i < m_trackers->size(); i++ ){
        std::string ts;
        if(tss[i].str() != ""){
        tss[i]>>ts;

        if(!toSTD)
            outputFile<<ts<<std::endl;//"\n";
        else
            std::cout<<ts<<std::endl;//"\n";
        }
        tss[i].str("");
    }
}

void ResultWriter::writeResultRow(std::string label, int territory, float X, float Y, float t, float L, int i){
//
//    tss[i]<<"\'"<<label<<"\'"<<",";
    tss[i]<<label<<",";
    tss[i]<<territory<<",";
    tss[i]<<std::setprecision(3)<<X<<",";
    tss[i]<<std::setprecision(3)<<Y<<",";
    tss[i]<<std::setprecision(9)<<t<<",";
    tss[i]<<std::setprecision(3)<<L<<"\n";
}

void ResultWriter::writeHead(){
        std::stringstream tmpss, areasSs, colNames,rowName;
        std::string ts;
        std::string video;
        if(m_options.videoFile != "")
            video = m_options.videoFile;
        else
            video = "From_Capture_Device";

        rowName<<"\"W\",\"H\",\"X\",\"Y\"";
        for(unsigned int i=0 ;i<m_trackers->size(); i++){
            colNames<<"\""<<(*m_trackers)[i].getLabel()<<"\"";

            int m_WHXY_ROI[4];
            cv::Rect tmpRec = (*m_trackers)[i].getROI();
            m_WHXY_ROI[0] = (tmpRec.br()-tmpRec.tl()).x;
            m_WHXY_ROI[1] = (tmpRec.br()-tmpRec.tl()).y;
            m_WHXY_ROI[2] = tmpRec.tl().x;
            m_WHXY_ROI[3] = tmpRec.tl().y;

            areasSs<<m_WHXY_ROI[0]<<","<<m_WHXY_ROI[1]<<","<<m_WHXY_ROI[2]<<","<<m_WHXY_ROI[3];

            if(i != m_trackers->size() - 1){
                colNames<<",";
                areasSs<<",";
            }
        }


        std::string dateString(currentDateTime());

for(unsigned int i = 0; i < dateString.length(); i++){
    if(dateString[i] == ' ')
        dateString[i] = '_';
}

        tmpss<<"\'list("
                <<"Global="
                    <<"c("
                        <<"Date=\""<<dateString<<"\""
                        <<","
                        <<"Input=\""<<video<<"\""
                        <<","
                        <<"Number_of_Areas=\""<<m_trackers->size()<<"\""
                        <<","
                        <<"Sensitivity=\""<<m_options.motionSensitivity<<"\""
                        <<","
                        <<"Number_of_Training_Round=\""<<m_options.MOGTrainingRounds<<"\""
//                        <<","
//                        <<"Output_Diretory=\""<<m_options.outDir<<"\""
                        <<","
                        <<"Width=\""<<m_width<<"\""
                        <<","
                        <<"Height=\""<<m_height<<"\""
                    <<")"
                <<","
                <<"Areas="
                    <<"matrix("
                        <<"c("
                            <<areasSs.str()
                        <<")"
                        <<","
                        <<4
                        <<","
                        <<m_trackers->size()
                        <<","
                        <<"dimname=list("
                            <<"c("
                                <<rowName.str()
                            <<")"
                            <<","
                            <<"c("
                                <<colNames.str()
                            <<")"
                        <<")"
                    <<")"
             <<")\'"
           <<std::endl;


        std::string headData("Area,Territory,X,Y,time,L");
        tmpss>>ts;
        if(!toSTD){
            outputFile<<ts<<std::endl;//"\n";
            outputFile<<headData<<std::endl;//"\n";
        }
        else{
            std::cout<<ts<<std::endl;//"\n";
            outputFile<<headData<<std::endl;//"\n";
        }
//        }


}

const std::string ResultWriter::currentDateTime() {
    time_t     now = time(0);
    struct tm  tstruct;
    char       buf[80];
    tstruct = *localtime(&now);
    strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct);
    return buf;
}

