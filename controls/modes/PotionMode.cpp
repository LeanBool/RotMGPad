#include "PotionMode.hpp"

void PotionMode::updateCursor(Axes leftStick, Axes rightStick) {
    int curX, curY;
    if (mouse_->getCursorPosition(curX, curY)) {
        mouse_->moveRelative(
            POTION_RACK_SLOT0_X - curX + itemIndex_ % 2 * POTION_RACK_SLOT_OFFSET_X,
            POTION_RACK_SLOT0_Y - curY + itemIndex_ / 2 * POTION_RACK_SLOT_OFFSET_Y
        );
    }
}

void PotionMode::onButtonA() {
    mouse_->tapLeft();
}

void PotionMode::onButtonB() {
    IInputMode::onButtonB();
}

void PotionMode::onDpadUp() {
    itemIndex_ -= 2;
    if (itemIndex_ < 0) itemIndex_ = 8;;
}

void PotionMode::onDpadDown() {
    itemIndex_ += 2;
    if (itemIndex_ > 8) itemIndex_ = 8;
}

void PotionMode::onDpadLeft() {
    if (itemIndex_ % 2 == 0 && itemIndex_ < 8) {
        itemIndex_++;
    }
    else {
        itemIndex_--;
    }
}

void PotionMode::onDpadRight() {
    itemIndex_++;
    if (itemIndex_ > 8) itemIndex_ = 8;
}
