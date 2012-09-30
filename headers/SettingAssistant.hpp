//todel
#include "defines.hpp"
#ifndef SETTINGASSISTANT_H
#define SETTINGASSISTANT_H


#include "PageVideoInput.hpp"
#include "PageAreaMaker.hpp"
#include "PageProcessPars.hpp"
#include "PageOutput.hpp"

#include "VideoGrabber.hpp"
#include "optionStructure.hpp"
#include <gtkmm.h>



class SettingAssistant : public Gtk::Assistant
{
    public:
        SettingAssistant(Options* opts,VideoGrabber* videoGrab, bool *wasAborted);
        virtual ~SettingAssistant();
    protected:
        bool on_delete_event(GdkEventAny* event);
        void on_quit();
    private:
        PageVideoInput m_pageVideoInput;
        PageAreaMaker m_pageAreaMaker;
        PageProcessPars m_pageProcessPars;
        PageOutput m_pageOutput;
        bool* m_wasAborted;
};

#endif // SETTINGASSISTANT_H
