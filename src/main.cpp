#include <iostream>


//#include "../includes/Processor.hpp"
#include "OptionParser.hpp"
#include "VideoGrabber.hpp"
#include "Processor.hpp"
#include "SettingAssistant.hpp"
#include "Application.hpp"

#include "defines.hpp"


int main(int argc, char **argv){

    /* 1) We make the otion parser */
    OptionParser optPars;
    Options* opts;
    VideoGrabber*   ;
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
            videoGrab = new VideoGrabber(opts);
            pro = new Processor(*opts, *videoGrab,true);
            }

        else{
            opts = new Options();
            videoGrab = new VideoGrabber();
            SettingAssistant m_Assist(opts,videoGrab,assistantWasAborted);
            Gtk::Main::run(m_Assist);
        }

        if(!(*assistantWasAborted)){
            pro = new Processor(*opts, *videoGrab,true);
            Application application(pro,*opts);
            Gtk::Main::run(application);
        }
        else{
            hasNewPro = false;
            delete assistantWasAborted;
        }

    }
    delete opts;
    delete videoGrab;

    if(hasNewPro)
        delete pro;

    return 1;
}



//   Glib::thread_init();
//        Gtk::Main kit(argc, argv);

       /* opts = new Options();
        videoGrab = new VideoGrabber();


        SettingAssistant m_Assist(opts,videoGrab);

        Gtk::Main::run(m_Assist);
*/
/*FOR TESTING*/
//optPars = OptionParser(argc,argv);
//opts = new Options(optPars.GetOptions());
//videoGrab = new VideoGrabber(opts);
//videoGrab->reinit(opts);
//
//        Processor* pro;
//        pro = new Processor(*opts, *videoGrab);
//        Display display(pro);
//        Application application(pro,*opts);
//        Gtk::Main::run(application);
//
//        delete pro;
