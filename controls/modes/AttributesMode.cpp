#include "AttributesMode.hpp"

#include <algorithm>
#include "../../platform/PlatformSleep.hpp"

void AttributesMode::updateCursor(Axes leftStick, Axes rightStick) {
    scrollPos_ += rightStick.y * ALPHA_CURSOR_SPEED;
    scrollPos_ = std::clamp(scrollPos_, 0.0f, 1.0f);

    int curX, curY;
    if (mouse_->getCursorPosition(curX, curY)) {
        mouse_->moveRelative(ATTRIBUTES_SCROLL_X - curX, ATTRIBUTES_SCROLL_Y - curY + scrollPos_ * ATTRIBUTES_SCROLL_HEIGHT);
        mouse_->tapLeft();
    }
}

void AttributesMode::onShoulderLeft() {
    mouse_->releaseLeft();
    int curX, curY;
    if (mouse_->getCursorPosition(curX, curY)) {
        mouse_->moveRelative(ATTRIBUTES_STATS_TAB_X - curX, ATTRIBUTES_STATS_TAB_Y - curY);
        sleepUs(150000);
        mouse_->tapLeft();
        sleepUs(150000);
        scrollPos_ = 0.0f;
    }
}

void AttributesMode::onShoulderRight() {
    mouse_->releaseLeft();
    int curX, curY;
    if (mouse_->getCursorPosition(curX, curY)) {
        mouse_->moveRelative(ATTRIBUTES_DUNGEONS_TAB_X - curX, ATTRIBUTES_DUNGEONS_TAB_Y - curY);
        sleepUs(150000);
        mouse_->tapLeft();
        sleepUs(150000);
        scrollPos_ = 0.0f;
    }
}

void AttributesMode::onModeEnter() {
    sleepUs(100000);
}

void AttributesMode::onModeExit() {
    mouse_->releaseLeft();
    int curX, curY;
    if (mouse_->getCursorPosition(curX, curY)) {
        mouse_->moveRelative(
            MENUBAR_X - curX + 0 * MENUBAR_WIDTH,
            MENUBAR_Y - curY
        );
        sleepUs(50000);
        mouse_->tapLeft();
        sleepUs(50000);
    }
}
