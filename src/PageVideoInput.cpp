#include "PageVideoInput.hpp"

//todel
#include <iostream>

PageVideoInput::PageVideoInput(){
}


PageVideoInput::PageVideoInput(Gtk::Assistant* parent,Options* opts,VideoGrabber* videoGrab ):
m_title(PAGE_VIDEO_INPUT_TITLE),
m_description(PAGE_VIDEO_INPUT_DESCRIPTION),
m_currentFileName(PAGE_VIDEO_INPUT_DESCRIPTION_NO_FILE),
m_loadFile(PAGE_VIDEO_INPUT_LOAD_FILE_BUTTON),
fileLocation(""),
webCamIdx(-1)
{
    m_opts = opts;
    m_videoGrab = videoGrab;
    m_parent = parent;

    pack_start(m_description);
    pack_start(m_hb1,true,false);

    m_hb1.pack_start(m_loadFile,true,true);
    m_hb1.pack_start(m_currentFileName,true,true);
    m_currentFileName.set_line_wrap();

    m_loadFile.signal_clicked().connect(sigc::mem_fun(*this,&PageVideoInput::on_loadFile_clicked) );
    show_all_children();
}

PageVideoInput::~PageVideoInput()
{
}

void PageVideoInput::on_loadFile_clicked(){

Gtk::FileChooserDialog dialog("Please choose a file",
          Gtk::FILE_CHOOSER_ACTION_OPEN);
  dialog.add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
  dialog.add_button(Gtk::Stock::OPEN, Gtk::RESPONSE_OK);
  Gtk::FileFilter filter_any, filter_avi;
  filter_any.set_name("Any files");
  filter_avi.set_name(".avi videos");
  filter_any.add_pattern("*");
  filter_avi.add_pattern("*.avi");
  dialog.add_filter(filter_avi);
  dialog.add_filter(filter_any);

  int result = dialog.run();

  //Handle the response:
  switch(result)
  {
    case(Gtk::RESPONSE_OK):
      fileLocation = dialog.get_filename();
      m_currentFileName.set_text(fileLocation);
      m_opts->videoFile = fileLocation;
        if(m_videoGrab->reinit(m_opts))
            m_parent->set_page_complete(*this);
        else
//            ErrorPrinter(VIDEO_WONT_INIT);
      break;

    case(Gtk::RESPONSE_CANCEL):
      break;

    default:
      break;

  }
}
