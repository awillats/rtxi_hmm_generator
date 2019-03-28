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
 * This is a template header file for a user modules derived from
 * DefaultGUIModel with a custom GUI.
 */
//#include "Time.h"

#include "include/BabyClass.h" // todo deleteme
#include <default_gui_model.h>
#include <vector>
#include <iterator>
#include <tuple>

// todo: redirect these includes to new hmmfuns!
// they live in?
#include "../../../module_help/StAC_rtxi/hmmFuns.hpp"
#include "../../../module_help/StAC_rtxi/hmm_tests/hmm_vec.hpp"
//#include "../../../module_help/
#include "../../../module_help/hmmX/hmm/printFuns.hpp"
#include "../../../module_help/hmmX/hmm/shuttleFuns.hpp"
#include "../../../module_help/hmmX/hmm/hmm_vec.hpp"



class HmmGenerator : public DefaultGUIModel
{

  Q_OBJECT

public:
  HmmGenerator(void);
  virtual ~HmmGenerator(void);

  void execute(void);
  void createGUI(DefaultGUIModel::variable_t*, int);
  void customizeGUI(void);


protected:
  virtual void update(DefaultGUIModel::update_flags_t);

private:

  //should these be extern statements?

  int getSkip; //weird
  double period;
  double period_ms;
  double spike;
  int gstate;

  int bufflen;
  int buffi;
//s  double rep_count;
  std::vector<int> spike_buff;
  std::vector<int> state_buff;
  std::vector<int> state_guess_buff;

//--- HMM guess params
  double pfr1;
  double pfr2;
  double ptr1;
  double ptr2;

  std::vector<double> vFr;
  std::vector<double> vTr;

  //HMM guess_hmm;
  HMMv guess_hmm = HMMv();
  //NB: this method declaration needs to go somewhere else!!

  void initParameters();
  void stepHMM();
  void decodeSpkBuffer();
  int* decodeHMM(HMMv);
  void restartHMM();
void printStuff();

private slots:
  // these are custom functions that can also be connected to events
  // through the Qt API. they must be implemented in plugin_template.cpp

  void aBttn_event(void);
  void bBttn_event(void);
};
