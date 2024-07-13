#include "ui.h"
#include "raylib.h"
#include "window_state.h"
#include <cstdio>
#include <iostream>
#include <fstream>

M3WindowState m3winState;

ToolBar::ToolBar(){
    posX = 0;
    posY = 0;
    width = m3winState.WINDOW_WIDTH;
    height = 25;
                // posx, posy, width, height 
    toolbarRec = {posX, posY, width, height};
    openButtonText = "Open a File...";
    int openButtonWidth = MeasureText(openButtonText, 14) + 4;
                    // posx, posy, width, height
    openFileButton = {3 , 0, openButtonWidth, 25};
    defaultColor = LIGHTGRAY;
}

void ToolBar::renderToolBar(M3WindowState* m3winState){
    toolbarRec.width = m3winState->WINDOW_WIDTH;
    DrawRectangleRec(toolbarRec, defaultColor); // render toolbar
    DrawRectangleRec(openFileButton, (isButtonBeingHovered ? DARKGRAY:defaultColor)); // render button
                           // x, y,fontsize,color
    DrawText(openButtonText, 4, 4, 14, RAYWHITE); // render button text. also text's x => +1 than button location
    isButtonBeingHovered = false;
}

Event ToolBar::checkEventTriggered() {
    Event ev;
    if(CheckCollisionPointRec(GetMousePosition(), openFileButton))
    {
        isButtonBeingHovered = true;
        if(IsMouseButtonReleased(MOUSE_LEFT_BUTTON)){
            ev = STATE_CLICK_OPENFILE;
        }
    }
    else{
        ev = STATE_NORMAL;
    }
    return ev;
}

void ToolBar::processEvent(Event ev) {
    if(ev == STATE_CLICK_OPENFILE){
       DrawText("I work with non blocking events", 200, 200, 14, WHITE);
    }
}


BaseBar::BaseBar(){
    posX = 0;
    width = m3winState.WINDOW_WIDTH;
    height = m3winState.WINDOW_HEIGHT * 0.15; // height is 15% of entire window height from bottom. don't ask why it is.
    posY = m3winState.WINDOW_HEIGHT - height;
    baseBarRec = {posX, posY, width, height};
    defaultColor = LIGHTGRAY;
}


void BaseBar::renderBaseBar(M3WindowState *m3winstate){
    baseBarRec.width = m3winstate->WINDOW_WIDTH;
    baseBarRec.y = m3winstate->WINDOW_HEIGHT - (m3winstate->WINDOW_HEIGHT * 0.15);
    baseBarRec.height = m3winstate->WINDOW_HEIGHT * 0.15; 
    DrawRectangleRec(baseBarRec, defaultColor);
}

Event BaseBar::checkEventTriggered(){
    return STATE_NORMAL;
}

void BaseBar::processEvent(Event ev){
    ;
}

TimeLine::TimeLine(){
    durationStrSizeSingle = MeasureText(" 00:00 ", 11); // text width covered by single duration string
    posX = durationStrSizeSingle + 1;
    durationStrSizeDouble = durationStrSizeSingle * 2;
    width = m3winState.WINDOW_WIDTH - (durationStrSizeDouble + 2); // +2 as a caution bufer b/w timeline and duration
    height = 12;
    posY =  (m3winState.WINDOW_HEIGHT - (m3winState.WINDOW_HEIGHT * 0.15) ) + 5; // +5 to bound timeline under basebox
    
    posXCurrentlyPLayed = 2; 
    posXTotalLength = width + durationStrSizeSingle + 3;  

    timeLineRecEmpty = {posX, posY, width, height};
    timeLineRecPlaying = {posX, posY, 0, height};
    emptyColor = DARKGRAY;
    playingColor = MAROON;
}

void TimeLine::renderTimeLine(M3WindowState *m3winstate, float timePlayedPercent, float currMusicTime, float totalMusicTime){
    
    currentlyPlayedFloatTime = currMusicTime;
    totalLengthFloatTime = totalMusicTime;
             
     secsToMinSecs(currentlyPlayedFloatTime, currentlyPlayedTime);
     secsToMinSecs(totalLengthFloatTime, totalLengthTime);
   

    //creating empty timeline
    timeLineRecEmpty.width = m3winstate->WINDOW_WIDTH - ((durationStrSizeDouble + 2));
    timeLineRecEmpty.y =  (m3winstate->WINDOW_HEIGHT - (m3winstate->WINDOW_HEIGHT * 0.15) ) + 5;
   

    //creating currently playing timeline
    timeLineRecPlaying.y = (m3winstate->WINDOW_HEIGHT - (m3winstate->WINDOW_HEIGHT * 0.15) ) + 5;
    timeLineRecPlaying.width = timeLineRecEmpty.width * timePlayedPercent;

    DrawRectangleRec(timeLineRecEmpty, emptyColor);
    DrawRectangleRec(timeLineRecPlaying, playingColor);

    DrawRectangleLinesEx(timeLineRecEmpty, 1.0, BLACK);

    //show timelime durations(both currentlyplayed, totallength)
    width  = m3winstate->WINDOW_WIDTH - (durationStrSizeDouble + 2);
    posXTotalLength =  width + durationStrSizeSingle + 3;  
    DrawText(currentlyPlayedTime, posXCurrentlyPLayed, timeLineRecEmpty.y, 11, BLACK ); 
    DrawText(totalLengthTime, posXTotalLength, timeLineRecEmpty.y, 11, BLACK ); 

}

Event TimeLine::checkEventTriggered(){
    return STATE_NORMAL;
}

void TimeLine::processEvent(Event ev){
    ;
}

void TimeLine::secsToMinSecs(float secs, char secsMins[100]){
    int minutes = (int)secs / 60;
    int seconds = (int)secs % 60;
    sprintf(secsMins, "%02d:%02d", minutes, seconds);
}


VolumeControl::VolumeControl(){
    // per volume bar width: 10
    // per 5 volume bar: 50;
    // + '20' padding buffer: 70;
    posX = m3winState.WINDOW_WIDTH - 70;  
    width = 10;
    height = 11;
    padding = 2; // padding between volum bars
    posY = ( m3winState.WINDOW_HEIGHT - ((m3winState.WINDOW_HEIGHT * 0.15) / 2) ); // subtract half of 15% from window height
   
    volume[0] = {posX, posY, width, height};

    for(int i=1;i<4;++i){
        volume[i] = {volume[i - 1].x + padding + width, posY, width, height};
    }
    
    volText = "Vol: ";
    volumeBarCurrIdx = 2;
    Colored = RED;
    unColored = GRAY;
}

void VolumeControl::renderVolumeControl(M3WindowState *m3winstate){
    posX = m3winstate->WINDOW_WIDTH - 70; 
    posY = ( m3winstate->WINDOW_HEIGHT - ((m3winstate->WINDOW_HEIGHT * 0.15) / 2) ); // subtract half of 15% from window height 
    
    volume[0] = {posX, posY, width, height};

    for(int i=1;i<5;++i){
        volume[i] = {volume[i - 1].x + padding + width, posY, width, height};
        std::cout<<volume[i].x<<std::endl;
    }

    DrawText(volText, posX - (MeasureText(volText,11) + 2), posY, 11, GRAY);

    for(int i=0;i<5;++i){ 
        DrawRectangleRec(volume[i], (i < volumeBarCurrIdx ? Colored: unColored));
    }

}

Event VolumeControl::checkEventTriggered(){
    for(int i=0;i<5;++i){
        
        if(CheckCollisionPointRec(GetMousePosition(), volume[i])){
            
            if(IsMouseButtonReleased(MOUSE_LEFT_BUTTON)){
                tmpVolumeIdxStore = i;
                return STATE_CHOOSE_VOLUME;
            }
        }
    }
    return STATE_NORMAL;
}

void VolumeControl::processEvent(Event ev){
    if(ev == STATE_CHOOSE_VOLUME ){
        volumeBarCurrIdx = ++tmpVolumeIdxStore;
    }
}

int VolumeControl::getVolumeBar(){
    return volumeBarCurrIdx;
}

PlayBackAddons::PlayBackAddons(){
    posX = m3winState.WINDOW_WIDTH / 2;
    posY = ( m3winState.WINDOW_HEIGHT - ((m3winState.WINDOW_HEIGHT * 0.15) / 2) );
    height = 30;
    width  = 30;
    
    playImPath = "../res/play.png"; 
    _playIm = LoadImage(playImPath);
    ImageResize(&_playIm, width, height);
    playImTexture = LoadTextureFromImage(_playIm);
    
    pauseImPath = "../res/pause.png";
    _pauseIm = LoadImage(pauseImPath);
    ImageResize(&_pauseIm, width, height);
    pauseImTexture = LoadTextureFromImage(_pauseIm);

    rewindBackwardsImPath = "../res/rewind _backwards.png";
    _rewindBackwardsIm  = LoadImage(rewindBackwardsImPath);
    ImageResize(&_rewindBackwardsIm, width, height);
    rewindBackwardsTexture = LoadTextureFromImage(_rewindBackwardsIm);    
   
    
    rewindForwardsImPath = "../res/rewind_forwards.png"; 
    _rewindForwardsIm = LoadImage(rewindForwardsImPath);
    ImageResize(&_rewindForwardsIm, width, height);
    rewindForwardsTexture = LoadTextureFromImage(_rewindForwardsIm);
    

    int playButtX = posX - (width / 2); // place play button in middle
    int seekBX = ( playButtX - 5 ) - width;
    int seekFX = ( playButtX ) + width + 5;


    // we add some padding buffers to all 
    // effective elements of button rectangles
    // to allow loose clicks

    float padding = 3;

    play = {playButtX - padding, posY + padding, width + padding, height + padding};
    seekBack = {seekBX, posY, width, height};
    seekForward = {seekFX, posY, width, height};

}

void PlayBackAddons::renderPlayBackAddons(M3WindowState *m3winstate){
    posX = m3winstate->WINDOW_WIDTH / 2;
    posY = ( m3winstate->WINDOW_HEIGHT - ((m3winstate->WINDOW_HEIGHT * 0.15) / 2) );

    playButtX = posX - (width / 2);
    int seekBX = ( playButtX - 5 ) - width;
    int seekFX = ( playButtX ) + width + 5;
     

    play = {playButtX, posY, width, height};
    seekBack = {seekBX, posY, width, height};
    seekForward = {seekFX, posY, width, height};
    
    if(!isPlaying){
    DrawTexture(playImTexture, playButtX, posY - (height / 4), GRAY); // normalize the height of play button by subtracting 1/4 of height from posY. this makes the position look more natural
    }
    else if(isPlaying){ 
        DrawTexture(pauseImTexture, playButtX, posY - (height / 4), GRAY); 
    }

    
    DrawTexture(rewindBackwardsTexture, seekBX, posY - (height / 4), GRAY);
    DrawTexture(rewindForwardsTexture, seekFX, posY - (height / 4), GRAY);

}

Event PlayBackAddons::checkEventTriggered(){
    if(CheckCollisionPointRec(GetMousePosition(), play) && IsMouseButtonReleased(MOUSE_LEFT_BUTTON)){
        if(isPlaying){
            isPlaying = false;
            return STATE_STOP_MUSIC;
        }
        else if(!isPlaying) {
            isPlaying = true;
            return STATE_PLAY_MUSIC;
        }
    }

    else if(CheckCollisionPointRec(GetMousePosition(), seekBack) && IsMouseButtonReleased(MOUSE_LEFT_BUTTON)){
        return SEEK_BACK;
    }

    else if(CheckCollisionPointRec(GetMousePosition(), seekForward) && IsMouseButtonReleased(MOUSE_LEFT_BUTTON)){
        return SEEK_AHEAD;
    }
    
    return STATE_NORMAL;
}

void PlayBackAddons::processEvent(Event ev){
    // won't do much good, since we need a music stream to mannipulate it.
    // Hence so This override remain a dummy
    ;
}


SplashScreenLogo::SplashScreenLogo(){ 
    width = 100;
    height = 100;
    posX = (m3winState.WINDOW_WIDTH / 2) - (width / 2);
    posY = (m3winState.WINDOW_HEIGHT / 2) - (height / 2);
    _imPlayerPath = "../res/icon.png"; 
    _imPlayerLogo = LoadImage(_imPlayerPath);
    ImageResize(&_imPlayerLogo, width, height);
    playerLogoTexture = LoadTextureFromImage(_imPlayerLogo);
}

void SplashScreenLogo::renderSplahScreenLogo(M3WindowState *m3winstate){
    posX = (m3winstate->WINDOW_WIDTH / 2) - (width / 2);
    posY = (m3winstate->WINDOW_HEIGHT / 2) - (height / 2);

    DrawTexture(playerLogoTexture, posX, posY, RAYWHITE);
}





