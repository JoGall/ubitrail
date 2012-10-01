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



void DrawingArea::makeNewPixbuf()
{
    cv::Mat clone;
    if(m_pro->getDeco(clone))
    {
        double fxy = this->scaleRatio(clone.cols,clone.rows);
        cv::resize(clone,clone,cv::Size(0,0),fxy,fxy,cv::INTER_LINEAR);
        {
            Glib::Mutex::Lock lock (m_mutexRender);
            cv::cvtColor(clone,mat, CV_BGR2RGB);
        }
    }
}



void DrawingArea::render()
{
    if(!mat.empty())
    {
        Glib::Mutex::Lock lock (m_mutexRender);
        pixbuf = Gdk::Pixbuf::create_from_data((guint8*)mat.data,Gdk::COLORSPACE_RGB,false,8,mat.cols,mat.rows,mat.step);
        pixbuf->render_to_drawable(get_window(), get_style()->get_black_gc(),0, 0, 0, 0, pixbuf->get_width(), pixbuf->get_height(),Gdk::RGB_DITHER_NONE, 0, 0);
        set_size_request(mat.cols,mat.rows);

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
