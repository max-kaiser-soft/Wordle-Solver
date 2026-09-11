#include <string>
#pragma once

void setWordList(const char* inputWordList);
std::string getTopTen();
void addUnorderedLetter(const char* letter, int index);
void removeLetters(const char* letters);
void addOrderedLetter(const char* letter, int index);
void giveWordRank();
void scoreLetters();
void readFile();
void resetFileName();
void recalculate();
void restart();

#ifdef __cplusplus
extern "C" {
#endif

char* getTopTen_c();

#ifdef __cplusplus
}
#endif
