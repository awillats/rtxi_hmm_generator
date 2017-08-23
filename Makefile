PLUGIN_NAME = hmm_generator

HEADERS = hmm_generator.h\
          BabyClass.h\
#          Time.h\


SOURCES = hmm_generator.cpp\
          moc_hmm_generator.cpp\
          BabyClass.cpp\
 #         Time.cpp\


LIBS = 

### Do not edit below this line ###

include $(shell rtxi_plugin_config --pkgdata-dir)/Makefile.plugin_compile
