#include "controls/modes/PartyMode.hpp"

#include <algorithm>
#include "../../platform/PlatformSleep.hpp"
#include "../../virtualinput/Keycodes.hpp"
#include "../../globals.hpp"

PartyMode::PartyMode(VirtualMouse *mouse, VirtualKeyboard *keyboard) {
    mouse_ = mouse;
    keyboard_ = keyboard;
}

void PartyMode::updateCursor(Axes leftStick, Axes rightStick) {
    cursorX_ += leftStick.x * ALPHA_CURSOR_SPEED;
    cursorX_ = std::clamp(cursorX_, -1.0f, 1.0f);
    cursorY_ += leftStick.y * ALPHA_CURSOR_SPEED;
    cursorY_ = std::clamp(cursorY_, -1.0f, 0.0f);
    int curX, curY;
    if (mouse_->getCursorPosition(curX, curY)) {
        mouse_->moveRelative(
            CHAT_X - curX + cursorX_ * CHAT_WIDTH / 2,
            CHAT_Y - curY + cursorY_ * CHAT_HEIGHT
        );
    }
}

void PartyMode::onButtonA() {
    keyboard_->press(KEY_LEFTCTRL);
    mouse_->tapLeft();
    sleepUs(150000);
    mouse_->moveRelative(CHAT_JOIN_BUTTON_OFFSET_X, CHAT_JOIN_BUTTON_OFFSET_Y);
    sleepUs(150000);
    mouse_->tapLeft();
    sleepUs(50000);
    mouse_->tapLeft();
    keyboard_->release(KEY_LEFTCTRL);
}

void PartyMode::onButtonB() {
    keyboard_->press(KEY_LEFTCTRL);
    mouse_->tapLeft();
    sleepUs(150000);
    mouse_->moveRelative(CHAT_TELEPORT_BUTTON_OFFSET_X, CHAT_TELEPORT_BUTTON_OFFSET_Y);
    sleepUs(150000);
    mouse_->tapLeft();
    sleepUs(50000);
    mouse_->tapLeft();
    keyboard_->release(KEY_LEFTCTRL);
}

void PartyMode::onButtonX() {
    keyboard_->tap(KEY_ENTER);
}

