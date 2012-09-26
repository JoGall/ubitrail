#ifndef PAGEOUTPUT_H
#define PAGEOUTPUT_H

#include <gtkmm.h>
#include "guiText.hpp"
#include "VideoGrabber.hpp"
#include "optionStructure.hpp"


class PageOutput : public Gtk::VBox
{
    public:
        PageOutput(Gtk::Assistant* parent,Options* opts,VideoGrabber* videoGrab);
        virtual ~PageOutput();
        Glib::ustring get_title(){
            return m_title;
        }

    protected:
        void on_pickFolder();
        void on_update();
        void on_start();
    private:
        Gtk::Label m_description;
        Glib::ustring m_title;
        Gtk::Assistant* m_parent;
        Options* m_opts;
        VideoGrabber* m_videoGrab;
        Gtk::Button m_butt_pickFolder;
        Gtk::Button m_butt_start;
        Gtk::HBox m_hb1;
        Gtk::Label m_currentFolderName;
        Gtk::CheckButton m_checkB_hasTextFile;
        Gtk::CheckButton m_checkB_hasFirstPicture;
        Gtk::CheckButton m_checkB_hasIndivVideo;
        Gtk::CheckButton m_checkB_hasGlobal;

};

#endif // PAGEOUTPUT_H

