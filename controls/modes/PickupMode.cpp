#include "PickupMode.hpp"

#include "../../platform/PlatformSleep.hpp"

#include "../../globals.hpp"
#include "virtualinput/VirtualMouse.hpp"

void PickupMode::updateCursor(Axes leftStick, Axes rightStick) {
    int curX, curY;
    if (mouse_->getCursorPosition(curX, curY)) {
        mouse_->moveRelative(
            PICKUP_SLOT0_X - curX + itemIndex_ % 4 * PICKUP_SLOT_WIDTH,
            PICKUP_SLOT0_Y - curY + itemIndex_ / 4 * PICKUP_SLOT_HEIGHT
        );
    }
}

void PickupMode::onButtonA() {
    mouse_->tapLeftDouble();
    sleepUs(50000);
}

void PickupMode::onDpadUp() {
    itemIndex_ -= 4;
    if (itemIndex_ < 0) {
        itemIndex_ += 8;
    }
}

void PickupMode::onDpadDown() {
    itemIndex_ += 4;
    itemIndex_ %= 8;
}

void PickupMode::onDpadLeft() {
    itemIndex_ -= 1;
    if (itemIndex_ < 0) {
        itemIndex_ = 3;
    } else if (itemIndex_ == 3) {
        itemIndex_ = 7;
    }
}

void PickupMode::onDpadRight() {
    itemIndex_ += 1;
    if (itemIndex_ % 4 == 0) {
        itemIndex_ -= 4;
    }
    itemIndex_ %= 8;
}
