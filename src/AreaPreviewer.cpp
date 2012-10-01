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

#include "AreaPreviewer.hpp"

AreaPreviewer::AreaPreviewer():
m_butt_saveImg("Save this image"),
quiet(true)
{
    //ctor
    this->set_title(PROGRAM_NAME " - Preview");
    this->set_default_size(480,620);
    this->set_resizable(false);
    this->add(m_VBox_preview);

    this->m_VBox_preview.pack_start(imageContainer);
    this->m_VBox_preview.pack_start(m_butt_saveImg,false,false);
    imageContainer.add(m_DrawArea_preview);
    this->show_all_children();


}

AreaPreviewer::~AreaPreviewer()
{
}

bool AreaPreviewer::on_draw(){
    if(mat.empty() || quiet){
        this->hide();
    }
    else{
        this->show();
        cv::Mat tmp;
        tmp =mat;
        pixbuf = Gdk::Pixbuf::create_from_data((guint8*)tmp.data,Gdk::COLORSPACE_RGB,false,8,tmp.cols,tmp.rows,tmp.step);
        pixbuf->render_to_drawable(m_DrawArea_preview.get_window(), m_DrawArea_preview.get_style()->get_black_gc(),0, 0, 0, 0, pixbuf->get_width(), pixbuf->get_height(),Gdk::RGB_DITHER_NONE, 0, 0);
        m_DrawArea_preview.set_size_request(mat.cols,mat.rows);
    }
    return !quiet;
}

bool AreaPreviewer::on_delete_event(GdkEventAny* event){
    quiet = true;
    this->hide();
    return false;
}

double AreaPreviewer::scaleRatio(int imgW, int imgH)
{

    Glib::RefPtr<Gdk::Screen> sc = Gdk::Screen::get_default();
    double screen_width =(double)sc->get_width();
    double screen_height =(double)sc->get_height()*0.8;


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
