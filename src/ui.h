#ifndef UI_H_
#define UI_H_

#include <iostream>
#include "raylib.h"
#include "window_state.h"


enum Event { 
    STATE_NORMAL,         
    STATE_HOVER_TOOLBAR_BUTTON,
    STATE_CLICK_OPENFILE,
    STATE_CHOOSE_VOLUME,
    STATE_PLAY_MUSIC,
    STATE_STOP_MUSIC,
    SEEK_BACK,
    SEEK_AHEAD,
};

class ControlsPosition{
    protected:
        float posX;
        float posY;
        float width;
        float height;
};


class EventTrigger{
    public:
        virtual Event checkEventTriggered() = 0;
        virtual void processEvent(Event) = 0;
};

class ToolBar: public EventTrigger, public ControlsPosition{
    private:
        Rectangle toolbarRec, openFileButton;
        bool isButtonBeingHovered = false;
        char* openButtonText;
        Color defaultColor;
    public:
        ToolBar();
        Event checkEventTriggered() override;
        void processEvent(Event) override;
        void renderToolBar(M3WindowState *m3winstate);
        
};

class BaseBar: public EventTrigger, ControlsPosition{
    private:
        Rectangle baseBarRec;
        Color defaultColor;
    public:
        BaseBar();
        Event checkEventTriggered() override;
        void processEvent(Event) override;
        void renderBaseBar(M3WindowState *m3winstate);
};

class TimeLine: public EventTrigger, ControlsPosition{
    private:
        int durationStrSizeSingle, durationStrSizeDouble, posXCurrentlyPLayed, posXTotalLength;
        float currentlyPlayedFloatTime, totalLengthFloatTime; 
        char currentlyPlayedTime[100], totalLengthTime[100]; 
        Rectangle timeLineRecEmpty, timeLineRecPlaying;
        Color emptyColor, playingColor;
    public:
        TimeLine();
        void renderTimeLine(M3WindowState *m3winstate, float timePlayed, float currMusicTime, float totalMusicTime);
        void secsToMinSecs(float secs, char secMins[100]);
        Event checkEventTriggered() override;
        void processEvent(Event) override;
};

class VolumeControl: public EventTrigger, public ControlsPosition{
    private:
        Rectangle volume[5];
        char* volText;
        int volumeBarCurrIdx, tmpVolumeIdxStore, padding;
        Color Colored, unColored;
    public:
        VolumeControl();
        void renderVolumeControl(M3WindowState *m3winstate);
        int getVolumeBar();
        Event checkEventTriggered() override;
        void processEvent(Event) override;
};


class PlayBackAddons: public EventTrigger, ControlsPosition{
    private:
        Rectangle play, seekBack, seekForward;
        const char *playImPath, *pauseImPath, *rewindBackwardsImPath, *rewindForwardsImPath;
        Image _playIm, _pauseIm, _rewindBackwardsIm , _rewindForwardsIm;
        Texture2D playImTexture, pauseImTexture, rewindBackwardsTexture, rewindForwardsTexture;
        int seekBX, seekFX, playButtX; // x pos for playback buttons
    public:
        bool isPlaying = false;
        PlayBackAddons();
        void renderPlayBackAddons(M3WindowState *m3winstate);
        Event checkEventTriggered() override;
        void processEvent(Event) override;
};



class SplashScreenLogo: public ControlsPosition{
    private:
        Image _imPlayerLogo;
        char* _imPlayerPath;
        Texture2D playerLogoTexture;
    public:
        SplashScreenLogo();
        void renderSplahScreenLogo(M3WindowState *m3winstate);
};


#endif // UI_H_
