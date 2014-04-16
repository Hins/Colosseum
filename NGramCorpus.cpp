/*
 * Log.cpp
 * Author: Hins
 * Create date: 09/13/2010
 * Last modified date: 04/16/2014
 */

#include "include\NGramCorpus.h"

namespace NGram{

using namespace std;

NGramCorpus::NGramCorpus(const char* file,
    DataSlideFunc dataSlideFunc, NGramType nGramType = Bigram)
{
    std::ifstream inFile(file, std::ios::in);
    if (!inFile.is_open())
    {
        return;
    }
    std::string str;
    typedef boost::tokenizer<boost::char_separator<char> > Token;
    typedef Token::const_iterator TokenItr;
    boost::char_separator<char> sep(" ");
    
    while (std::getline(inFile, str))
    {
        Token tok(str, sep);
        Token::iterator it = tok.begin();
        for (TokenItr it = tok.begin(); it != tok.end(); ++it)
        {
            NGramDictionaryItr itr = m_NGramDictionary->find(*it);
            if (itr == m_NGramDictionary->end())
            {
                m_NGramDictionary->insert(std::pair<Term, TermSize>(*it, 1));
            }
            else
            {
                TermSize ts = itr->second;
                ts++;
                m_NGramDictionary->erase(itr);
                m_NGramDictionary->insert(std::pair<Term, TermSize>(*it, ts));
            }
            NGramTerms element;
            if (nGramType == Bigram)
            {
                element = {it, it+1};
            }
            else if (nGramType == Trigram)
            {
                element = {it, it+1, it+2};
            }
            NGramContainerItr itr = std::find_if(m_NGramContainer->begin(), NGramContainerItr->end(),
                    [&](NGramContainerItr& it)
                    {
                        if (std::equal(it->first->begin(), it->first->end(), element.begin()))
                        {
                            return it;
                        }
                    });
            Score score = (itr != m_NGramContainer->end()) ? itr->second+1 : 1.0;
            if (itr != m_NGramContainer->end())
            {
                m_NGramContainer->erase(itr);
            }
            m_NGramContainer->insert(std::pair<NGramTerms, Score>(element, score));
        }
    }
    dataSlideFunc(m_NGramContainer, m_NGramDictionary, nGramType);
}

std::ostream& operator<< (std::ostream& os, const NGramCorpus& nGramCorpus)  
{
    BOOST_FOREACH(NGramDictionaryItr itr, nGramCorpus.m_NGramDictionary)
    {
        os<<itr->first<<"\t"<<itr->second<<std::endl;
    }
    os<<"Split line..."<<std::endl;
    BOOST_FOREACH(NGramContainerItr itr, nGramCorpus.m_NGramContainer)
    {
        BOOST_FOREACH(NGramTermsItr it, itr->first)
        {
            if (it+1 != itr->first.end())
            {
                os<<*it<<" ";
            }
            else
            {
                os<<*it;
            }
        }
        os<<"\t"<<itr->second<<std::endl;
    } 
    return os;
}

std::istream& operator>> (std::istream& is, NGramCorpus& nGramCorpus)   
{
    nGramCorpus.m_NGramDictionary.clear();
    nGramCorpus.m_NGramContainer.clear();
    std::string str;
    bool fDictionary = true;
    typedef boost::tokenizer<boost::char_separator<char> > Token;
    typedef Token::const_iterator TokenItr;
    boost::char_separator<char> sep1("\t");
    boost::char_separator<char> sep2(" ");
    while(is)
    {
        is>>str;
        if (str.find("Split line...") != std::string::npos)
        {
            fDictionary = false;
            continue;
        }
        Token tok(str, sep1);
        Token::iterator it = tok.begin();
        if (fDictionary)
        {
            Term t = *it;
            TermSize ts = static_cast<TermSize>(*(it+1));
            nGramCorpus.m_NGramDictionary->insert(std::pair<Term, TermSize>(t, ts));
        }
        else
        {
            Score score = static_cast<Score>(*(it+1));
            Token tok2((*it), sep2);
            NGramTerms nGramTerms;
            std::copy(tok2.begin(), tok2.end(), nGramTerms.begin());
            nGramCorpus.m_NGramContainer->insert(std::pair<NGramTerms, Score>(nGramTerms), score);
        }
    }
    return is;  
} 

}
