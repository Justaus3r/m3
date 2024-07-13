#ifndef M3_H_
#define M3_H_

#include "error_codes.h"
#include "window_state.h"

class M3{
    public:
        M3();
        void setUpdatedWindowSize(M3WindowState*);
        RetCode startPlayerEventLoop();
};

#endif //M3_H_
