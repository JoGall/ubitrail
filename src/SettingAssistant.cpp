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


#include "SettingAssistant.hpp"

SettingAssistant::SettingAssistant(Options* opts,VideoGrabber* videoGrab,bool *wasAborted):
m_pageVideoInput(this,opts,videoGrab),
m_pageAreaMaker(this,opts,videoGrab),
m_pageProcessPars(this,opts,videoGrab),
m_pageOutput(this,opts,videoGrab)
{
    //ctor
    m_wasAborted = wasAborted;

    set_border_width(12);
    set_default_size(400, 300);
    set_title(TITLE_OF_SETTING_ASSISTANT);

    append_page(m_pageVideoInput);
    set_page_title(*get_nth_page(0), m_pageVideoInput.get_title());

    append_page(m_pageAreaMaker);
    set_page_title(*get_nth_page(1), m_pageAreaMaker.get_title());

    append_page(m_pageProcessPars);
    set_page_title(*get_nth_page(2), m_pageProcessPars.get_title());
    set_page_complete(m_pageProcessPars);

    append_page(m_pageOutput);
    set_page_title(*get_nth_page(3), m_pageOutput.get_title());

    this->show_all_children();
    m_pageAreaMaker.hideModeBoxes();
    this->signal_cancel().connect( sigc::mem_fun(*this, &SettingAssistant::on_quit));

}

SettingAssistant::~SettingAssistant()
{
}

bool SettingAssistant::on_delete_event(GdkEventAny* event){
    this->on_quit();
    return true;
}
void SettingAssistant::on_quit(){
    *m_wasAborted = true;
    Gtk::Main::quit();
}
