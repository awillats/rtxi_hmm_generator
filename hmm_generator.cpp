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
    "FR 1", "Firing rate",
    DefaultGUIModel::PARAMETER | DefaultGUIModel::DOUBLE,
  },
  {
    "FR 2", "Firing rate",
    DefaultGUIModel::PARAMETER | DefaultGUIModel::DOUBLE,
  },
  {
    "TR 1", "Transition rate",
    DefaultGUIModel::PARAMETER | DefaultGUIModel::DOUBLE,
  },
  {
    "TR 2", "Transition rate",
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
    
    //update(MODIFY);
    //update(MODIFY);
    printf("\n");
    //update(INIT);
    //refresh();
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
    restartHMM();
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
void HmmGenerator::restartHMM()
{
    spike_buff = genHMM(vFr,vTr,bufflen);
    std::vector<double>PI(2,.5);
    guess_hmm = HMMv(2,2,vFr,vTr,PI);
    decodeSpkBuffer();
    //printf("HMM restarted\n");
}


void
HmmGenerator::initParameters(void)
{
    spike=0;
    gstate=0;

    pfr1=1e-3;//0.003;//30;
    pfr2=20e-3;//0.02;//10;
    ptr1=4e-4;//0.03;//0.1;
    ptr2=4e-4;//0.03;//0.1;

    vFr = {pfr1, pfr2};
    vTr = {ptr1, ptr2};

    buffi = 0;
    bufflen = 5500;
    
    getSkip=1;
    
    restartHMM();
    stepHMM();
}


void
HmmGenerator::update(DefaultGUIModel::update_flags_t flag)
{
  double foo;
  
  switch (flag) {
    case INIT:
      printf("Init called\n");
      period = RT::System::getInstance()->getPeriod() * 1e-6; // ms
      period_ms = period*1e-3;
      setParameter("GUI label", 10.1);
      //setState("A State", some_state);
      setParameter("FR 1", pfr1/period_ms);
      setParameter("FR 2", pfr2/period_ms);
      setParameter("TR 1", ptr1/period_ms);
      setParameter("TR 2", ptr2/period_ms);
      break;

    case MODIFY:
      printf("Modify called\n");
      
      buffi=0;
        //period = RT::System::getInstance()->getPeriod() * 1e-6; // ms
        //period_ms = period*1e-3;

        //this skip business is probably unnecessary, but for some reason this seems to reset values to zero and prevent the INIT clause from taking effect
      if (getSkip==0)
      {
        printf("\n**ModMod\n");
        pfr1 = getParameter("FR 1").toDouble()*period_ms;
        pfr2 = getParameter("FR 2").toDouble()*period_ms;
        ptr1 = getParameter("TR 1").toDouble()*period_ms;
        ptr2 = getParameter("TR 2").toDouble()*period_ms;
      }
      else
      {
        getSkip=0;
        printf("\n**Mod skip\n");
      } 
      printf("F:%.3f, %.3f, T:%.3f, %.3f\n",pfr1,pfr2,ptr1,ptr2);
        //Set up the params for the new HMM
      vFr = {pfr1, pfr2};  
      vTr = {ptr1, ptr2};
      restartHMM();
      
      break;

    case UNPAUSE:
      break;

    case PAUSE:
      break;

    case PERIOD:
      period = RT::System::getInstance()->getPeriod() * 1e-6; // ms
     period_ms = period*1e-3;
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
