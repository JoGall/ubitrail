# The name of the program
EXEC_NAME := ubitrail
PROGRAM_NAME := '"Ubitrail"'
WEBSITE_URI := '"http://ubitrail.sourceforge.net"'

# The version
VERSION := 1

# The minimal version for OpenCV
MIN_OCV_VERSION := 2.3

# Directory structure of the project
SOURCE_DIR := src/
HEADERS_DIR := headers/
SAMPLES_DIR := samples/

# We define the sourcefile as all the cpp file in the source directory
SOURCES := $(shell ls $(SOURCE_DIR)*.cpp)
# And the object files are/will have the same name with ".o" sufix
OBJS := $(patsubst %.cpp, %.o, $(notdir $(SOURCES)))

# SAMPLES := $(shell ls $(SAMPLES_DIR)crop*.avi )

# RESULTS := $(patsubst %.avi, %.out, $(SAMPLES))



# The compiler
CXX := g++
# The flags:
# We tell where the headers are
CPPFLAGS = -I$(HEADERS_DIR)
# We define the version
CPPFLAGS += -DVERSION=$(VERSION)
CPPFLAGS += -DPROGRAM_NAME=$(PROGRAM_NAME)
CPPFLAGS += -DWEBSITE_URI=$(WEBSITE_URI)
# We define the include location of opencv
#TODO --atleast-version=$(MIN_OCV_VERSION) 
CPPFLAGS += $(shell pkg-config --cflags opencv)
CPPFLAGS +=  -fopenmp
CPPFLAGS += $(shell pkg-config --cflags gtkmm-2.4)
CPPFLAGS += $(shell pkg-config --cflags gthread-2.0)

LIBS = $(shell pkg-config  --libs opencv)
LIBS +=  -fopenmp
LIBS += $(shell pkg-config --libs gtkmm-2.4)
LIBS += $(shell pkg-config --libs gthread-2.0)
# We define the mode it should be one of those 3:
#mode = debug
mode = devel
#mode = release

# We define additional C flags according to the mode
ifeq ($(mode),release)
   CPPFLAGS += -O2 -Wall -DNDEBUG
else
   ifeq ($(mode),devel)
      CPPFLAGS +=  -O2 -g -Wall -pg
      LIBS += -pg
   else
      CPPFLAGS += -O0 -g -Wall -pg
      LIBS += -pg
   endif
endif


# We tell make to look for pre-requisites in the source directory
vpath %.cpp  src
#vpath %.avi  $(SAMPLES_DIR)


.PHONY: all clean test R linuxRelease
#I~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~	
all : $(EXEC_NAME)

# linking
$(EXEC_NAME) : $(OBJS)

	@echo "Building $@"
	$(CXX) $^ $(LIBS) -o $@

%.o : %.cpp
	@echo "Building $*.o"
	$(CXX) -c $(CPPFLAGS) $(SOURCE_DIR)/$*.cpp -o $*.o
	gcc -MM -I$(HEADERS_DIR) $(SOURCE_DIR)/$*.cpp > $*.d
	@mv -f $*.d $*.d.tmp
	@sed -e 's|.*:|$*.o:|' < $*.d.tmp > $*.d
	@sed -e 's/.*://' -e 's/\\$$//' < $*.d.tmp | fmt -1 | sed -e 's/^ *//' -e 's/$$/:/' >> $*.d
	@rm -f $*.d.tmp

# pull in dependency info for *existing* .o files
-include $(OBJS:.o=.d)


#I~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~	
install: $(EXEC_NAME) install/icons/ubitrail.svg ./install/$(EXEC_NAME).desktop
	sudo mkdir -p /usr/share/$(EXEC_NAME)/
	sudo cp ./install/icons/ubitrail.png /usr/share/$(EXEC_NAME)/
	sudo chmod 755 /usr/share/$(EXEC_NAME)/ubitrail.png
	sudo cp ./$(EXEC_NAME) /usr/bin/
	sudo chmod 755 /usr/bin/$(EXEC_NAME)
	sudo cp ./install/$(EXEC_NAME).desktop /usr/share/applications/
	sudo chmod 755 /usr/share/applications/$(EXEC_NAME).desktop

uninstall:
	sudo rm -r -f /usr/share/$(EXEC_NAME)/
	sudo rm -f /usr/bin/$(EXEC_NAME)
	sudo rm -f /usr/share/applications/$(EXEC_NAME).desktop

#I~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

clean:
	rm -f $(EXEC_NAME) *.o *.d  *.tar.gz *.out *.pdf

#I~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

LINUX_TARBALL := $(EXEC_NAME)-$(VERSION).tar.gz
	
linuxRelease: $(LINUX_TARBALL)
	@echo releasing to sourceforge:
	rsync -avP -e ssh $(LINUX_TARBALL) quentelery,ubitrail@frs.sourceforge.net:/home/frs/project/u/ub/ubitrail/linux/
	
$(LINUX_TARBALL): $(EXEC_NAME)
#	TMP = $(shell mktemp -d)
#	$(eval TMP=$(shell mktemp -d))
	@mkdir $(EXEC_NAME)-$(VERSION)/
	cp -r src install headers $(EXEC_NAME)-$(VERSION)/
	cp Makefile $(EXEC_NAME)-$(VERSION)/
	tar -pczf $@  $(EXEC_NAME)-$(VERSION)/
	rm -r $(EXEC_NAME)-$(VERSION)/
	echo "Uploading Windows setup"
	

#Build the R package~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

R_DIR = rubitrail/
R_SOURCES := $(shell ls $(R_DIR)/R/*.R )
R_TGZ := $(cat $(R_DIR)/DESCRIPTION |grep ^Package  | cut --fields=2 --delimiter=: |sed s/\ //)_$(cat $(R_DIR)/DESCRIPTION |grep ^Version  | cut --fields=2 --delimiter=: |sed s/\ //)".tar.gz"
vpath %.R  $(R_DIR)
PACKAGE_NAME := rubitrail


R : $(R_TGZ)
	R CMD Rd2pdf --force rubitrail
	@echo "installing rubitrail"
	R CMD INSTALL rubitrail*.tar.gz
	

$(R_TGZ) : $(R_SOURCES) $(ROXYGEN_FILE)
	@echo "Roxygeniting:"
	@echo "library(roxygen2); roxygenise(\"$(PACKAGE_NAME)\",roxygen.dir=$(\"R_DIR\"),copy.package=F,unlink.target=F)" | R --vanilla
#	@R --vanilla --slave < $(ROXYGEN_FILE)
	@echo "Building Package $(R_TGZ):"
	@R CMD build rubitrail
	
	
	
#I~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
releaseAll: $(R_TGZ) $(LINUX_TARBALL)
	make clean
	rsync -avP -e ssh * quentelery,ubitrail@frs.sourceforge.net:/home/frs/project/u/ub/ubitrail/all

