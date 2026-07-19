#include "MissionPreviewMode.hpp"

#include <algorithm>
#include "../../platform/PlatformSleep.hpp"

void MissionPreviewMode::updateCursor(Axes leftStick, Axes rightStick) {
    scrollPos_ += rightStick.y * ALPHA_CURSOR_SPEED;
    scrollPos_ = std::clamp(scrollPos_, 0.0f, 1.0f);

    int curX, curY;
    if (mouse_->getCursorPosition(curX, curY)) {
        mouse_->moveRelative(MISSION_PREVIEW_MODE_SCROLL_X - curX, MISSION_PREVIEW_MODE_SCROLL_Y - curY + scrollPos_ * MISSION_PREVIEW_MODE_SCROLL_HEIGHT);
        mouse_->tapLeft();
    }
}

void MissionPreviewMode::onButtonA() {
    mouse_->releaseLeft();
    int curX, curY;
    if (mouse_->getCursorPosition(curX, curY)) {
        mouse_->moveRelative(MISSION_MODE_SEE_ALL_X - curX, MISSION_MODE_SEE_ALL_Y - curY);
        sleepUs(250000);
        mouse_->tapLeft();
        sleepUs(250000);
    }
}

void MissionPreviewMode::onModeEnter() {
    sleepUs(250000);
}

void MissionPreviewMode::onModeExit() {
    mouse_->releaseLeft();
    int curX, curY;
    if (mouse_->getCursorPosition(curX, curY)) {
        mouse_->moveRelative(
            MENUBAR_X - curX + 3 * MENUBAR_WIDTH,
            MENUBAR_Y - curY
        );
        sleepUs(50000);
        mouse_->tapLeft();
        sleepUs(50000);
    }
}
