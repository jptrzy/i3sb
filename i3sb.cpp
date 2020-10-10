/*
TODO
-Multi Threading for Block
*/
#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <unistd.h>
#include <stdexcept>
#include <stdio.h>
#include <fstream>
#include <sys/stat.h>
#include <sys/types.h>
#include <pwd.h>

using namespace std;

#include "config.h"

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

int main()
{
    //Get Home Dir
    char *homedir;

    if ( PyWal && (homedir = getenv("HOME")) == NULL) {
        homedir = getpwuid(getuid())->pw_dir;
    }

	//Load PyWal
    string dir = homedir;
    dir+="/.cache/wal/colors";
    ifstream infile( dir );
    if(infile.good()){
        ifstream input( dir );
        int i = 0;
        for( string line; getline( input, line ); )
        {
            Colors[i] = line;
            i++;
        }
    }


    cout << "{ \"version\": 1 }[[],";
    
    //cout << "{ \"full_text\": \"Test\", \"color\": \"#ffffff\", \"background\": \"#00ff00\", \"separator\": false, \"separator_block_width\": 0}," << endl;

    int blockLenght = sizeof(Block)/sizeof(*Block);
    string color;
    string newcolor;

    while(true){
        color = Background;

        cout << "[";
        for(int i = 0; i < blockLenght; i++) {
            newcolor = Colors[ stoi(Block[i][1]) ]; // ERROR replace stoi
            cout << "{ \"full_text\": \"\", \"color\": \"" 
            << newcolor << "\", \"background\": \"" 
            << color <<"\", \"separator\": false, \"separator_block_width\": 0},{ \"full_text\": \" " 
            << exec( (Block[i][0] + " | awk '{printf \"%s\", $0}' " ).c_str()) << " \", \"color\": \"" 
            << Foreground << "\", \"background\": \"" 
            << newcolor << "\", \"separator\": false, \"separator_block_width\": 0}," ;
            color = newcolor;
        }
        
        cout << "{ \"full_text\": \"\", \"color\": \"" 
        << Background << "\", \"background\": \"" 
        << color << "\", \"separator\": false, \"separator_block_width\": 0}," << "{ \"full_text\": \" \", \"color\": \"" 
        << Background << "\", \"background\": \"" 
        << Background << "\", \"separator\": false, \"separator_block_width\": 0}" << "]," << endl;
        usleep(4*1000000); //4 second
    }
    
}
