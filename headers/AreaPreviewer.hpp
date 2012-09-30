#ifndef AREAPREVIEWER_H
#define AREAPREVIEWER_H

#include <gtkmm/drawingarea.h>
#include <gtkmm.h>
#include "opencv2/imgproc/imgproc.hpp"

#include "defines.hpp"

class AreaPreviewer : public Gtk::Window
{
    public:
        AreaPreviewer();
        virtual ~AreaPreviewer();
        void setMat(cv::Mat& input){
            double fxy = this->scaleRatio(input.cols,input.rows);
            cv::resize(input,mat,cv::Size(0,0),fxy,fxy,cv::INTER_LINEAR);
            cv::cvtColor(mat,mat, CV_BGR2RGB);
            if(quiet){
                quiet = false;
                Glib::signal_timeout().connect( sigc::mem_fun(*this, &AreaPreviewer::on_draw), DISPLAY_REFRESH_TIME );
            }
        }
    protected:
    bool on_draw();
        double scaleRatio(int imgW, int imgH);
        bool on_delete_event(GdkEventAny* event);
    private:
        Glib::RefPtr<Gtk::Window > m_window_preview;
        Gtk::VBox m_VBox_preview;
        Glib::RefPtr<Gdk:: Pixbuf > pixbuf;
        Gtk::DrawingArea m_DrawArea_preview;
        Gtk::EventBox imageContainer;
        Gtk::Button m_butt_saveImg;
        cv::Mat mat;
        bool quiet;


};

#endif // AREAPREVIEWER_H
