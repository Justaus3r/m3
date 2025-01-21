#include <iostream>
#include "raylib.h"
#include "m3.h"
#include "error_codes.h"

int main(){
    
    RetCode rC;
    M3 player;

    rC = player.startPlayerEventLoop();
    
    switch(rC){      
        case WINDOW_NOT_INITIALIZED:{
            std::cout<<"ERROR: error while initializing the window";
            return WINDOW_NOT_INITIALIZED; 
        }
  
    }
    return 0;
}

