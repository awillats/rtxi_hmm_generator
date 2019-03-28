PLUGIN_NAME = hmm_generator

#gflag is to enable better debugging
CFLAGS = -g -Wall -Wextra

HEADERS = hmm_generator.h\
          ../../../module_help/hmmX/hmm/printFuns.hpp\
          ../../../module_help/hmmX/hmm/shuttleFuns.hpp\
          ../../../module_help/hmmX/hmm/hmm_vec.hpp
          
           #         include/hmmFuns.hpp\
           #         ../../../module_help/StAC_rtxi/hmmFuns.hpp\
           #         ../../../module_help/StAC_rtxi/hmm_tests/hmm_vec.hpp\
          #../../../module_help/StAC_rtxi/hmm_tests/hmm_fs.hpp\
#          include/BabyClass.h\


SOURCES = hmm_generator.cpp\
          moc_hmm_generator.cpp\
          ../../../module_help/hmmX/hmm/printFuns.cpp\
          ../../../module_help/hmmX/hmm/shuttleFuns.cpp\
          ../../../module_help/hmmX/hmm/hmm_vec.cpp
#          ../../../module_help/StAC_rtxi/hmmFuns.cpp\
#          ../../../module_help/StAC_rtxi/hmm_tests/hmm_vec.cpp\
#          ../../../module_help/StAC_rtxi/hmm_tests/hmm_fs.cpp\
#          include/hmmFuns.cpp\
#          include/BabyClass.cpp\




LIBS = 

### Do not edit below this line ###

include $(shell rtxi_plugin_config --pkgdata-dir)/Makefile.plugin_compile
