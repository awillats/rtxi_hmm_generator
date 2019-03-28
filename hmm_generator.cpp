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



/*
to do list:
* remove excess states/params
* remove getSkip machiner
* redirect inlcudes to new compu code


*/







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
    "Debug label", "Tooltip description",
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
    "Spike", "ooze", DefaultGUIModel::OUTPUT,
  },
  {
    "TrueState", "return of the ooze", DefaultGUIModel::OUTPUT,
  },
  {
    "GuessState", "snooze", DefaultGUIModel::OUTPUT,
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
   int tstate= state_buff[buffi];
   gstate= state_guess_buff[buffi]; //seems weird... only updating the guess when the buffer resets

   output(0)=spike;
   output(1)=tstate;
   output(2)=gstate;
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

    delete[] guessed;
}

void HmmGenerator::restartHMM()
{
    //spike_buff = genHMM(vFr,vTr,bufflen);

/*
    //fake guess data?
    HMM_Data HD = genHMM_Data(vFr,vTr,bufflen);
    spike_buff = HD.spikes;
    state_buff = HD.states;
*/

    std::vector<double>PI(2,.5);
    guess_hmm = HMMv(2,2,vFr,vTr,PI);

  //  guess_hmm.genSeq(bufflen);//new, probably extraneous
    std::std::vector<int> Zvec(bufflen,0);
    spike_buff = Zvec; //guess_hmm.spikes;
    state_buff = Zvec; //guess_hmm.states;

//removing this to test whether it's necessary for the memory leak!
   decodeSpkBuffer();
    //printf("HMM restarted\n");
}


void
HmmGenerator::initParameters(void)
{
    spike=0;
    gstate=0;

    pfr1=1e-3;
    pfr2=20e-3;
    ptr1=4e-4;
    ptr2=4e-4;

    vFr = {pfr1, pfr2};
    vTr = {ptr1, ptr2};

    buffi = 0;
    bufflen = 3000;

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
      setParameter("Debug label", 10.2);
      setParameter("FR 1", pfr1/period_ms);
      setParameter("FR 2", pfr2/period_ms);
      setParameter("TR 1", ptr1/period_ms);
      setParameter("TR 2", ptr2/period_ms);
      break;

    case MODIFY:
      printf("Modify called\n");

      buffi=0;

        //this skip business is probably unnecessary, but for some reason this seems to reset values to zero and prevent the INIT clause from taking effect
        //look at hmm_decoder for an example which doesn't require this...
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


void HmmGenerator::printStuff(void)
{
spike_buff[buffi]=99;//hopefully trigger hmm_decoder to print also
spike_buff[buffi+1]=99;
spike_buff[buffi+2]=99;
spike_buff[buffi+3]=99;
printf("\n\n generator;spike_buff=[");
  for (int i=0; i<bufflen; i++)
  {
       printf("%d,",spike_buff[i]);
  }

printf("];\nstate_guess=[");
  for (int i=0; i<bufflen; i++)
  {
       printf("%d,",state_guess_buff[i]);
  }
printf("];endgen;");
}



void
HmmGenerator::customizeGUI(void)
{
  QGridLayout* customlayout = DefaultGUIModel::getLayout();

  QGroupBox* button_group = new QGroupBox;

  QPushButton* abutton = new QPushButton("Print params");
  QHBoxLayout* button_layout = new QHBoxLayout;
  button_group->setLayout(button_layout);
  button_layout->addWidget(abutton);
  QObject::connect(abutton, SIGNAL(clicked()), this, SLOT(aBttn_event()));

  customlayout->addWidget(button_group, 0, 0);
  setLayout(customlayout);
}

// functions designated as Qt slots are implemented as regular C++ functions
void
HmmGenerator::aBttn_event(void)
{
	printStuff();
}
