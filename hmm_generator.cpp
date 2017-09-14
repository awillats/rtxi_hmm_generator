/*
 * Copyright (C) 2011 Georgia Institute of Technology, University of Utah,
 * Weill Cornell Medical College
 *
 * This program is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/*
 * This is a template implementation file for a user module derived from
 * DefaultGUIModel with a custom GUI.
 */

//#include <string.h>


#include "hmm_generator.h"
#include <iostream>
#include <main_window.h>
//#include "../../../module_help/StAC_rtxi/hmm_tests/hmm_fs.hpp"

extern "C" Plugin::Object*
createRTXIPlugin(void)
{
  return new HmmGenerator();
}

static DefaultGUIModel::variable_t vars[] = {
  {
    "GUI label", "Tooltip description",
    DefaultGUIModel::PARAMETER | DefaultGUIModel::DOUBLE,
  },
  {
    "A State", "Tooltip description", DefaultGUIModel::STATE,
  },
  {
    "Spike", "ooze", DefaultGUIModel::OUTPUT,
  },
  {
    "GuessState", "return of the ooze", DefaultGUIModel::OUTPUT,
  },


};

static size_t num_vars = sizeof(vars) / sizeof(DefaultGUIModel::variable_t);

HmmGenerator::HmmGenerator(void)
  : DefaultGUIModel("HmmGenerator with Custom GUI", ::vars, ::num_vars)
{
  setWhatsThis("<p><b>HmmGenerator:</b><br>QWhatsThis description.</p>");
  DefaultGUIModel::createGUI(vars,
                             num_vars); // this is required to create the GUI
  customizeGUI();
  initParameters();
  update(INIT); // this is optional, you may place initialization code directly
                // into the constructor
  refresh();    // this is required to update the GUI with parameter and state
                // values
  QTimer::singleShot(0, this, SLOT(resizeMe()));
}

HmmGenerator::~HmmGenerator(void)
{
}


void
HmmGenerator::stepHMM(void)
{
  buffi++;
  if (buffi>=bufflen)
  {
    //reset the index
    buffi=0;
    //generate a new chunk of observations!
    spike_buff = genHMM(vFr,vTr,bufflen);
    std::vector<double>PI(2,.5);
    guess_hmm = HMMv(2,2,vFr,vTr,PI);
    decodeSpkBuffer();
  }

   spike = spike_buff[buffi];
   gstate= state_guess_buff[buffi];

   output(0)=spike;
   output(1)=gstate;
}

void
HmmGenerator::execute(void)
{
   stepHMM();
  return;
}




int* HmmGenerator::decodeHMM(HMMv guess_hmm_)
{
  int* guessed = viterbi(guess_hmm_, spike_buff, bufflen);
  return guessed;  
}
void HmmGenerator::decodeSpkBuffer()
{
    int* guessed = decodeHMM(guess_hmm);
    //NB: no idea why this temporary vector is necessary. should be able to replace this with one line...
    std::vector<int> temp_vec(guessed,guessed+bufflen);
    state_guess_buff = temp_vec;
}


void
HmmGenerator::initParameters(void)
{
    spike=0;
    gstate=0;

    vFr = {0.003, 0.02};
    vTr = {0.03, 0.03};
    std::vector<double>PI(2,.5);

    buffi = 0;
    bufflen = 5500;

    spike_buff = genHMM(vFr,vTr,bufflen);
    guess_hmm = HMMv(2,2,vFr,vTr,PI);
    decodeSpkBuffer();
    stepHMM();
}

void
HmmGenerator::update(DefaultGUIModel::update_flags_t flag)
{
  switch (flag) {
    case INIT:
      period = RT::System::getInstance()->getPeriod() * 1e-6; // ms
      //setParameter("GUI label", some_parameter);
      //setState("A State", some_state);


      break;

    case MODIFY:
      //some_parameter = getParameter("GUI label").toDouble();
      buffi=0;
      break;

    case UNPAUSE:
      break;

    case PAUSE:
      break;

    case PERIOD:
      period = RT::System::getInstance()->getPeriod() * 1e-6; // ms
      break;

    default:
      break;
  }
}

void
HmmGenerator::customizeGUI(void)
{
  QGridLayout* customlayout = DefaultGUIModel::getLayout();

  QGroupBox* button_group = new QGroupBox;

  QPushButton* abutton = new QPushButton("Button A");
  QPushButton* bbutton = new QPushButton("Button B");
  QHBoxLayout* button_layout = new QHBoxLayout;
  button_group->setLayout(button_layout);
  button_layout->addWidget(abutton);
  button_layout->addWidget(bbutton);
  QObject::connect(abutton, SIGNAL(clicked()), this, SLOT(aBttn_event()));
  QObject::connect(bbutton, SIGNAL(clicked()), this, SLOT(bBttn_event()));

  customlayout->addWidget(button_group, 0, 0);
  setLayout(customlayout);
}

// functions designated as Qt slots are implemented as regular C++ functions
void
HmmGenerator::aBttn_event(void)
{
}

void
HmmGenerator::bBttn_event(void)
{
}
