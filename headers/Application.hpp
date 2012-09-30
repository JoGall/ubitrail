#ifndef APPLICATION_H
#define APPLICATION_H

#include <gtkmm.h>
#include "ControlPanel.hpp"
#include "DrawingArea.hpp"
#include "optionStructure.hpp"

class Application : public Gtk::Window
{
    public:
        Application(Processor* pro,Options opts);
        virtual ~Application();
    protected:
        void drawingLoop();
        bool on_delete_event(GdkEventAny* event);
    private:
        Processor* m_pro;
        std::string* ROI;
        ControlPanel* m_controlPanel;
        DrawingArea* m_drawingArea ;
        Glib::Thread *processingThread, *panelThread, *drawingLoopThread ;
        Gtk::EventBox imageContainer;

};

#endif // APPLICATION_H
