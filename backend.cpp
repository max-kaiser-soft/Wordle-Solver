#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

#include "backend.h"
#include "ui.h"

using namespace std;

string fileName = "coreList.txt"; //word list file
bool automaticMode = true;
int wordSize = 5; //length of words; here to play alternate games

//vector<string> wordList = {}; //original word list
vector<pair<string, int>> letterRank = {}; //letters with given ranks
vector<pair<string, int>> wordRank = {}; //remaining words with given ranks

void sortVector(vector<pair<string, int>> &input);

//helpers////////////////////////////////////////////////////////////////////////////////////

extern "C" char* toCString(const std::string& s) { //converts string to cstring, helper
    char* out = (char*)malloc(s.size() + 1);
    memcpy(out, s.c_str(), s.size() + 1);
    return out;
}

extern "C" char* getTopTen_c() { //converts string to cstring
    sortVector(wordRank);
    return toCString(getTopTen());
}

string cleanString(string input){ //returns a string that removes non-letters and shifts to lowercase; tEs5t -> test
    string output = "";

    for(int c = 0; c < input.size(); c++){

        string letter = input.substr(c, 1);

        if (isalpha(static_cast<unsigned char>(letter[0])))
           output += tolower(static_cast<unsigned char>(letter[0]));
    }
    return output;
}

int contains(vector<pair<string, int>> &inputVector, string inputString){ //checks if string element is in vector of pairs (string and int). returns index or -1 for none
    for(int c = 0; c < inputVector.size(); c++){
        if(inputVector[c].first == inputString)
            return c;
    }
    return -1;
}

bool contains(string word, string letter){ //checks if word contains letter, returns boolean
    for(int c = 0; c < word.size(); c++){
        if(word.substr(c, 1).compare(letter) == 0)
            return true;
    }
  return false;
}

bool sortVectorHelper(pair<string, int> x, pair<string, int> y){ //helper function for sortVector()
    return x.second > y.second;
}

void sortVector(vector<pair<string, int>> &input){ //sorts vector of pairs by .second (desc). passes by reference
    sort(input.begin(), input.end(), sortVectorHelper);
}

string getUniqueLetters(string input){ //returns unique letters in a string; abbc -> abc
    string uniqueLetters;
    for (int y = 0; y < input.size(); y++){
        string letter = input.substr(y, 1);
        bool present = false;

        if(uniqueLetters.size() > 0){
            for(int c = 0; c < uniqueLetters.size(); c++){
                if(uniqueLetters.substr(c, 1).compare(letter) == 0)
                    present = true;
            }
        }

        if (!present){
            uniqueLetters += letter;
        }
    }
    return uniqueLetters;
}

//settings////////////////////////////////////////////////////////////////////////////////////

void setWordList(const char* inputWordList){ //determines which word list file to use

    string wordList = inputWordList; //cstring to string

    automaticMode = false;

    if(wordList == "Core List")
        fileName = "coreList.txt";

    if(wordList == "Expanded List")
        fileName = "expandedList.txt";

      if(wordList == "Automatic"){
        fileName = "coreList.txt";
        automaticMode = true;
      }
}

//prints////////////////////////////////////////////////////////////////////////////////////

string getTopTen(){ //prints top ten highest ranked words from wordRank
    int limit;
    string output = "";

    if(!automaticMode){ //manual list mode
        if(wordRank.size() == 0)
            return("No words found.");
        else if(wordRank.size() > 10)
            limit = 10;
        else
            limit = wordRank.size();

        for(int c = 0; c < limit; c++){
            output += wordRank[c].first + " " + to_string(wordRank[c].second);
            if(c != limit - 1)
                output += "\n";
        }
    }
    else{ //automatic mode

        if(wordRank.size() == 0){
            if((fileName == "coreList.txt")){
                fileName = "expandedList.txt";

                manualButtonPress();


                if(wordRank.size() == 0)
                    return("No words found.");
                else if(wordRank.size() > 10)
                    limit = 10;
                else
                    limit = wordRank.size();

                for(int c = 0; c < limit; c++){
                    output += wordRank[c].first + " " + to_string(wordRank[c].second);
                    if(c != limit - 1)
                        output += "\n";
                }

                fileName = "coreList.txt";


                return output;

            }
        else
            return("No words found.");
        }

        else if(wordRank.size() > 10)
            limit = 10;
        else
            limit = wordRank.size();

        for(int c = 0; c < limit; c++){
            output += wordRank[c].first + " " + to_string(wordRank[c].second);
            if(c != limit - 1)
                output += "\n";

        }
    }

    return output;
}

string getNewWord(){ //prints highest ranked word from wordRank
    if(wordRank.size() > 0)
        return(wordRank[0].first);
    return("No words found.");
}


//function////////////////////////////////////////////////////////////////////////////////////

//yellow
void addUnorderedLetter(const char* inputLetters, int index){ //removed words with letter at index and words that are missing the letter somewhere else

    string letters = inputLetters; //cstring to string

    letters = cleanString(letters);

    for(int x = 0; x < letters.size(); x++){
        string letter = letters.substr(x, 1);

        for(int c = wordRank.size() - 1; c >=0; c--){
            if(contains((wordRank[c].first), letter)){
                if(wordRank[c].first.substr(index, 1).compare(letter) == 0){
                    wordRank[c] = wordRank.back();
                    wordRank.pop_back();
                }
            }
            else{
                wordRank[c] = wordRank.back();
                wordRank.pop_back();
            }
        }
    }
}

//gray
void removeLetters(const char* inputLetters){ //removes words from wordRank based on letter input; input like abcd

    string letters = inputLetters; //cstring to string

    letters = cleanString(letters);

    for(int x = 0; x < letters.size(); x++){
        string letter = letters.substr(x, 1);

        for(int y = wordRank.size() - 1; y >= 0; y--){
           if(contains((wordRank[y].first), letter)){
               wordRank[y] = wordRank.back();
               wordRank.pop_back();
            }
        }
    }
}

//green
void addOrderedLetter(const char* inputLetter, int index){ //removes words from wordRank that do not contain letter at index; index starts at zero

    string letter = inputLetter; //cstring to string

    letter = cleanString(letter);

    if(letter.size() == 1){
        for(int c = wordRank.size() - 1; c >=0; c--){
            if(wordRank[c].first.substr(index, 1).compare(letter) != 0){
                wordRank[c] = wordRank.back();
                wordRank.pop_back();
            }
        }
    }
}

int getLetterRank(string input){ //returns the rank of a letter based on letterRank
    for(int c = 0; c < letterRank.size(); c++){
        if(letterRank[c].first.compare(input) == 0){
            return(letterRank[c].second);
        }
    }
    return 0;
}

void giveWordRank(){ //gives words scores in wordRank
    for(int x = 0; x < wordRank.size(); x++){
        int score = 0;
        string word = wordRank[x].first;
        string uniqueWord = getUniqueLetters(word);

        for(int y = 0; y < uniqueWord.size(); y++)
            score += getLetterRank(uniqueWord.substr(y, 1));

        int wordIndex = contains(wordRank, word);

        if(wordIndex == -1)
            wordRank.push_back({word, score});
        else
            wordRank[wordIndex].second = score;
    }
    sortVector(wordRank);
}

void giveLetterRanks(vector<pair<string, int>> &input){ //gives letters scores based on ranks, as opposed to the occurance itself
    int rank = 1;
    for (int c = input.size() - 1; c >= 0; c--){
        bool nextBigger = false;

        if(c > 0){
            if(input[c].second != input[c - 1].second)
                nextBigger = true;
        }

        input[c].second = rank;

        if(nextBigger)
            rank++;
    }
}

void scoreLetters(){ //gives each letter a score based on its appearance in wordRank
    for (int x = 0; x < wordRank.size(); x++){
        string word = wordRank[x].first;
        string uniqueLetters = getUniqueLetters(word); //this is to count letters once per word

        for (int y = 0 ; y < uniqueLetters.size(); y++){
            string letter = uniqueLetters.substr(y, 1);

            int letterIndex = contains(letterRank, letter); //index of letter in letterRank, -1 if missing

            if(letterIndex == -1) //if letterRank contains the current letter
                letterRank.push_back({letter, 1});

            else //else
                letterRank[letterIndex].second++;
        }
    }
    sortVector(letterRank);
    giveLetterRanks(letterRank);
}

string toLowerCase(string input){ //returns string in lower case
    string output;

    for (unsigned char c : input) { //check characters
        output += tolower(c);
    }
    return output;
}

bool checkLine(string input){ //checks line in text file matches correct formatting
    if(input.size() != wordSize) //check length
        return false;
    for (unsigned char c : input) { //check characters
        if (!isalpha(c)) {
            return false;
        }
    }
    return true;
}

void readFile(){ //reads wordlist file and puts words into wordRank vector
    bool formatting = true;
    string line;

    ifstream wordListFile;
    wordListFile.open(fileName);

    if (wordListFile.is_open()){
        while (getline(wordListFile, line) && formatting){

            if (!line.empty() && line.back() == '\r') { //strips hidden Windows characters
                line.pop_back();
            }

            if(checkLine(line)){
                wordRank.push_back({toLowerCase(line), 0});
            }
            else{
                cout<<"Input file formatting error: "<<line<<'\n';
                formatting = false;
            }
        }
    }
    else{
        cout<<"Wordlist file not found."<<'\n';
    }
    wordListFile.close();
}

void resetFileName(){
    if((fileName != "coreList.txt") && (automaticMode)){
        fileName = "coreList.txt";
    }
}

void recalculate(){ //rescores and reorders; this is to be used after word removals
    scoreLetters();
    giveWordRank();
    sortVector(wordRank);
}

void restart(){ //rereads word list and resets choices
    letterRank.clear();
    wordRank.clear();
    readFile();
    //scoreLetters();
    //giveWordRank();
    //sortVector(wordRank);
}
