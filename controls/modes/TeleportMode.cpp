#include "TeleportMode.hpp"

#include <algorithm>

#include "../../globals.hpp"
#include "../../platform/PlatformSleep.hpp"

void TeleportMode::updateCursor(Axes leftStick, Axes rightStick) {
    cursorX_ += leftStick.x * ALPHA_CURSOR_SPEED;
    cursorX_ = std::clamp(cursorX_, -1.0f, 1.0f);
    cursorY_ += leftStick.y * ALPHA_CURSOR_SPEED;
    cursorY_ = std::clamp(cursorY_, -1.0f, 1.0f);

    int curX, curY;
    if (mouse_->getCursorPosition(curX, curY)) {
        mouse_->moveRelative(
            MAP_CENTER_X - curX + cursorX_ * MAP_RADIUS,
            MAP_CENTER_Y - curY + cursorY_ * MAP_RADIUS
        );
    }
}

void TeleportMode::onButtonA() {
    mouse_->tapLeft();
    sleepUs(150000);
    mouse_->moveRelative(MAP_TELEPORT_BUTTON_OFFSET_X, MAP_TELEPORT_BUTTON_OFFSET_Y);
    sleepUs(150000);
    mouse_->tapLeftDouble();
}
