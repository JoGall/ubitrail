#ifndef RESULTWRITER_H
#define RESULTWRITER_H

#include <fstream>
#include <iostream>
#include <sstream>
#include <iomanip>
//#include <vector>
#include "Tracker.hpp"

#include "optionStructure.hpp"

class ResultWriter
{
    public:
        ResultWriter();
        ResultWriter(std::vector<Tracker>* trackers, Options& options);
        virtual ~ResultWriter();
        void writeTracker(int i,long time);
        void flush();
    protected:
        void writeResultRow(std::string label, int territory, float X, float Y, float t, float L, int i);
        void writeHead();
        const std::string currentDateTime();
    private:
        Options m_options;
        std::ofstream outputFile;
        bool toSTD;
        std::vector<Tracker> *m_trackers;
        std::stringstream* tss;
};

#endif // RESULTWRITER_H
