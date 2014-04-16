/*
 * NGram.h
 * Author: Hins
 * Create date: 09/07/2010
 * Last modified date: 04/16/2014
 */

#include<fstream>
#include<iostream>

#include "NGramCorpus.h"

#include <boost/foreach.hpp>

namespace NGram{

using namespace std;
using namespace boost;

typedef boost::function<bool (RawContent, boost::shared_ptr<NGramCorpus>, SegmentResults&)> SegmentFunc;
typedef boost::function<bool (Sentence, boost::shared_ptr<NGramCorpus>, SegmentResults&)> CalculateResultFunc;

//Ngram base class;
class NGram{
    public:
        NGram(std::ifstream ifs,
            SegmentFunc segmentFunc,
            CalculateResultFunc calculateResultFunc,
            boost::shared_ptr<NGramCorpus> nGramCorpus,
            ) : m_segmentFunc(segmentFunc), m_calculateResultFunc(calculateResultFunc), m_NGramCorpus(nGramCorpus)
        {
            std::copy(std::istream_iterator<unsigned char>(ifs),
                          std::istream_iterator<unsigned char>(), std::back_inserter(m_content));
        };
        virtual ~NGram(){};
        
        bool WordBreak();
        bool GetSplitResult(std::ofstream os);
        void PrintHelp();
    private:
        SegmentResults m_SegmentResults;
        SegmentResult m_BestSegmentResult;
        SegmentFunc m_segmentFunc;
        CalculateResultFunc m_calculateResultFunc;
        boost::shared_ptr<NGramCorpus> m_NGramCorpus;
        boost::shared_ptr<Log> m_log;
        RawContent m_content;

        bool CaclulateSegmentResult();
        void GetBestSegmentResult();
};

}

//#endif //N-GRAM_MODEL_H
