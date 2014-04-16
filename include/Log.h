/*
 * Log.h
 * Author: Hins Pan
 * Create date: 09/13/2010
 * Last modified date: 04/16/2014
 */

#include<iostream>
#include<fstream>

namespace NGram {

using namespace std;
class Log{
    public:
        Log()
        {
            m_oStream.open(m_file, std::ios::out | std::ios::app);
        };
        ~Log()
        {
            m_oStream.flush();
            m_oStream.close();
        };
        void Write(std::string s)
        {
            if (m_oStream)
            {
                m_oStream<<s<<std::endl;
            }
        };
        void Read(std::ifstream iStream)
        {
            if (m_oStream)
            {
                iStream>>m_oStream;
            }
        };
    private:
        std::ofstream m_oStream; 
        static const char* m_file;
};

const char* Log::m_file = "../log/log";
}

#endif
