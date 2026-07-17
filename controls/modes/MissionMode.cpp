#include "MissionMode.hpp"

#include <algorithm>
#include "../../platform/PlatformSleep.hpp"

void MissionMode::updateCursor(Axes leftStick, Axes rightStick) {
    cursorX_ -= leftStick.x * ALPHA_CURSOR_SPEED;
    cursorX_ = std::clamp(cursorX_, -1.0f, 1.0f);
    cursorY_ -= leftStick.y * ALPHA_CURSOR_SPEED;
    cursorY_ = std::clamp(cursorY_, -1.0f, 1.0f);

    int curX, curY;
    if (mouse_->getCursorPosition(curX, curY)) {
        mouse_->moveRelative(
            SCREEN_WIDTH / 2 - curX - cursorX_ * SCREEN_WIDTH / 2,
            SCREEN_HEIGHT / 2 - curY - cursorY_ * SCREEN_HEIGHT / 2
        );
    }
}

void MissionMode::onButtonA() {
    mouse_->tapLeft();
}

void MissionMode::onModeEnter() {
    int curX, curY;
    if (mouse_->getCursorPosition(curX, curY)) {
        mouse_->moveRelative(MISSION_MODE_SEE_ALL_X - curX, MISSION_MODE_SEE_ALL_Y - curY);
        sleepUs(250000);
        mouse_->tapLeft();
        sleepUs(250000);
    }
}

void MissionMode::onModeExit() {
    int curX, curY;
    if (mouse_->getCursorPosition(curX, curY)) {
        mouse_->moveRelative(MISSION_MODE_CLOSE_X - curX, MISSION_MODE_CLOSE_Y - curY);
        sleepUs(500000);
        mouse_->tapLeftDouble();
        sleepUs(500000);
    }
}
