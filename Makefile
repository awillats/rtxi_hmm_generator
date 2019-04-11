PLUGIN_NAME = hmm_generator

HMM_DIR = /home/stanleyrtxi/CodeRepository/hmm

#gflag is to enable better debugging
CFLAGS = -g -Wall -Wextra

HEADERS = hmm_generator.h\
          $(HMM_DIR)/printFuns.hpp\
          $(HMM_DIR)/shuttleFuns.hpp\
          $(HMM_DIR)/hmm_vec.hpp


SOURCES = hmm_generator.cpp\
          moc_hmm_generator.cpp\
          $(HMM_DIR)/printFuns.cpp\
          $(HMM_DIR)/shuttleFuns.cpp\
          $(HMM_DIR)/hmm_vec.cpp

LIBS =

CXXFLAGS := $(CXXFLAGS) -I$(HMM_DIR)

### Do not edit below this line ###

include $(shell rtxi_plugin_config --pkgdata-dir)/Makefile.plugin_compile
