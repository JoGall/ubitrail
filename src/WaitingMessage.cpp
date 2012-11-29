#include "WaitingMessage.hpp"
#include "defines.hpp"

WaitingMessage::WaitingMessage():
m_label_message("Please wait, parameters are setting up. ")
{

#if defined WIN32 || defined WIN64
this->set_icon_from_file("../ubitrail.ico");
//m_controlPanel->set_icon_from_file("../ubitrail.ico");
#endif
this->set_title(PROGRAM_NAME " - Setting up...");
this->add(m_label_message);

//            Glib::Thread messageThread;

//
this->show();
this->show_all_children();
}

WaitingMessage::~WaitingMessage()
{
    //dtor

}

void WaitingMessage::runMe(){
    Gtk::Main::run(*this);
    this->show();
    this->show_all_children();
}
