/*
 * Util.h
 * Author: Hins Pan
 * Create date: 09/13/2010
 * Last modified date: 04/16/2014
 */

#include"NGramCorpus.h"
#include<boost/shared_ptr.hpp>
#include<boost/tokenizer.hpp>
#include<boost/foreach.hpp>

namespace NGram{
    
void PrintHelp();
CommandType ParseCommand(int argv, char* argc[]);
    
bool ForwardSegment(RawContent rawContent, 
        boost::shared_ptr<NGramCorpus> container,
        SegmentResults& segmentResults);

bool ReverseSegment(RawContent rawContent, 
        boost::shared_ptr<NGramContainer> container,
        SegmentResults& segmentResults);

bool CommonSlide(boost::shared_ptr<NGramContainer> Container,
        boost::shared_ptr<NGramDictionary> Dictionary,
        NGramType nGramType);

bool GoodTuringSlide(boost::shared_ptr<NGramContainer> Container,
        boost::shared_ptr<NGramDictionary> Dictionary,
        NGramType nGramType);

bool CalculateSentenceScore(Sentence sentence,
    boost::shared_ptr<NGramCorpus> nGramCorpus,
    SegmentResults& segmentResults);
}

#endif
