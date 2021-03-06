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


#include "PageOutput.hpp"

PageOutput::PageOutput(Gtk::Assistant* parent,Options* opts,VideoGrabber* videoGrab ):
m_title(PAGE_OUTPUT_TITLE),
m_description(PAGE_OUTPUT_DESCRIPTION),
m_butt_pickFolder(" Choose a folder"),
m_butt_start("Start Processing"),
m_currentFolderName(PAGE_OUTPUT_DESCRIPTION_NO_FILE),
m_checkB_hasTextFile("Record positions over time"),
m_checkB_hasFirstPicture("Save first frame"),
m_checkB_hasIndivVideo("Record marked global video"),
m_checkB_hasGlobal("Record a marked video for each area")
{
    m_parent = parent;
    m_opts = opts;
    m_videoGrab = videoGrab;


    m_checkB_hasTextFile.set_active();
    m_currentFolderName.set_line_wrap();
    m_butt_start.set_sensitive(false);

    pack_start(m_description,false,false);
    pack_start(m_hb1,false,false);
    pack_start(m_checkB_hasTextFile,false,false);
    pack_start(m_checkB_hasFirstPicture,false,false);
    pack_start(m_checkB_hasGlobal,false,false);
    pack_start(m_checkB_hasIndivVideo,false,false);
    pack_start(m_butt_start,false,false);

    m_hb1.pack_start(m_butt_pickFolder,false,false);
    m_hb1.pack_start(m_currentFolderName,false,false);


    m_butt_pickFolder.signal_clicked().connect(sigc::mem_fun(*this,&PageOutput::on_pickFolder) );

    m_checkB_hasFirstPicture.signal_clicked().connect(sigc::mem_fun(*this,&PageOutput::on_update) );
    m_checkB_hasGlobal.signal_clicked().connect(sigc::mem_fun(*this,&PageOutput::on_update) );
    m_checkB_hasIndivVideo.signal_clicked().connect(sigc::mem_fun(*this,&PageOutput::on_update) );
    m_checkB_hasTextFile.signal_clicked().connect(sigc::mem_fun(*this,&PageOutput::on_update) );
    m_butt_start.signal_clicked().connect(sigc::mem_fun(*this,&PageOutput::on_update) );
    m_butt_start.signal_clicked().connect(sigc::mem_fun(*this,&PageOutput::on_start) );

}


PageOutput::~PageOutput()
{
    //dtor
}
void PageOutput::on_start(){
    m_parent->hide();
}
void PageOutput::on_pickFolder(){
    Gtk::FileChooserDialog dialog("Please choose a folder",Gtk::FILE_CHOOSER_ACTION_SELECT_FOLDER);
    dialog.set_create_folders (true);
    dialog.add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
    dialog.add_button("Select", Gtk::RESPONSE_OK);

    int result = dialog.run();
    std::stringstream tss;
    //Handle the response:
    switch(result){
        case(Gtk::RESPONSE_OK):
            m_opts->outDir = dialog.get_filename();
            tss<<" Writing results in \""<<m_opts->outDir<<"\".";
            m_currentFolderName.set_text(tss.str());
            this->on_update();
            m_butt_start.set_sensitive(true);
            break;
        case(Gtk::RESPONSE_CANCEL):
            break;
        default:
            break;
    }
}

void PageOutput::on_update(){

        m_opts->resultFile = m_checkB_hasTextFile.get_active();
        m_opts->allFrameOutput = m_checkB_hasIndivVideo.get_active();
        m_opts->videosOutput = m_checkB_hasGlobal.get_active();
        m_opts->writeFirstPicture = m_checkB_hasFirstPicture.get_active();

        if(!m_opts->resultFile && !m_opts->allFrameOutput && !m_opts->allFrameOutput && !m_opts->writeFirstPicture)
            m_butt_start.set_sensitive(true);
        else if(m_opts->outDir == "")
            m_butt_start.set_sensitive(false);
}












