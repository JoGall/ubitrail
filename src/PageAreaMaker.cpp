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


#include "PageAreaMaker.hpp"


PageAreaMaker::PageAreaMaker(Gtk::Assistant* parent,Options* opts,VideoGrabber* videoGrab ):
m_title(PAGE_AREA_TITLE),
m_description(PAGE_AREA_DESCRIPTION),
m_modeNames ({"Default","Auto circles","From file","Manual"}),
m_modeLabelsText({PAGE_AREA_DEFAULT_MODE_DESCRIPTION,PAGE_AREA_AUTOCIRCLE_MODE_DESCRIPTION,PAGE_AREA_FILE_MODE_DESCRIPTION,PAGE_AREA_MANUAL_MODE_DESCRIPTION}),
m_isAvailable({true,true,true,false}),
m_generatePreview("Generate Preview"),
m_generatePreview2("Generate Preview"),
m_adju_nDish(2, 2, 1000, 1, 5.0, 0.0),
m_adju_nLine(0, 0, 1, 1, 1.0, 0.0),
m_spinButt_nDish(m_adju_nDish),
m_spinButt_nLine(m_adju_nLine),
m_lab_nDish("Number of circles: "),
m_lab_nLine("Number of lines per circle: "),
m_lab_mode("Area type: "),
m_butt_maskPicker("Choose an image")
{
    //ctor
    m_parent = parent;
    m_opts = opts;
    m_videoGrab = videoGrab;
    pack_start(m_description,true,true);
    pack_start(m_hb0,true,true);

    m_hb0.pack_start(m_hb_mode,true,true);
//    m_hb0.pack_start(m_vb_dummy_mode,false,false);
//    m_vb_dummy_mode.pack_start(m_hb_mode,true,true);
    m_hb_mode.pack_start(m_lab_mode,false,false);

    m_hb_mode.pack_start(m_vb_dummy_mode,false,false);
    m_vb_dummy_mode.pack_start(m_mode,true,false);

    for(unsigned int i = 0; i < N_MODES ;i++){
        m_mode.append_text(m_modeNames[i]);
        m_modeLabels[i].set_text(m_modeLabelsText[i]);
        m_modeLabels[i].set_line_wrap();
        m_modeVboxes[i].pack_start(m_modeLabels[i],true,false);
        m_hb0.pack_start(m_modeVboxes[i],true,false);
    }
//\\//\\m_modeVboxes[0].pack_start(m_modeLabels[i]);
    m_mode.set_active_text(m_modeNames[0]);

    m_modeVboxes[1].pack_start(m_hb_nDish,false,false);
    m_modeVboxes[1].pack_start(m_hb_nLine,false,false);
    m_modeVboxes[1].pack_start(m_generatePreview,false,false);

    m_hb_nDish.pack_start(m_lab_nDish,false,false);
    m_hb_nDish.pack_start(m_spinButt_nDish,false,false);
    m_hb_nLine.pack_start(m_lab_nLine,false,false);
    m_hb_nLine.pack_start(m_spinButt_nLine,false,false);


    m_modeVboxes[2].pack_start(m_butt_maskPicker,false,false);
    m_modeVboxes[2].pack_start(m_generatePreview2,false,false);

    m_mode.signal_changed().connect(sigc::mem_fun(*this,&PageAreaMaker::on_mode_changed) );
    m_spinButt_nDish.signal_value_changed().connect(sigc::mem_fun(*this,&PageAreaMaker::updateOpts) );
    m_generatePreview.signal_clicked().connect(sigc::mem_fun(*this,&PageAreaMaker::makePreview) );
    m_generatePreview2.signal_clicked().connect(sigc::mem_fun(*this,&PageAreaMaker::makePreview) );
    m_butt_maskPicker.signal_clicked().connect(sigc::mem_fun(*this,&PageAreaMaker::on_loadFile_clicked));
}

PageAreaMaker::~PageAreaMaker()
{
    //dtor
}

void PageAreaMaker::on_mode_changed(){
//    get_active_text ()
    this->hideModeBoxes();
    updateOpts();
    m_videoGrab->getFPS();
}
void PageAreaMaker::hideModeBoxes(){

    int active(m_mode.get_active_row_number());
    for(int i = 0; i < N_MODES ;i++){
        if(active == i)
             m_modeVboxes[i].show();
        else
            m_modeVboxes[i].hide();
    }
}
void PageAreaMaker::updateOpts(){
    Glib::ustring activeS = m_mode.get_active_text();
    if(activeS == "Default"){
        m_opts->nDishes = 0;
        m_opts->nLinePerDishes = 0;
        maskFileName = "";
    }
    else if(activeS == "Auto circles"){
        m_opts->nDishes = m_spinButt_nDish.get_value_as_int() ;
        m_opts->nLinePerDishes = m_spinButt_nLine.get_value_as_int() ;
        maskFileName = "";

    }
    else if(activeS == "From file"){
       m_opts->nDishes = 0;
       m_opts->nLinePerDishes = 0;
       m_opts->maskFile = maskFileName ;
    }

    int active(m_mode.get_active_row_number());
    bool complete(true);
    if(active < 0 || !m_isAvailable[active])
        complete =false;
    if( activeS == "From file" && m_opts->maskFile == "")
        complete =false;

    m_generatePreview2.set_sensitive(m_opts->maskFile != "");
    m_parent->set_page_complete(*this,complete);
}

void PageAreaMaker::makePreview(){
    m_parent->set_page_complete(*this,false);
    Gtk::Main::iteration();
    m_generatePreview.set_sensitive(false);
    this->updateOpts();
    cv::Mat mat;
    m_videoGrab->reset();
    Processor pro(*m_opts,*m_videoGrab,false,&mat);
    m_areaPreview.setMat( mat);
    m_generatePreview.set_sensitive(true);
    m_parent->set_page_complete(*this,true);
}

void PageAreaMaker::on_loadFile_clicked(){

    Gtk::FileChooserDialog dialog("Please choose a file",
    Gtk::FILE_CHOOSER_ACTION_OPEN);

    dialog.add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
    dialog.add_button(Gtk::Stock::OPEN, Gtk::RESPONSE_OK);
    Gtk::FileFilter     filter_any,  filter_png,  filter_jpg,  filter_tiff,  filter_bmp;

    filter_any.set_name("Any files");
    filter_png.set_name(".png images");
    filter_jpg.set_name(".jpg images");
    filter_tiff.set_name(".tiff images");
    filter_bmp.set_name(".bmp images");

    filter_any.add_pattern("*");

    filter_png.add_pattern("*.png");
    filter_png.add_pattern("*.PNG");

    filter_jpg.add_pattern("*.jpg");
    filter_jpg.add_pattern("*.jpeg");
    filter_jpg.add_pattern("*.JPEG");

    filter_tiff.add_pattern("*.tiff");
    filter_tiff.add_pattern("*.tiff");
    filter_tiff.add_pattern("*.TIF");

    filter_bmp.add_pattern("*.bmp");
    filter_bmp.add_pattern("*.BMP");



  dialog.add_filter(filter_png);
  dialog.add_filter(filter_tiff);
  dialog.add_filter(filter_jpg);
  dialog.add_filter(filter_bmp);
  dialog.add_filter(filter_any);

  int result = dialog.run();

  //Handle the response:
  switch(result)
  {
    case(Gtk::RESPONSE_OK):
        maskFileName = dialog.get_filename();
//        std::cout<<maskFileName<<std::endl;
      break;
    case(Gtk::RESPONSE_CANCEL):
      break;

    default:
      break;

  }
  this->updateOpts();
}
