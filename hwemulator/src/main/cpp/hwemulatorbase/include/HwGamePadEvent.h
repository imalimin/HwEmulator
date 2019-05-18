//
// Created by mingyi.li on 2019/5/17.
//

#ifndef HWEMULATOR_HWGAMEPADEVENT_H
#define HWEMULATOR_HWGAMEPADEVENT_H

#include "../../common/include/Object.h"

class HwGamePadEvent : public Object {
public:
    static HwGamePadEvent *create(int key, int action);

    virtual ~HwGamePadEvent();

    int getKey();

    int getAction();

private:
    HwGamePadEvent(int key, int action);

    void toString();

public:
    static const int KEY_NONE = 0x000;
    static const int KEY_RIGHT = 0x001;
    static const int KEY_LEFT = 0x002;
    static const int KEY_UP = 0x003;
    static const int KEY_DOWN = 0x004;
    static const int KEY_SELECT = 0x005;
    static const int KEY_START = 0x006;
    static const int KEY_A = 0x007;
    static const int KEY_B = 0x008;
    static const int KEY_ESC = 0x009;
    static const int ACTION_NONE = 0x000;
    static const int ACTION_DOWN = 0x101;
    static const int ACTION_UP = 0x102;
private:
    int key = KEY_NONE;
    int action = ACTION_NONE;
};


#endif //HWEMULATOR_HWGAMEPADEVENT_H
