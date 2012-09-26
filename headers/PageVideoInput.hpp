#ifndef PAGEVIDEOINPUT_H
#define PAGEVIDEOINPUT_H

#include <gtkmm.h>
#include "guiText.hpp"
#include "VideoGrabber.hpp"
#include "optionStructure.hpp"

class PageVideoInput : public Gtk::VBox
{
    public:
        PageVideoInput();
        PageVideoInput(Gtk::Assistant* parent,Options* opts,VideoGrabber* videoGrab);
        virtual ~PageVideoInput();
        Glib::ustring get_title(){
            return m_title;
        }
    protected:
        void on_loadFile_clicked();
//        void on_complete();
    private:
        Gtk::Label m_description;
        Gtk::Label m_currentFileName;
        Gtk::Button m_loadFile;
        Glib::ustring m_title;
        Glib::ustring fileLocation;
        Gtk::HBox m_hb1;
        int webCamIdx;
//        bool* m_isComplete;
        Gtk::Assistant* m_parent;
        Options* m_opts;
        VideoGrabber* m_videoGrab;
};

#endif // PAGEVIDEOINPUT_H
