
#ifndef OPTIONPARSER_H
#define OPTIONPARSER_H

#include "optionStructure.hpp"

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
