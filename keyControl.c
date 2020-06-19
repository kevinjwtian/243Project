#include <stdbool.h>
#include <stdlib.h>
#include <math.h>
#define PB_EDGECAPTURE ((volatile long*) 0xFF20005C)
#define RLEDS ((volatile long*) 0xFF200000)
#define counter_max ((volatile int) 500000)

int main(){
	long PBreleases;
	int i;
	*PB_EDGECAPTURE = 0;
	int key;
    int counter_left = 0; 
    int counter_right = 0; 
	int lane = 230;
    //flags
    bool lane_change = false; 

    while (1) { //in game loop
    
        //Check key interupt
        PBreleases = *PB_EDGECAPTURE;//variable to mess with
        key = PBreleases & 0b1111;
        if(PBreleases){
            //*RLEDS = PBreleases;
            switch (key)
            {
            case 1: // code to be executed if key = 0; right 
                    if(lane<289 && !lane_change){ 
                        lane_change = true; //make in action
                        counter_right = counter_max; //load counter
                        break;
                    }
                    
            case 2: // code to be executed if key = 1; left
                    if(lane>229&& !lane_change){
                    lane_change = true; //make in action
                    counter_left = counter_max; //load counter
                        break;
                    }
                break;
            case 4: // code to be executed if key = 2;
                    for(i = 0; i<1000000;i++)
                    *RLEDS = 0x0004;
                break;
            case 8: // code to be executed if key = 3;
                    for(i = 0; i<1000000;i++)
                    *RLEDS = 0x0008;
                break;
            default:
                break;
            }
        
        *PB_EDGECAPTURE = PBreleases;
        }

		if(lane_change){
            if(counter_right > 0){
                counter_right --;
                *RLEDS = 0x7;
            }
            else if(counter_left > 0){
                counter_left --;
                *RLEDS = 0x380;
            }
            else{
                *RLEDS = 0;
                lane_change = false;
            }
        }


	}
}