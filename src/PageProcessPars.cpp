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

#include "guiToolTips.hpp"
#include "PageProcessPars.hpp"

PageProcessPars::PageProcessPars(Gtk::Assistant* parent,Options* opts,VideoGrabber* videoGrab ):
m_title(PAGE_PROCPARS_TITLE),
m_description(PAGE_PROCPARS_DESCRIPTION),
m_adju_sensit(opts->motionSensitivity, 0, 255, 1, 5.0, 0.0),
m_adju_training(opts->MOGTrainingRounds, 1, 100000, 1, 100.0, 0.0),
m_spinButt_sensit(m_adju_sensit),
m_spinButt_training(m_adju_training),
m_lab_sensit("Sensitivity: "),
m_lab_training("Number of frames used for training: ")
{
    //ctor
    m_parent = parent;
    m_opts = opts;
    m_videoGrab = videoGrab;

    pack_start(m_description);
    pack_start(m_hb_sensit,false,false);
    pack_start(m_hb_training,false,false);

    m_hb_sensit.pack_start(m_lab_sensit,false,false);
    m_hb_sensit.pack_start(m_spinButt_sensit,false,false);
    m_hb_training.pack_start(m_lab_training,false,false);
    m_hb_training.pack_start(m_spinButt_training,false,false);

    m_spinButt_sensit.signal_changed().connect(sigc::mem_fun(*this,&PageProcessPars::updateOpts) );
    m_spinButt_training.signal_changed().connect(sigc::mem_fun(*this,&PageProcessPars::updateOpts) );

    m_hb_sensit.set_tooltip_text(TOOLTIP_PROCPARS_SENSITIVITY);
    m_hb_training.set_tooltip_text(TOOLTIP_PROCPARS_TRAINING);


}

PageProcessPars::~PageProcessPars()
{
    //dtor
}


void PageProcessPars::updateOpts(){
        m_opts->motionSensitivity = m_spinButt_sensit.get_value_as_int() ;
        m_opts->MOGTrainingRounds = m_spinButt_training.get_value_as_int();
}

//#include "PageProcessPars.hpp"
//
//PageProcessPars::PageProcessPars(Gtk::Assistant* parent,Options* opts,VideoGrabber* videoGrab ):
//m_title(PAGE_PROCPARS_TITLE),
//m_description(PAGE_PROCPARS_DESCRIPTION)
//{
//}
//
//PageProcessPars::~PageProcessPars()
//{
//}
