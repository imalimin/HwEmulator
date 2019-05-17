//
// Created by mingyi.li on 2019/5/17.
//

#include "../include/HwGamePadEvent.h"

HwGamePadEvent *HwGamePadEvent::create(int key, int action) {
    return new HwGamePadEvent(key, action);
}

HwGamePadEvent::HwGamePadEvent(int key, int action) : Object() {
    this->key = key;
    this->action = action;
}

HwGamePadEvent::~HwGamePadEvent() {

}

int HwGamePadEvent::getKey() {
    return key;
}

int HwGamePadEvent::getAction() {
    return action;
}