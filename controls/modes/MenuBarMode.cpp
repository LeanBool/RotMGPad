#include "MenuBarMode.hpp"

#include "../../platform/PlatformSleep.hpp"

void MenuBarMode::updateCursor(Axes leftStick, Axes rightStick) {
    int curX, curY;
    if (mouse_->getCursorPosition(curX, curY)) {
        mouse_->moveRelative(
            MENUBAR_X - curX + itemIndex_ * MENUBAR_WIDTH,
            MENUBAR_Y - curY
        );
    }
}

void MenuBarMode::onDpadLeft() {
    itemIndex_--;
    if (itemIndex_ < 0) itemIndex_ = 3;
}

void MenuBarMode::onDpadRight() {
    itemIndex_++;
    if (itemIndex_ > 3) itemIndex_ = 0;
}

void MenuBarMode::onButtonA() {
    mouse_->tapLeft();
    sleepUs(50000);
}
