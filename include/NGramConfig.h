/*
 * NGramConfig.h
 * Author: Hins
 * Create date: 09/13/2010
 * Last modified date: 04/16/2014
 */

#include<iostream>
#include<string>
#include<map>
#include<vector>
#include<algorithm>
#include<exception>
#include<time.h>
#include<stdio.h>

#include<boost/tokenizer.hpp>

/*
 * Data macro constant definition
 */
#define COMPAR 0.5
#define WORDMAXLEN 5
#define MAX_SIZE 1024
#define ERROR -1

/*
 * String macro constant definition
 */
#define OPEN_COR_ERROR "open corpus failed!"
#define OPEN_COR_OK "open corpus successfully!"
#define READ_COR_ERROR "read corpus failed!"
#define START_PARSE_COR "start parsing corpus..."
#define PAR_COR_OK "parse corpus successfully!"
#define FIND_RESULT "find a result!"
#define OPEN_LOG_ERROR "open log file failed!"
#define WRITE_LOG_ERROR "write log file failed!"
#define READ_LOG_ERROR "read log file failed!"
#define CREATE_BIGRAM "create Bigram successfully!"
#define CREATE_TRIGRAM "create Trigram successfully!"
#define DESTROY_BIGRAM "destroy Bigram successfully!"
#define DESTROY_TRIGRAM "destroy Trigram successfully!"
#define START_SPLIT_SEN "start split sentence"
#define SPLIT_SEN_OK "split sentence successfully!"
#define WORDFILE_OK "word file has existed!"
#define OPEN_WORD_ERROR "open word failed!"
#define WRITE_WORD_OK "write word file successfully!"
#define CORPUSFILE_OK "corpus file has existed!"
#define OPEN_CORPUS_ERROR "open corpus failed!"
#define WRITE_CORPUS_OK "write corpus file successfully!"
#define GTERROR "Good turing slide error!"
#define READFROM_FILE "read data from file!"
#define ENTER 0x1c0d

/*
 * Path macro constant definition
 */
const std::string PATH = "/data0/home/xiaotong/pub/projects/gsps/src/media/NLP/NgramStone";
const std::string CORPUS_PATH = PATH + "/data/chinese corpus.txt";
const std::string WORD = PATH + "/data/word";
const std::string COR_FOLDER = PATH + "/data/corpus";
#define BI_POSTFIX "_Bi"
#define TRI_POSTFIX "_Tri"

namespace NGram{

//using namespace std;

typedef unsigned int TermSize; //size_t may insufficient;
typedef std::string Term;
typedef std::string Sentence;
typedef std::string RawContent;
typedef float Score;
//const size_t NGramUpperBound = 5;
typedef std::vector<Term> NGramTerms;
typedef std::map<NGram::NGramTerms,Score> NGramContainer;
//typedef std::map<NGramTerms,Score>::const_iterator NGramContainerItr;
typedef std::map<Term,TermSize> NGramDictionary;
//typedef NGramDictionary::const_iterator NGramDictionaryItr;
typedef std::vector<Term> SegmentResult;
typedef SegmentResult::const_iterator SegmentResultItr;
//typedef std::map<Sentence, Score > SegmentResults;
//typedef typename SegmentResults::const_iterator SegmentResultsItr;

//DataSlideType -- different data slide types;
typedef enum DataSlideType 
{
    Common = 0,
    Katz,
    Jelinek,
    Witten,
    Absolute,
    Kneser
}DataSlideType;

//Segment type -- forward and reverse;
typedef enum SegmentType 
{
    Plain = 0,
    Forward,
    Reverse,
    Best    // Just put Best as a placeholder;
}SegmentType;

typedef enum NGramType
{
    Bigram = 0,
    Trigram
}NGramType;

typedef enum CommandType 
{
    CTHelp = 0,
    CTTrain,
    CTSegment,
    CTEstimate
}CommandType;

}
