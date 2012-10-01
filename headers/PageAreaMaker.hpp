#ifndef PAGEAREAMAKER_H
#define PAGEAREAMAKER_H


#define N_MODES 4


#include <gtkmm.h>
#include <gtkmm/drawingarea.h>
#include "guiText.hpp"
#include "Processor.hpp"
#include "VideoGrabber.hpp"
#include "optionStructure.hpp"
#include "AreaPreviewer.hpp"


class PageAreaMaker: public Gtk::VBox
{
    public:
        PageAreaMaker(Gtk::Assistant* parent,Options* opts,VideoGrabber* videoGrab);
        virtual ~PageAreaMaker();
        Glib::ustring get_title(){
            return m_title;
        }
        void hideModeBoxes();
    protected:
        void on_mode_changed();
        void updateOpts();
        void makePreview();
        void on_loadFile_clicked();
    private:
        Glib::ustring m_title;
        Gtk::Label m_description;
        Gtk::Assistant* m_parent;
        Gtk::ComboBoxText m_mode;
        Gtk::HBox m_hb0;

        Glib::ustring m_modeNames[N_MODES];
        Gtk::Label m_modeLabels[N_MODES];
        Glib::ustring m_modeLabelsText[N_MODES];
        Gtk::VBox m_modeVboxes[N_MODES], m_vb_dummy_mode;
        bool m_isAvailable[N_MODES];
        Gtk::Button m_generatePreview, m_generatePreview2;

        Gtk::Adjustment m_adju_nDish, m_adju_nLine;
        Gtk::SpinButton m_spinButt_nDish, m_spinButt_nLine;
        Gtk::HBox m_hb_nDish, m_hb_nLine,m_hb_mode;
        Gtk::Label m_lab_nDish, m_lab_nLine,m_lab_mode;

//        Gtk::Button generatePreview;
        Options* m_opts;
        VideoGrabber* m_videoGrab;
//        Glib::RefPtr<Gdk::Window> preview
        AreaPreviewer m_areaPreview;
        std::string maskFileName;
        Gtk::Button m_butt_maskPicker;
//        Glib::RefPtr<Gdk:: Pixbuf > m_pixbuf;

//        Z
};


#endif // PAGEAREAMAKER_H
