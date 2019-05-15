/*
 * Copyright (c) 2018-present, lmyooyo@gmail.com.
 *
 * This source code is licensed under the GPL license found in the
 * LICENSE file in the root directory of this source tree.
 */
#include "../include/Size.h"

Size::Size() {
    this->width = 0;
    this->height = 0;
};

Size::Size(int w, int h) {
    this->width = w;
    this->height = h;
};

Size::~Size() {
};
