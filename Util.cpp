/*
 * Util.cpp
 * Author: Hins Pan
 * Create date: 09/13/2010
 * Last modified date: 04/16/2014
 */

#include "include\Util.h"

namespace Ngram{

void PrintHelp()
{
    cout<<"Usage: Ngram [Options] [SlideType] [SegmentType] [NGramType] [File] [Corpus]"<<std::endl;
    cout<<"SlideType -- data slide type"<<std::endl;
    cout<<"       1 -- Common slide"<<std::endl;
    cout<<"       2 -- Katz slide"<<std::endl;
    cout<<"       3 -- Jelinek slide"<<std::endl;
    cout<<"       4 -- Witten-Bell slide"<<std::endl;
    cout<<"       5 -- Absolute slide"<<std::endl;
    cout<<"       6 -- Kneser-Ney slide"<<std::endl;
    cout<<"SegmentType -- segment sentence's algorihtm"<<std::endl;
    cout<<"       1 -- Forward segment"<<std::endl;
    cout<<"       2 -- Reverse segment"<<std::endl;
    cout<<"       3 -- Best segment: get best result from either forward or reverse segment"<<std::endl;
    cout<<"NGramType -- 2 or 3"<<std::endl;
    cout<<"Corpus -- corpus path need to be trained"<<std::endl;
    cout<<"File -- test file"<<std::endl;
    cout<<"Options -- command parameters"<<std::endl;
    cout<<"-t -- train a corpus by some data slide method"<<std::endl;
    cout<<"Format: -t [Slidetype] [NGramType] [Corpus]"<<std::endl;
    cout<<"-s -- segment a file"<<std::endl;
    cout<<"Format: -s [SlideType] [SegmentType] [NGramType] [File] [Corpus]"<<std::endl;
    cout<<"-h -- get help information"<<std::endl;
    cout<<"Format: -h"<<std::endl;
}
    
CommandType ParseCommand(int argc, char *argv[])
{
    if(argc == 2 && !std::strcmp(argv[1],"-h"))
        return CTHelp;
    if(argc == 4 && !strcmp(argv[1],"-t"))
        return CTTrain;
    if(argc == 5 && !strcmp(argv[1],"-s"))
        return CTSplit;
    return CTHelp;
}

bool ForwardSegment(RawContent rawContent,
    std::size_t offset,
    boost::shared_ptr<NGramCorpus> container,
    SegmentResults& segmentResults,
    SegmentResult& segmentResult)
{
    for (std::size_t i = offset; i < rawContent.size(); i++)
    {
        Term curTerm = (rawContent[i]&0x80 && (unsigned)rawContent[i] >= 0) ?
            rawContent.substr(i++,2) : rawContent.substr(i,1);
        if (container->FindTerm(curTerm))
        {
            ForwardSegment(rawContent, i, container, segmentResults, segmentResult);
            Term tSegmentResult = "";
            BOOST_FOREACH(Term term, segmentResult)
            {
                tSegmengResult += term + " ";
            }
            segmentResults.insert(std::pair<Sentence, score>(tSegmentResult, float(0.0)));
        }
    }
}

bool ReverseSegment(RawContent rawContent,
    std::size_t offset,
    boost::shared_ptr<NGramCorpus> container,
    SegmentResults& segmentResults,
    SegmentResult& segmentResult)
{
    for (std::size_t i = offset; i >= 0; i--)
    {
        Term curTerm = (rawContent[i]&0x80 && (unsigned)rawContent[i] >= 0) ?
            rawContent.substr(i--,2) : rawContent.substr(i,1);
        if (container->FindTerm(curTerm))
        {
            ForwardSegment(rawContent, i, container, segmentResults, segmentResult);
            Term tSegmentResult = "";
            BOOST_FOREACH(Term term, segmentResult)
            {
                
tSegmengResult += term + " ";
            }
            segmentResults.insert(std::pair<Sentence, score>(tSegmentResult, float(0.0)));
        }
    }
}

bool CommonSlide(boost::shared_ptr<NGramContainer> Container,
        boost::shared_ptr<NGramDictionary> Dictionary,
        NGramType nGramType)
{
    if (nGramType == Bigram)
    {
        BOOST_FOREACH(NGramDictionaryItr term1, Dictionary)
        {
            BOOST_FOREACH(NGramDictionaryItr term2, Dictionary)
            {
                NGramTerms cur{term1->first, term2->first};
                Score curScore = 0.0;
                std::find_if(Container->begin(), Container->end(),
                        [&](NGramContainerItr& itr)
                        {
                            if (std::equal(itr->first, itr->end(), cur.begin()))
                            {
                                curScore = itr->score;
                                Container->erase(itr);
                                return true;
                            }
                            return false;
                        });
                Container->insert(std::pair<NGramTerms, Score>(cur, static_cast<double>(COMPAR+curScore) /static_cast<double>(COMPAR*Dictionary->size()+ term1->second)));
            }
        }
    }
    else if (nGramType == Trigram)
    {
        BOOST_FOREACH(NGramDictionaryItr term1, Dictionary)
        {
            BOOST_FOREACH(NGramDictionaryItr term2, Dictionary)
            {
                BOOST_FOREACH(NGramDictionaryItr term3, Dictionary)
                {
                    NGramTerms cur{term1->first, term2->first, term3->first};
                    Score curScore = 0.0;
                    std::find_if(Container->begin(), Container->end(),
                            [&](NGramContainerItr& itr)
                            {
                                if (std::equal(itr->first, itr->end(), cur.begin()))
                                {
                                    curScore = itr->score;
                                    Container->erase(itr);
                                    return true;
                                }
                                return false;
                            });
                    Container->insert(std::pair<NGramTerms, Score>(cur, static_cast<double>(COMPAR+curScore) /static_cast<double>(COMPAR*Dictionary->size()+ term1->second)));
                }
            }
        }
    }
    return true;
}

bool GoodTuringSlide(boost::shared_ptr<NGramContainer> Container,
    boost::shared_ptr<NGramDictionary> Dictionary,
    NGramType nGramType)
{
    return true;
}

bool CalculateSentenceScore(Sentence sentence,
    boost::shared_ptr<NGramCorpus> nGramCorpus,
    SegmentResults& segmentResults)
{
    if (sentence.empty() ||
        nGramCorpus->empty())
    {
        return false;
    }
    
    typedef boost::tokenizer<boost::char_separator<char> > Token;
    typedef Token::const_iterator TokenItr;
    boost::char_separator<char> sep(" ");
    Token tok(sentence, sep);

    NGramType nGramType = nGramCorpus->GetNGramType();
    Score result = 1.0;
    BOOST_FOREACH(TokenItr itr, tok)
    {
        NGramTerms nGramTerms;
        nGramTerms.push_back(*itr);
        if (itr+1 != tok.end())
        {
            if (nGramType == Bigram)
            {
                result /= nGramCorpus->FindNGramTerms(nGramTerms);
                nGramTerms.push_back(*(itr+1));
                result *= nGramCorpus->FindNGramTerms(nGramTerms);
            }
            else if (nGramType == Trigram)
            {
                nGramTerms.push_back(*(itr+1));
                if (itr+2 != tok.end())
                {
                    result /= nGramCorpus->FindNGramTerms(nGramTerms);
                    nGramTerms.push_back(*(itr+2));
                    result *= nGramCorpus->FindNGramTerms(nGramTerms);
                }
            }
        }
    }
    segmentResults->insert(std::pair<Sentence, Score>(sentence, result));
    return true;
}

}
