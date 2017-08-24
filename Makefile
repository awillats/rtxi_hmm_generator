PLUGIN_NAME = hmm_generator

HEADERS = hmm_generator.h\
          include/BabyClass.h\
          include/hmmFuns.hpp\
          ../../../module_help/StAC_rtxi/hmmFuns.hpp\
          ../../../module_help/StAC_rtxi/hmm_tests/hmm_fs.hpp



SOURCES = hmm_generator.cpp\
          moc_hmm_generator.cpp\
          include/BabyClass.cpp\
          ../../../module_help/StAC_rtxi/hmmFuns.cpp\
          ../../../module_help/StAC_rtxi/hmm_tests/hmm_fs.cpp\
#          include/hmmFuns.cpp\



LIBS = 

### Do not edit below this line ###

include $(shell rtxi_plugin_config --pkgdata-dir)/Makefile.plugin_compile
