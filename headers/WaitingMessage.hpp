#ifndef WAITINGMESSAGE_H
#define WAITINGMESSAGE_H
#include <gtkmm.h>

class WaitingMessage : public Gtk::Window
{
    public:
        WaitingMessage();
        virtual ~WaitingMessage();
        void runMe();

    protected:
    private:
    Gtk::Label m_label_message;
};

#endif // WAITINGMESSAGE_H
