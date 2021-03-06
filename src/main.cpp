/*
    Copyright Quentin Geissmann 2012

    This file is part of Ubitrail

    Ubitrail is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Ubitrail is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Ubitrail.  If not, see <http://www.gnu.org/licenses/>.
*/


#include "OptionParser.hpp"
#include "VideoGrabber.hpp"
#include "Processor.hpp"
#include "SettingAssistant.hpp"
#include "Application.hpp"
#include "WaitingMessage.hpp"

int main(int argc, char **argv){

    /* 1) We make the otion parser */
    OptionParser optPars;
    Options* opts;
    VideoGrabber* videoGrab;
    Processor* pro;
    optPars = OptionParser(argc,argv);
    bool hasNewPro(true);

    if(!optPars.checkOptions())
        return 0;

    opts = new Options(optPars.GetOptions());

    if(!opts->hasGUI && !opts->hasAssistant){
        videoGrab = new VideoGrabber(opts);
        pro = new Processor(*opts, *videoGrab);
        pro->track();
    }

    else {
        Glib::thread_init();
        Gtk::Main kit(argc, argv);
        bool *assistantWasAborted;
        assistantWasAborted = new bool(false);


        if(!opts->hasAssistant){
            WaitingMessage wm;
            videoGrab = new VideoGrabber(opts);
            Glib::Thread *messageThread;
            messageThread = Glib::Thread::create(sigc::mem_fun(wm, &WaitingMessage::runMe), true);
            pro = new Processor(*opts, *videoGrab,true);
            //messageThread->join();
            Gtk::Main::quit();

            }

        else{
            opts = new Options();
            opts->hasAssistant = true;
            videoGrab = new VideoGrabber();
            SettingAssistant m_Assist(opts,videoGrab,assistantWasAborted);
            Gtk::Main::run(m_Assist);
        }

        if(!(*assistantWasAborted)){
            if(opts->hasAssistant){
                WaitingMessage wm;
                Glib::Thread *messageThread;
                messageThread = Glib::Thread::create(sigc::mem_fun(wm, &WaitingMessage::runMe), true);
                pro = new Processor(*opts, *videoGrab,true);
                Gtk::Main::quit();
                //messageThread->join();
            }
            Application application(pro,*opts);
            Gtk::Main::run(application);
        }
        else{
            hasNewPro = false;
            delete assistantWasAborted;
        }
    }

//    }
    delete opts;
    delete videoGrab;

    if(hasNewPro)
        delete pro;

    return 1;
}

