/*
 * NGram.cpp
 * Author: Hins Pan
 * Create date: 07/26/2010
 * Last modified date: 04/16/2014
 */

#include "include\NGram.h"

namespace Ngram{

void NGram::GetSplitResult(std::ofstream os)
{
    BOOST_FOREACH(Term term, m_BestSegmentResult)
    {
        cout<<term<<std::endl;
    }
}

bool NGram::WordBreak()
{
    if (m_content.empty())
    {
        return false;
    }

    return m_segmentFunc(m_content, m_NGramCorpus, m_SegmentResults);
}

bool NGram::CaclulateSegmentResult()
{
    if (m_SegmentResults.empty())
    {
        return false;
    }
    SegmentResults newSR;
    BOOST_FOREACH(SegmentResultsItr srItr, m_SegmentResults)
    {
        m_calculateResultFunc(srItr->first, m_NGramCorpus, newSR);
    }
    boost::swap(m_SegmentResults, newSR);
}

void NGram::GetBestSegmentResult()
{
    Score maxScore = -1000;
    std::for_each(m_SegmentResults.begin(), m_SegmentResults.end(),
        [&](SegmentResultsItr& segmentResult){
            if (segmentResult->second > maxScore)
            {
                maxScore = segmentResult->second;
                m_BestSegmentResult = segmentResult->first;
            }
    });
}
}
