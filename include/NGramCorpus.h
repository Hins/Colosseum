/*
 * NGramCorpus.h
 * Author: Hins
 * Create date: 09/07/2010
 * Last modified date: 04/16/2014
 */

#include "NGramConfig.h"

#include<iostream>
#include<fstream>

#include <boost/foreach.hpp>

namespace NGram{

using namespace std;
using namespace boost;

typedef boost::function<bool (boost::shared_ptr<NGramContainer>, boost::shared_ptr<NGramDictionary>, NGramType)> DataSlideFunc;

class NGramCorpus
{
    public:
        NGramCorpus(const char* file,
            DataSlideFunc dataSlideFunc, NGramType nGramType = Bigram) : m_DataSlideFunc(dataSlideFunc), m_NGramType(nGramType);
        ~NGramCorpus(){};

        bool FindTerm(Term term) const
        {
            return m_NGramDictionary->find(term) != m_NGramDictionary->end();
        };

        Score FindNGramTerms(NGramTerms nGramTerms) const
        {
            NGramContainerItr itr = std::find_if(m_NGramContainer->begin(), m_NGramContainer->end(),
                [&](NGramContainerItr& it)
                {
                    return std::equal(nGramTerms.begin(), nGramTerms.end(), NGramContainerItr->first.begin());
                }
                );
            return (itr == m_NGramContainer->end()) ? 0.01 : itr->second;
        };
        NGramType GetNGramType() const {return m_NGramType;};
        bool empty() const {return m_NGramContainer.empty() || m_NGramDictionary.empty();};

        friend std::ostream& operator << (std::ostream& os, const NGramCorpus& nGramCorpus);
        friend std::istream& operator >> (std::istream& is, NGramCorpus& nGramCorpus);
        
    private:
        bool GenerateCorpusByPlain();
        bool GenerateCorpusByMaximumSegmentation();

        boost::shared_ptr<NGramContainer> m_NGramContainer;
        boost::shared_ptr<NGramDictionary> m_NGramDictionary;
        DataSlideFunc m_DataSlideFunc;
        NGramType     m_NGramType;
};

}
