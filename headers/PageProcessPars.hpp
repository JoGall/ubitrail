#ifndef PAGEPROCESSPARS_H
#define PAGEPROCESSPARS_H

#include <gtkmm.h>
#include "guiText.hpp"
#include "VideoGrabber.hpp"
#include "optionStructure.hpp"

class PageProcessPars: public Gtk::VBox
{
    public:
        PageProcessPars(Gtk::Assistant* parent,Options* opts,VideoGrabber* videoGrab );
        virtual ~PageProcessPars();
        Glib::ustring get_title(){
            return m_title;
        }
    protected:
        void updateOpts();
    private:
        Glib::ustring m_title;
        Gtk::Label m_description;

        Gtk::Assistant* m_parent;

        Gtk::Adjustment m_adju_sensit, m_adju_training;
        Gtk::SpinButton m_spinButt_sensit, m_spinButt_training;
        Gtk::HBox m_hb_sensit, m_hb_training;
        Gtk::Label m_lab_sensit, m_lab_training;
        Options* m_opts;
        VideoGrabber* m_videoGrab;

};

#endif // PAGEPROCESSPARS_H
//
//
//#ifndef PAGEPROCESSPARS_H
//#define PAGEPROCESSPARS_H
//
//#include <gtkmm.h>
//#include "guiText.hpp"
//#include "VideoGrabber.hpp"
//#include "optionStructure.hpp"
//
//class PageProcessPars: public Gtk::VBox
//{
//    public:
//        PageProcessPars(Gtk::Assistant* parent,Options* opts,VideoGrabber* videoGrab );
//        virtual ~PageProcessPars();
//        Glib::ustring get_title(){
//            return m_title;
//        }
//    protected:
//    private:
//        Gtk::Label m_description;
//        Glib::ustring m_title;
//        Gtk::Assistant* m_parent;
//};
//
//#endif // PAGEPROCESSPARS_H
