/*
TODO:
-add pywal color location
*/
#include <iostream> //cout
#include <signal.h> //signal()
#include <unistd.h> //sleep()

#include <vector>
#include <cstdlib>
#include <stdexcept>
#include <stdio.h>
#include <fstream>
#include <sys/stat.h>
#include <sys/types.h>
#include <pwd.h>


using namespace std;


//Define Varibles Types
typedef struct {
	const char *command;
	short colornum;
	short printime;
	short signal;
} Block;



//Define Functions
string exec(const char* cmd);

void signalhandler(int signum);
void terminalhandler(int signum);
void barstatusloop();

void execblock(int id);

void printstatus();
void oldprintstatus();

void loadpywalcolors();

void blockschecktime();



//Import config
#include "config.h"



//Define Varibles
bool running = true;
bool toprint = true;
const int blockscount = sizeof(BLOCKS)/sizeof(*BLOCKS);
string blockstatus[ blockscount ];
int blockstime[ blockscount ]; //Remaining Time to Change Block Status and Then print all



string exec(const char* cmd) {
    char buffer[128];
    string result = "";
    FILE* pipe = popen(cmd, "r");
    if (!pipe) throw runtime_error("popen() failed!");
    try {
        while (fgets(buffer, sizeof buffer, pipe) != NULL) {
            result += buffer;
        }
    } catch (...) {
        pclose(pipe);
        throw;
    }
    pclose(pipe);
    return result;
}

void signalhandler(int signum){

    for(int i = 0; i < blockscount; i++) {
        if(BLOCKS[i].signal == signum-34){
            execblock(i);
        }
    }
    if(!toprint){
        cout << endl << "[ERROR] No block is using signal " << signum-34 << endl << endl;
    }

}

void terminalhandler(int signum){
    running = false;
	exit(0);
}

void barstatusloop(){
    while(running){
        blockschecktime();
        if(toprint){
            oldprintstatus();
            toprint=false;
        }
        sleep(1);
    }
}

void execblock(int id){ //Update Block Status
    blockstatus[id] = exec( BLOCKS[id].command );
    toprint = true;
}

void printstatus(){
    cout << "[{ \"full_text\": \" ";

    for(int i = 0; i < blockscount; i++) {
        cout << blockstatus[i] << " ";
    }

    cout << "\", \"separator\": false, \"separator_block_width\": 0}]," << endl;
}

void oldprintstatus(){
    string newcolor;
    string color = BACKGROUND;

    cout << "[";
    for(int i = 0; i < blockscount; i++) {
        newcolor = COLORS[ BLOCKS[i].colornum ]; // ERROR replace stoi
        cout << "{ \"full_text\": \"\", \"color\": \"" 
        << newcolor << "\", \"background\": \"" 
        << color <<"\", \"separator\": false, \"separator_block_width\": 0},{ \"full_text\": \" " 
        << blockstatus[i] << " \", \"color\": \"" 
        << FOREGROUND << "\", \"background\": \"" 
        << newcolor << "\", \"separator\": false, \"separator_block_width\": 0}," ;
        color = newcolor;
    }
    
    cout << "{ \"full_text\": \"\", \"color\": \"" 
    << BACKGROUND << "\", \"background\": \"" 
    << color << "\", \"separator\": false, \"separator_block_width\": 0}," << "{ \"full_text\": \" \", \"color\": \"" 
    << BACKGROUND << "\", \"background\": \"" 
    << BACKGROUND << "\", \"separator\": false, \"separator_block_width\": 0}" << "]," << endl;

}

void loadpywalcolors(){
    char *homedir;
    if((homedir = getenv("HOME")) == NULL) {
        homedir = getpwuid(getuid())->pw_dir;
    }
        
    string dir = homedir;
    dir+="/.cache/wal/colors";
    ifstream infile( dir );
    if(infile.good()){
        ifstream input( dir );
        int i = 0;
        for( string line; getline( input, line ); )
        {
            COLORS[i] = line;
            i++;
        }
    }
}

void blockschecktime(){
    for(int i = 0; i < blockscount; i++) {
        if(blockstime[i] > 0){
            blockstime[i] -= 1;
            if(blockstime[i] == 0){
                execblock(i);
                blockstime[i] = BLOCKS[i].printime;
            }
        }
    }
}

int main(int argc, char** argv){
    signal(SIGINT, terminalhandler);
    signal(SIGTERM, terminalhandler);
	
    if(PYWAL){ loadpywalcolors(); }

    cout << "{ \"version\": 1 }[[],";
    for(int i = 0; i < blockscount; i++) {
        //execblock(i);
        blockstime[i] = 1;
    }

    for(int i=0; i <= 30; i++){
        signal(34+i, signalhandler);
    }

    barstatusloop();
    return(0);
}