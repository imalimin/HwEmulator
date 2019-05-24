//
// Created by mingyi.li on 2019/5/17.
//

#include "../include/HwGamePadEvent.h"
#include "Logcat.h"

HwGamePadEvent *HwGamePadEvent::create(int key, int action) {
    return new HwGamePadEvent(key, action);
}

HwGamePadEvent::HwGamePadEvent(int key, int action) : Object() {
    this->key = key;
    this->action = action;
//    toString();
}

HwGamePadEvent::~HwGamePadEvent() {

}

int HwGamePadEvent::getKey() {
    return key;
}

int HwGamePadEvent::getAction() {
    return action;
}

void HwGamePadEvent::toString() {
    string key;
    string action;
    switch (this->key) {
        case KEY_UP:
            key = "UP";
            break;
        case KEY_DOWN:
            key = "DOWN";
            break;
        case KEY_LEFT:
            key = "LEFT";
            break;
        case KEY_RIGHT:
            key = "RIGHT";
            break;
        case KEY_SELECT:
            key = "SELECT";
            break;
        case KEY_START:
            key = "START";
            break;
        case KEY_A:
            key = "A";
            break;
        case KEY_B:
            key = "B";
            break;
        case KEY_NONE:
        default:
            key = "NONE";
    }
    switch (this->action) {
        case ACTION_UP:
            action = "UP";
            break;
        case ACTION_DOWN:
            action = "DOWN";
            break;
        case ACTION_NONE:
        default:
            action = "NONE";
    }
    Logcat::i("HWEMULATOR", "key=%s, action=%s", key.c_str(), action.c_str());
}