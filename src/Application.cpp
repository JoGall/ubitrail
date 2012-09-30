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

#include "Application.hpp"

Application::Application(Processor* pro,Options opts){

    this->set_title(PROGRAM_NAME " - Display");
    this->set_border_width(5);
    this->add(imageContainer);
    this->set_resizable(false);
//    this->set_deletable(false);


    m_pro = pro;

    ROI = new std::string("");
    m_drawingArea = new DrawingArea(pro,ROI);
    m_controlPanel = new ControlPanel(pro,ROI,opts);
    processingThread = Glib::Thread::create(sigc::mem_fun(m_pro, &Processor::track), true);
    drawingLoopThread = Glib::Thread::create(sigc::mem_fun(m_drawingArea, &DrawingArea::loop), true);


    imageContainer.add(*m_drawingArea);

    m_drawingArea->show();
    m_controlPanel->show();
    imageContainer.show();
    this->show();
}

Application::~Application(){
    m_pro->setIsFinished(true);
    processingThread->join();
    drawingLoopThread->join();
    delete m_drawingArea;
    delete m_controlPanel;
    delete ROI;
}

bool Application::on_delete_event(GdkEventAny* event){
    std::stringstream tss;
    tss<<"Thank you for using "<<PROGRAM_NAME<<". Do you really want to quit?"<<std::endl
        <<"NOTE: If some data had already been saved, interupting the program in this manner will NOT delete them.";
    Gtk::MessageDialog dialog(*this, "Quit?!",false, Gtk::MESSAGE_QUESTION,Gtk::BUTTONS_OK_CANCEL);
    dialog.set_secondary_text(tss.str());
    int result = dialog.run();
    if(result==GTK_RESPONSE_OK)
        return false;
    else{
        return true;
    }
}
