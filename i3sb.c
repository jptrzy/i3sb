#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#include <unistd.h>
#include <string.h>
#include <signal.h> 

#define WAITING_FOR_REQUEST 1
#define socket_status(a) WAITING_FOR_REQUEST



/* macros */
#define MIN(a, b)               ((a) < (b) ? (a) : (b))
#define MAX(a, b)               ((a) < (b) ? (b) : (a))
#define LEN(a)                  (sizeof(a) / sizeof(a)[0])

typedef struct {
	const char *command;
	short print_time;
	short signal;
} Block;

void tick_loop();
void check_blocks_time();
void update_block();
void update_status();
void runtime_error(char* msg);
void exec_command();

#include "config.h"

char blocks_status[ LEN(BLOCKS) ][ STATUS_LENGTH ] = {0};
int	blocks_time[ LEN(BLOCKS) ] = {0};

bool running = true;
bool print_needed = true;



void tick_loop(){
	printf("{ \"version\": 1 }[[],");
	while(running){
        check_blocks_time();
        if(print_needed){
            update_status();
            print_needed=false;
        }
        sleep(1);
    }
	printf("[]]}");
	fflush(stdout);
}

void check_blocks_time(){
	for(int id=0; id<BLOCKS_SIZE; id++){
		if(blocks_time[id] == 0){
			update_block(id);
			blocks_time[id] = BLOCKS[id].print_time;
		}
		blocks_time[id]--;
	}
}


void update_block(int id){ //TODO remove this function
	exec_command(BLOCKS[id].command, &blocks_status[id]);
	print_needed = true;
}

void update_status(){
	printf("[{ \"full_text\": \"");
	printf(SEPARATOR_SYMBOL);
	for(int id=0; id<BLOCKS_SIZE; id++){
		printf(blocks_status[id]);
		printf(SEPARATOR_SYMBOL);
	}
	printf("\", \"separator\": false, \"separator_block_width\": 0}],");
	fflush(stdout);
}

//Return only one line of cmd output
void exec_command(const char* cmd, char* str){
	char buffer[STATUS_LENGTH];

    FILE* pipe = popen(cmd, "r");
	if (!pipe){ 
		runtime_error("Failed to run block cmd.");
		return; 
	}

    fgets(buffer, STATUS_LENGTH-1, pipe);
    pclose(pipe);

	strcpy(str, buffer);
	str[strlen(str)+1] = '\0';
}

void runtime_error(char* msg){
	char command[1024];

    strcpy(command,"notify-send -u critical \"i3sb Error\" \"");
    strcat(command,msg);
	strcat(command,"\"");

    system(command);
}

void exit_signal(int signum){
	running = false;
}

void block_signal(int signum){
	for(int id=0; id<BLOCKS_SIZE; id++){
		if(BLOCKS[id].signal == signum-SIGRTMIN){
			update_block(id);
		}
	}
}

void setup_signals(){
	signal(SIGINT, exit_signal);
    signal(SIGTERM, exit_signal);

	// OLDCODE
	// sigemptyset(&sa.sa_mask);
	// sigaddset(&sa.sa_mask, 35);
	// sa.sa_handler = block_signal;
	// sigaction(35, &sa, 0);

	for(int i=0; i <= 30; i++){
        signal(SIGTERM+i, block_signal);
    }
}


int main() {
	setup_signals();

	//Main Status Loop
	tick_loop();

	return 0;
}