#include "include\NGram.h"

namespace NGram{

int Test(int argc,char* argv[])
{
    CommandType On = static_cast<CommandType>(ParseCommand(argc,argv));
    switch(On)
    {
        case CTHelp:
            PrintHelp();
            break;
        case CTTrain:
            DataSlideFunc dataSlideFunc = boost::bind(CommonSlide);
            NGramType nGramType = std::strcmp(argv[3], "2") ? Trigram : Bigram;
            NGramCorpus* corpus = new NGramCorpus(argv[4], dataSlideFunc, nGramType);
            std::ofstream outFile(CORPUS_PATH, std::ios::out | std::ios::app);
            outFile>>*corpus;
            outFile.flush();
            outFile.close();
            delete corpus;
            break;
        case CTSegment:
            DataSlideFunc dataSlideFunc = boost::bind(CommonSlide);
            SegmentFunc segmentFunc = std::strcmp(argv[3], "1") ? boost::bind(ForwardSegment) : boost::bind(ReverseSegment);
            CalculateResultFunc calculateResultFunc = boost::bind(CalculateSentenceScore);
            NGramType nGramType = std::strcmp(argv[4], "2") ? Trigram : Bigram;
            NGramCorpus* corpus = new NGramCorpus(argv[6], dataSlideFunc, nGramType);
            std::ifstream inFile(argv[5], std::ios::in);
            NGram* instance = new NGram(inFile, segmentFunc, calculateResultFunc, corpus);
            if(!instance->WordBreak())
                return ERROR;
            instance->GetSplitResult(std::cout);
            inFile.close();
            delete corpus;
            delete instance;
            break;
        default:
            PrintHelp();
            break;
    }
    return 0;
    }
}

int main(int argc,char* argv[])
{
    return NGram::Test(argc,argv);
}
