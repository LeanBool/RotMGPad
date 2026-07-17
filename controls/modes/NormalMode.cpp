#include "NormalMode.hpp"

#include "../../virtualinput/Keycodes.hpp"
#include "../../globals.hpp"

void NormalMode::updateCursor(Axes leftStick, Axes rightStick) {
    const int targetX = static_cast<int>(CENTER_X + rightStick.x * RADIUS);
    const int targetY = static_cast<int>(CENTER_Y + rightStick.y * RADIUS);

    int curX, curY;
    if (mouse_->getCursorPosition(curX, curY)) {
        const int dx = targetX - curX;
        const int dy = targetY - curY;

        mouse_->moveRelative(dx * ALPHA_CURSOR_SPEED, dy * ALPHA_CURSOR_SPEED);
    }
}

void NormalMode::onButtonA() {
    keyboard_->tap(KEY_SPACE);
    mouse_->tapLeft();
}

void NormalMode::onButtonB() {
    keyboard_->tap(KEY_V);
}

void NormalMode::onButtonX() {
    keyboard_->tap(KEY_R);
}

void NormalMode::onButtonY() {
    keyboard_->tap(KEY_I);
}

void NormalMode::onDpadRight() {
    keyboard_->tap(KEY_B);
}

void NormalMode::onDpadLeft() {
    keyboard_->tap(KEY_X);
}

void NormalMode::onDpadDown() {
    keyboard_->tap(KEY_Z);
}

void NormalMode::onTriggerLeft() {
    IInputMode::onTriggerLeft();
}

void NormalMode::onTriggerRight() {
    IInputMode::onTriggerRight();
}

void NormalMode::onShoulderLeft() {
    keyboard_->tap(KEY_F);
}

void NormalMode::onShoulderRight() {
    mouse_->tapRight();
}
