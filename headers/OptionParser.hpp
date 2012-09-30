
#ifndef OPTIONPARSER_H
#define OPTIONPARSER_H

#include "optionStructure.hpp"
#include "opencv2/highgui/highgui.hpp"

/**
 * The class responsible parsing command line options and storing them to an Options structure.
 * It also can check for conflicting options.
 */

class OptionParser
{
    public:
        OptionParser();
        OptionParser(int argc, char **argv);
        Options GetOptions() { return opts; }


        bool checkOptions();
    protected:
        void help();
    private:
        Options opts;
        bool valid;
};

#endif // OPTIONPARSER_H
