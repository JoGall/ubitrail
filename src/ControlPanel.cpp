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

#include "ControlPanel.hpp"

ControlPanel::ControlPanel(Processor* pro,std::string *ROI,Options opts):
//m_drawingArea(pro),
m_lab_ROIpicker("Area to display "),
m_lab_gobal("Global informations "),
m_lab_area("Area "),
//m_lab_dimentionArea("dim"),
//m_str_timeFromBegi("Time spent analysing (min): "),
//m_str_dimPos("Dimentions:"),
//m_str_pos("Position: "),
//m_lab_timeFromBegi(m_str_timeFromBegi),
m_lab_dimPos("Dimentions: "),
m_lab_pos("Position: "),
m_lab_territ("Territory: "),
m_opts(opts),
finished(false)
{
    m_ROI = ROI;
    m_pro = pro;

//
    this->set_title(PROGRAM_NAME " - Panel");
    this->set_default_size(300,700);
//    this->set_resizable(false);
this->set_deletable(false);
    this->set_border_width(5);

    m_panel.set_homogeneous(false);
    m_info_global.set_homogeneous(false);

    m_progressTraining.set_text("Model training...");


    this->add(m_panel);

    m_panel.pack_start(m_vb_ROIPicker,false,false);
    m_panel.pack_start(m_info_global,false,false);
    m_panel.pack_start(m_info_area,false,false);


    m_vb_ROIPicker.pack_start(m_hb_ROIPick,false,false);
    m_hb_ROIPick.pack_start(m_lab_ROIpicker,false,false);
    m_hb_ROIPick.pack_start(m_sep_ROIPick,true,true);

    m_vb_ROIPicker.pack_start(m_ROIPicker,false,false);


    m_info_global.pack_start(m_hb_gobalSep,false,true);

    m_hb_gobalSep.pack_start(m_lab_gobal,false,true);
    m_hb_gobalSep.pack_start(m_sep_gobal,true,true);

    m_info_global.pack_start(m_progressBar,false,false);


    m_info_area.pack_start(m_hb_areaSep,false,true);

    m_hb_areaSep.pack_start(m_lab_area,false,true);
    m_hb_areaSep.pack_start(m_sep_area,true,true);

//    m_info_area.pack_start(m_lab_area,false,false);
    m_info_area.pack_start(m_hb_dimPos,false,false);
    m_info_area.pack_start(m_progressTraining,false,false);
    m_info_area.pack_start(m_hb_pos,false,false);
    m_info_area.pack_start(m_hb_territ,false,false);

    m_hb_dimPos.pack_start(m_lab_dimPos,false,false);
    m_hb_dimPos.pack_start(m_lab_dimPos_res,false,false);

    m_hb_pos.pack_start(m_lab_pos,false,false);
    m_hb_pos.pack_start(m_lab_pos_res,false,false);

    m_hb_territ.pack_start(m_lab_territ,false,false);
    m_hb_territ.pack_start(m_lab_territ_res,false,false);


    std::vector<std::string> ROILabels = m_pro->getROILabels();
    ROILabels = sortAlpha(ROILabels);

    for(unsigned int i = 0; i < ROILabels.size(); i++){
        m_ROIPicker.prepend_text(ROILabels[i]);
    }
    m_ROIPicker.prepend_text("Whole frame");

    m_ROIPicker.signal_changed().connect(sigc::mem_fun(*this,&ControlPanel::on_changeArea) );

//
    Glib::signal_timeout().connect( sigc::mem_fun(*this, &ControlPanel::on_tick), DISPLAY_REFRESH_TIME );
    Glib::signal_timeout().connect( sigc::mem_fun(*this, &ControlPanel::on_updateProgress), PROGRESS_REFRESH_TIME );

    show_all_children();
    m_ROIPicker.set_active(0);

}

ControlPanel::~ControlPanel(){
}

bool ControlPanel::on_tick(){
    updateLabels();
    std::string tmpText = m_ROIPicker.get_active_text() ;

    if(tmpText == "Whole frame")
        tmpText = "";
//
    *m_ROI = tmpText;

    if(m_pro->getIsFinished()){
        this->on_finished();
        return false;
    }
    else{
        return true;

    }
}

std::vector<std::string> ControlPanel::sortAlpha(std::vector<std::string> sortThis){
    int swap;
    std::string temp;
    do{
        swap = 0;
        for (unsigned int count = 0; count < sortThis.size() - 1; count++) {
            if (sortThis.at(count) < sortThis.at(count + 1)){
                temp = sortThis.at(count);
                sortThis.at(count) = sortThis.at(count + 1);
                sortThis.at(count + 1) = temp;
                swap = 1;
            }
        }
    }  while (swap != 0);
    return sortThis;
}

bool ControlPanel::on_updateProgress(){

    double progress = m_pro->getProgress();
    if(progress >= 0)
        m_progressBar.set_fraction(progress);
    else
        m_progressBar.pulse(); //todo

    std::stringstream tss;
    long timeS = (long)m_pro->getTime() / 1000;

    if(finished){
        if(timeS < 60)
            tss<<"Completed in "<<timeS<<"s";
        else
            tss<<"Completed in "<<timeS/60<<"min";

        m_progressBar.set_text(tss.str());
        return false;
    }
    else{
        if(timeS < 60)
            tss<<(int)(progress*100)<<"\% in "<<timeS<<"s";
        else
            tss<<(int)(progress*100)<<"\% in "<<timeS/60<<"min";

        m_progressBar.set_text(tss.str());
        return true;
    }
}


void ControlPanel::updateLabels(){
    std::stringstream tss;
    if(m_ROIPicker.get_active_text() != "Whole frame"){

        float *tmpAreaInfo;

        tmpAreaInfo = m_pro->getTrackerXYTrainedTerrit();

        if(tmpAreaInfo[2] >= 1){
            tss<<std::setprecision (2)<<"("<<tmpAreaInfo[0]<<","<<tmpAreaInfo[1]<<")";
            m_lab_pos_res.set_text(tss.str());
            tss.str(std::string(""));
            tss<<tmpAreaInfo[3];
            m_lab_territ_res.set_text(tss.str());

            m_progressTraining.hide();
            m_hb_pos.show();
            m_hb_territ.show();
        }

        else{
            tss<<"(NA,NA)";
            m_lab_pos_res.set_text(tss.str());
            tss.str(std::string(""));
            tss<<"NA";
            m_lab_territ_res.set_text(tss.str());
            m_progressTraining.show();
            m_hb_pos.hide();
            m_hb_territ.hide();
            m_progressTraining.set_fraction(tmpAreaInfo[2]);
        }
    }

}


void ControlPanel::on_changeArea(){
    std::cout<<__FILE__<<":"<<__LINE__<<std::endl;
    if(m_ROIPicker.get_active_text() == "Whole frame"){
        m_info_area.hide();
        m_pro->setROIForGUI("");
    }

    else{
        std::stringstream tss;

        m_pro->setROIForGUI(m_ROIPicker.get_active_text());
//        m_pro->getTrackerWHXY_ROI(tmpROIInfo,m_ROIPicker.get_active_text());
        int *tmpROIInfo;
        tmpROIInfo = m_pro->getTrackerWHXY_ROI();

        tss<<tmpROIInfo[0]<<"x"<<tmpROIInfo[1]<<"+"<<tmpROIInfo[2]<<"+"<<tmpROIInfo[3];
        m_lab_dimPos_res.set_text(tss.str());
        tss.str(std::string(""));

        m_info_area.show();
    }
    std::cout<<__FILE__<<":"<<__LINE__<<std::endl;
}

void ControlPanel::on_finished(){
    this->finished = true;
    std::stringstream tss;
    m_progressBar.set_text(tss.str());
    tss <<"Processing complete.  <a href=\"file://"
        <<m_opts.outDir
        <<"\" "
        <<"title=\""
        <<m_opts.outDir
        <<"\">"
        <<"Open result folder</a>.\n"
        <<"Please visit our "
        <<"<a href=\""
        <<WEBSITE_URI
        <<"\" "
        <<"title=\""
        <<WEBSITE_URI
        <<"\">"
        << "website </a>"
        <<" if your need help for analysing data.";

    Gtk::MessageDialog dialog (tss.str(),true);
    dialog.run();
}
