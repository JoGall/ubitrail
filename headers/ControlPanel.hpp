#ifndef CONTPAN_H
#define CONTPAN_H

#include <gtkmm.h>
#include "Processor.hpp"
#include "defines.hpp"
#include "optionStructure.hpp"

class ControlPanel : public Gtk::Window
{
    public:
        ControlPanel(Processor* pro, std::string *ROI,Options opts);
        virtual ~ControlPanel();
        void loop();
//        void closeMe();
        void setFinished(bool b){
            finished = b;
        }
    protected:
        bool on_tick();
        bool on_updateProgress();
        std::vector<std::string> sortAlpha(std::vector<std::string> sortThis);
        void updateLabels();

        void on_changeArea();
        void on_finished();

    private:
        Processor* m_pro;
        std::string *m_ROI;
        Gtk::HBox m_mainHB,m_hb_gobalSep,m_hb_areaSep,m_hb_pos,m_hb_territ,m_hb_dimPos,m_hb_ROIPick;
        Gtk::VBox m_panel,m_info_global,m_info_area,m_vb_ROIPicker;
        Gtk::ComboBoxText m_ROIPicker;
        Gtk::ProgressBar m_progressBar, m_progressTraining;
        Gtk::Label m_lab_ROIpicker,m_lab_gobal, m_lab_area,m_lab_dimentionArea;

        Gtk::HSeparator m_sep_gobal, m_sep_area,m_sep_ROIPick;
        Gtk::Label m_lab_dimPos, m_lab_pos,m_lab_territ,m_lab_dimPos_res, m_lab_pos_res,m_lab_territ_res,m_lab_processingSpeed;



        Options m_opts;
        bool finished;

};

#endif // CONTPAN_H
