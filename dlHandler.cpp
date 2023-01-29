#include "dlHandler.hpp"

//constructor for the downloader
dlHandler::dlHandler(){
    this->songQueue = std::queue<std::string>();
    this->prevVal = 0;
    this->queueSize = 0;
    this->isPlaying = false;
}

//deconstructor for the downloader
dlHandler::~dlHandler(){
    while(!this->songQueue.empty()){
        this->songQueue.pop();
        queueSize--;
    }
}

//gets the next song file from the queue
std::string dlHandler::getNext(){
    if(this->songQueue.empty()){
        throw std::runtime_error("Attempted to play from empty queue");
    }
    std::string outval = this->songQueue.front();
    this->songQueue.pop();
    queueSize--;
    return outval;   
}

//adds a song file to the queue
int dlHandler::addQueue(std::string song){
    this->songQueue.push(song);
    queueSize++;
    return queueSize;
}

void dlHandler::clear(){
    for(int i = 1; i <= CACHESIZE; i++){
        std::string rmcmd = "rm ./songs/";
        rmcmd.append(std::to_string(i));
        system(rmcmd.c_str());
        rmcmd.append(".opus");
        system(rmcmd.c_str());
    }
    while(!this->songQueue.empty()){
        this->songQueue.pop();
        queueSize--;
    }
}

//determines if the queue is empty or not
bool dlHandler::hasNext(){
    return !this->songQueue.empty();
}

void dlHandler::setPlaying(){
    this->isPlaying = true;
}

void dlHandler::setStopped(){
    this->isPlaying = false;
}

bool dlHandler::playing(){
    return this->isPlaying;
}

int dlHandler::size(){
    return this->queueSize;
}

//main functionality of the downloader
//downloads a song and converts it to opus filetype
int dlHandler::downloadURL(std::string url){

    std::cout << "queue size: " << queueSize << std::endl;

    //note, store downloads in a seperate folder

    std::string fname = "./songs/";
    //come up with a way to make names for the files
    if((queueSize + 1) < CACHESIZE){
        prevVal++;
        fname.append(std::to_string(prevVal));
        prevVal = prevVal % CACHESIZE;
        
    }
    else{
        std::cout << "queue size: " << queueSize << std::endl;
        prevVal++;
        prevVal = prevVal % CACHESIZE;
        std::string rmcmd = "rm ./songs/";
        rmcmd.append(std::to_string(prevVal));
        system(rmcmd.c_str());
        std::cout<<rmcmd<<std::endl;
        rmcmd.append(".opus");
        std::cout<<rmcmd<<std::endl;
        system(rmcmd.c_str());
        fname.append(std::to_string(prevVal));
        this->songQueue.pop();
        queueSize--;
        
    }


    //builds the download command

    std::string dlcmd = "yt-dlp -o \"";
    dlcmd.append(fname);
    dlcmd.append("\" -f \"ba\" --force-overwrites ");
    dlcmd.append(url);

    //if the download fails, throw an error
    
    if(system(dlcmd.c_str()) != -1){
        //std::cout<< std::to_string(i) << std::endl;
        throw std::runtime_error("Failed to download audio");
    }

    //builds the file conversion command

    std::string concmd = "ffmpeg -i ";
    concmd.append(fname);
    concmd.append(" ");
    concmd.append(fname);
    concmd.append(".opus");

    int i = system(concmd.c_str());

    if(i != -1){
        std::cout<< std::to_string(i) << std::endl;
        throw std::runtime_error("Failed to convert audio");
    }

    fname.append(".opus");

    int len = this->addQueue(fname);
    return len;
}
