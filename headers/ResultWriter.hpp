#ifndef RESULTWRITER_H
#define RESULTWRITER_H

#include <fstream>
#include <iostream>
#include <sstream>
#include <iomanip>
#include "Tracker.hpp"

#include "optionStructure.hpp"


/**
 * The class responsible for writing results to a result file.
 * It can stores the result of each trackers (potentialy updated simultanously by different threads)
 * and then write them all.
 * If no result file exits, it writes in stdout.
 * The results are store in a data-fram format : each row is an individual read and each colum a diferent variable.
 * In addition, the first lien of the output contains global information formated as an evaluable R code.
 */


class ResultWriter
{
    public:
        /**
         * The default constructor.
         */
        ResultWriter();

        /**
         * Another contructor. A ForegroundExtractor object cannot be used unless it was constructed in this manner.
         * @param trackers a vector of Tracker objects.
         * @param options an Options structure.
         */
        ResultWriter(std::vector<Tracker>* trackers, Options& options,int width,int height);

        virtual ~ResultWriter();

        /**
         * A public member function gathering information about a tracker.
         * @param i the index of the tracker.
         * @param the time (in ms).
         *
         * The function does not write immediately the data, it simply stores data from each
         * Tracker object in a string ready to be writen by flush(). This architecture is to avoid threading overhead
         * and the risk that two threads write data simultaneously in the same file.
         */
        void writeTracker(int i,long time);

        /**
         * A public member function writing informations already gathered (by writeTracker()).
         * It should be called by the master thread.
         */
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
        int m_width;
        int m_height;
};

#endif // RESULTWRITER_H
