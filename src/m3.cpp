#include <iostream>
#include <string>
#include "raylib.h"
#include "m3.h"
#include "ui.h"
#include "window_state.h"
#include "file_dialog.h"

using namespace std;



M3WindowState m3WinState;

M3::M3(){
    InitWindow(m3WinState.WINDOW_WIDTH, m3WinState.WINDOW_HEIGHT, m3WinState.WINDOW_TITLE);
    SetWindowState(FLAG_WINDOW_RESIZABLE);
    Image icon = LoadImage("../res/icon.png");
    SetWindowIcon(icon);
}


void M3::setUpdatedWindowSize(M3WindowState* m3winstate){
    int updatedWidth = GetRenderWidth();
    int updatedHeight = GetRenderHeight();
    
    m3winstate->WINDOW_WIDTH = updatedWidth;
    m3winstate->WINDOW_HEIGHT = updatedHeight;
}

RetCode M3::startPlayerEventLoop(){   
    if(!IsWindowReady()){ 
        return WINDOW_NOT_INITIALIZED;
    } 
    
    ToolBar tb;
    BaseBar bb;
    TimeLine tl;
    VolumeControl vc;
    PlayBackAddons pb;
    SplashScreenLogo spsl;

    InitAudioDevice();
    
    string filePath = getFileNameUsingWinDialog();
    Music musicStream = LoadMusicStream(filePath.c_str()); 
    PlayMusicStream(musicStream);
 
    pb.isPlaying = true;
    
    float timePlayedPercent = 0.0f;
    float currMusicTime, totalMusicTime; 

    while(!WindowShouldClose()){
        ClearBackground(BLACK);

        setUpdatedWindowSize(&m3WinState); 
        
        UpdateMusicStream(musicStream);

        timePlayedPercent = GetMusicTimePlayed(musicStream)/GetMusicTimeLength(musicStream); 
        
        BeginDrawing();
        
        tb.renderToolBar(&m3WinState);
        Event ev = tb.checkEventTriggered();
        tb.processEvent(ev);
        
        bb.renderBaseBar(&m3WinState);
       

        if (timePlayedPercent > 1.0f) timePlayedPercent = 1.0f; // dont let the timePlayed go higher than orignal lenght;
        
        currMusicTime = GetMusicTimePlayed(musicStream);
        totalMusicTime = GetMusicTimeLength(musicStream);

        tl.renderTimeLine(&m3WinState, timePlayedPercent, currMusicTime, totalMusicTime);

        vc.renderVolumeControl(&m3WinState);
 
        
        int volbar = vc.getVolumeBar();
        /*
         //didn't work effectively 
        float musicVOl = ( (volbar - 1) / (4) ); // (x - 1) / (5 - 1) 
        musicVOl += musicVOl<= 0.7 ? 0.2: 0.1;                                   // + 0.2 if < 1 else 0.1
        */
        float musicVol;

        switch(volbar){
            case 1: musicVol = 0.2; break;
            case 2: musicVol = 0.4; break;
            case 3: musicVol = 0.6; break;
            case 4: musicVol = 0.8; break;
            case 5: musicVol = 1.0; break;
        }

        SetMusicVolume(musicStream, musicVol);
        
        Event ev2 = vc.checkEventTriggered();  
        vc.processEvent(ev2);
       
        
        pb.renderPlayBackAddons(&m3WinState);
        
        Event ev3 = pb.checkEventTriggered();
        
        if(ev3 == STATE_STOP_MUSIC){
            PauseMusicStream(musicStream);
        }

        else if(ev3 == STATE_PLAY_MUSIC){
            ResumeMusicStream(musicStream);
        }
        else if(ev3 == SEEK_BACK){
            // seek 2 secs backward
            SeekMusicStream(musicStream,  GetMusicTimePlayed(musicStream) - 2.0);
        }
        

        else if(ev3 == SEEK_AHEAD){ 
            // seek 2 secs forward
            SeekMusicStream(musicStream,  GetMusicTimePlayed(musicStream) + 2.0);
        }
        
        
        spsl.renderSplahScreenLogo(&m3WinState);

        /*
        if(!pb.isPlaying){
                	  DrawTriangle((Vector2){m3WinState.WINDOW_WIDTH / 2 + 20, m3WinState.WINDOW_HEIGHT / 2}, 
                     (Vector2){m3WinState.WINDOW_WIDTH / 2 - 20, m3WinState.WINDOW_HEIGHT / 2 - 30}, 
                     (Vector2){m3WinState.WINDOW_WIDTH / 2 - 20, m3WinState.WINDOW_HEIGHT / 2 + 30},RAYWHITE);
        }
        else{

        }
        */
        
        EndDrawing();
        }

    UnloadMusicStream(musicStream);
    CloseAudioDevice();
    
    return SUCCESS;
}


