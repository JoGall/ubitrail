#ifndef DRAWINGAREA_H
#define DRAWINGAREA_H

#include <gtkmm.h>
#include <gtkmm/drawingarea.h>
#include "Processor.hpp"

class DrawingArea : public Gtk::DrawingArea
{
    public:
        DrawingArea();
        DrawingArea(Processor* pro,std::string *ROI);
        virtual ~DrawingArea();
        void loop();
    protected:
        double scaleRatio(int imgW, int imgH);
        void render();
        void makeNewPixbuf();
        bool test();
    private:
        Glib::RefPtr<Gdk:: Pixbuf > pixbuf;
        Processor* m_pro;
        double screen_width;
        double screen_height;
        std::string* m_ROI;
        cv::Mat mat;
        Glib::Dispatcher m_dispa;
//        Glib::Threads::Mutex test;
};

#endif // DRAWINGAREA_H
