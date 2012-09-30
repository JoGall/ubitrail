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
    along with Foobar.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "DrawingArea.hpp"
#define PANEL_WIDTH 100

DrawingArea::DrawingArea() {}

DrawingArea::DrawingArea(Processor* pro,std::string *ROI)
{
    //ctor
    m_pro = pro;
    m_ROI = ROI;
    Glib::RefPtr<Gdk::Screen> sc = Gdk::Screen::get_default();
    screen_width =(double)sc->get_width() - PANEL_WIDTH ;
    screen_height =(double)sc->get_height()*0.8;
    m_dispa.connect(sigc::mem_fun(*this, &DrawingArea::render));
}

DrawingArea::~DrawingArea()
{
}
//
void DrawingArea::loop()
{
    bool end(false);
    while(!end)
    {
        if(!m_pro->getIsFinished())
        {
            this->makeNewPixbuf();
            Glib::usleep(1000*30);//magic number
            m_dispa.emit();
            }
        else
            end = true;
    }
}
bool DrawingArea::test(){
    if(!m_pro->getIsFinished())
    {
        this->makeNewPixbuf();
        this->render();
        return true;
    }
    else
        return false;
}

void DrawingArea::makeNewPixbuf()
{
    cv::Mat clone, clone2;
    if(m_pro->getDeco(clone))
    {
        double fxy = this->scaleRatio(clone.cols,clone.rows);
        cv::resize(clone,clone2,cv::Size(0,0),fxy,fxy,cv::INTER_LINEAR);
        cv::cvtColor(clone2,mat, CV_BGR2RGB);
    }
}



void DrawingArea::render()
{
    if(!mat.empty())
    {
        pixbuf = Gdk::Pixbuf::create_from_data((guint8*)mat.data,Gdk::COLORSPACE_RGB,false,8,mat.cols,mat.rows,mat.step);
        pixbuf->render_to_drawable(get_window(), get_style()->get_black_gc(),0, 0, 0, 0, pixbuf->get_width(), pixbuf->get_height(),Gdk::RGB_DITHER_NONE, 0, 0);
        set_size_request(pixbuf->get_width(),pixbuf->get_height());
    }
}


double DrawingArea::scaleRatio(int imgW, int imgH)
{
    double ratio;
    if((screen_width/screen_height) > ((double)imgW/(double)imgH))
    {
        ratio = (double)screen_height/(double)imgH;
    }
    else
    {
        ratio = (double)screen_width/(double)imgW;
    }

    return ratio;
}
//
//
//
//
//#include <iostream>
//#include <queue>
//#include <glibmm/threads.h>
//#include <glibmm/random.h>
//#include <glibmm/timer.h>
//#include <glibmm/init.h>
//namespace
//{
//class MessageQueue : public sigc::trackable
//{
//public:
//    MessageQueue();
//    ~MessageQueue();
//    void producer();
//    void consumer();
//private:
//    Glib::Threads::Mutex mutex_;
//    Glib::Threads::Cond cond_push_;
//    Glib::Threads::Cond cond_pop_;
//    std::queue<int> queue_;
//};
//
//MessageQueue::MessageQueue()
//{}
//MessageQueue::~MessageQueue()
//{}
//
//void MessageQueue::producer()
//{
//    Glib::Rand rand (1234);
//    for(int i = 0; i < 200; ++i)
//    {
//        {
//            Glib::Threads::Mutex::Lock lock (mutex_);
//            while(queue_.size() >= 64)
//                cond_pop_.wait(mutex_);
//            queue_.push(i);
//            std::cout << '*';
//            std::cout.flush();
//            cond_push_.signal();
//        }
//        if(rand.get_bool())
//            continue;
//        Glib::usleep(rand.get_int_range(0, 100000));
//    }
//}
//void MessageQueue::consumer()
//{
//    Glib::Rand rand (4567);
//    for(;;)
//    {
//        {
//            Glib::Threads::Mutex::Lock lock (mutex_);
//            while(queue_.empty())
//                cond_push_.wait(mutex_);
//            const int i = queue_.front();
//            queue_.pop();
//            std::cout << "\x08 \x08";
//            std::cout.flush();
//            cond_pop_.signal();
//            if(i >= 199)
//                break;
//        }
//        if(rand.get_bool())
//            continue;
//        Glib::usleep(rand.get_int_range(10000, 200000));
//    }
//}
//}
//int main(int, char**)
//{
//    Glib::init();
//    MessageQueue queue;
//    Glib::Threads::Thread *const producer = Glib::Threads::Thread::create(sigc::mem_fun(queue, &MessageQueue::producer));
//    Glib::Threads::Thread *const consumer = Glib::Threads::Thread::create(sigc::mem_fun(queue, &MessageQueue::consumer));
//    producer->join();
//    consumer->join();
//    std::cout << std::endl;
//    return 0;
//}
//
//
//
//
//
