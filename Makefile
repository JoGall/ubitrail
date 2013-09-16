# The name of the program
EXEC_NAME := ubitrail
PROGRAM_NAME := '"Ubitrail"'
WEBSITE_URI := '"http://ubitrail.sourceforge.net"'

# The version
VERSION := 1.3

# The minimal version for OpenCV
MIN_OCV_VERSION := 2.4

# Directory structure of the project
SOURCE_DIR := src/
HEADERS_DIR := headers/
PAPER_DIR := paper/
PAPER_NAME := paper.tex
#SAMPLES_DIR := samples/

# We define the sourcefile as all the cpp file in the source directory
SOURCES := $(shell ls $(SOURCE_DIR)*.cpp)
# And the object files are/will have the same name with ".o" sufix
OBJS := $(patsubst %.cpp, %.o, $(notdir $(SOURCES)))


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
CPPFLAGS += $(shell pkg-config --cflags gthread-2.0 glibmm-2.4)
#CPPFLAGS += -std=c++11

LIBS = $(shell pkg-config  --libs opencv)
LIBS +=  -fopenmp
LIBS += $(shell pkg-config --libs gtkmm-2.4)
LIBS += $(shell pkg-config --libs gthread-2.0 glibmm-2.4)
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



figureDirs := $(ls paper |grep ^fig  | grep -v '\.')
# We tell make to look for pre-requisites in the source directory
vpath %.cpp  src
#~ vpath %.tex  paper


.PHONY: all clean  R linuxRelease install releaseAll paper
#I~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
all : $(EXEC_NAME)

# linking
$(EXEC_NAME) : $(OBJS)
	@echo "==========="
	@echo "Linking $@"
	@$(CXX) $^ $(LIBS) -o $@

%.o : %.cpp
	@echo "Building $*.o"
	@$(CXX) -c $(CPPFLAGS) $(SOURCE_DIR)/$*.cpp -o $*.o
	@gcc -MM $(CPPFLAGS) $(SOURCE_DIR)/$*.cpp > $*.d
	@mv -f $*.d $*.d.tmp
	@sed -e 's|.*:|$*.o:|' < $*.d.tmp > $*.d
	@sed -e 's/.*://' -e 's/\\$$//' < $*.d.tmp | fmt -1 | sed -e 's/^ *//' -e 's/$$/:/' >> $*.d
	@rm -f $*.d.tmp

# pull in dependency info for *existing* .o files
-include $(OBJS:.o=.d)


#I~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
install: $(EXEC_NAME) install/icons/ubitrail.svg ./install/$(EXEC_NAME).desktop
	sudo mkdir -p /usr/share/$(EXEC_NAME)/
	sudo cp ./install/icons/ubitrail.svg /usr/share/$(EXEC_NAME)/
	sudo chmod 755 /usr/share/$(EXEC_NAME)/ubitrail.svg
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
	rm -f $(EXEC_NAME) *.o *.d  *.tar.gz *.out *.pdf *.aux *.log

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
N = $(shell cat $(R_DIR)/DESCRIPTION |grep ^Package | cut --fields=2 --delimiter=: |sed s/\ //g)
V := $(shell cat $(R_DIR)/DESCRIPTION |grep ^Version  | cut --fields=2 --delimiter=: |sed s/\ //g)
R_TGZ := $(N)_$(V).tar.gz
R_PDF := $(N).pdf
vpath %.R  $(R_DIR)
PACKAGE_NAME := rubitrail


R : $(R_TGZ) $(R_PDF)
	@echo "installing rubitrail"
	R CMD INSTALL rubitrail*.tar.gz

$(R_PDF) : $(R_SOURCES)
	R CMD Rd2pdf --force rubitrail

$(R_TGZ) : $(R_SOURCES)
	@echo "Roxygeniting:"
	@echo "library(roxygen2); roxygenise(\"$(PACKAGE_NAME)\",roxygen.dir=$(\"R_DIR\"),copy.package=F,unlink.target=F)" | R --vanilla
	@echo "Building Package $(R_TGZ):"
	@R CMD build rubitrail



#I~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
releaseAll: $(R_TGZ) $(LINUX_TARBALL)
	@echo "uploading $(LINUX_TARBALL)"
	rsync -avP -e ssh $(LINUX_TARBALL) quentelery,ubitrail@frs.sourceforge.net:/home/frs/project/u/ub/ubitrail/linux
	@echo "uploading $(R_TGZ)"
	rsync -avP -e ssh $(R_TGZ) $(R_PDF) quentelery,ubitrail@frs.sourceforge.net:/home/frs/project/u/ub/ubitrail/R
#	make clean


paper: $(PAPER_DIR)$(PAPER_NAME)
	pdflatex $(PAPER_DIR)$(PAPER_NAME)
	
##
