#pragma once
#include <string>
#include <queue>
#include <stdlib.h>
#include <iostream>
#include <exception>

#define CACHESIZE 5

class dlHandler{
    public:
        dlHandler();
        ~dlHandler();
        int downloadURL(std::string url);
        std::string getNext();
        void clear();
        bool hasNext();
        void setPlaying();
        void setStopped();
        bool playing();
        int size();

    private:
        std::queue<std::string> songQueue;
        int prevVal;
        int queueSize;
        bool isPlaying;
        int addQueue(std::string song);

};
