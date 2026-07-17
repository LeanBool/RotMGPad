#include "../../platform/PlatformSleep.hpp"

#include "BattlePassMode.hpp"

void BattlePassMode::updateCursor(Axes leftStick, Axes rightStick) {
    const int targetX = static_cast<int>(SCREEN_CENTER_X + leftStick.x * SCREEN_WIDTH / 2);
    const int targetY = static_cast<int>(SCREEN_CENTER_Y + leftStick.y * SCREEN_HEIGHT / 2);

    int curX, curY;
    if (mouse_->getCursorPosition(curX, curY)) {
        const int dx = targetX - curX;
        const int dy = targetY - curY;

        mouse_->moveRelative(dx * ALPHA_CURSOR_SPEED, dy * ALPHA_CURSOR_SPEED);
    }
}

void BattlePassMode::onButtonA() {
    mouse_->tapLeft();
}

void BattlePassMode::onButtonX() {
    int curX, curY;
    if (mouse_->getCursorPosition(curX, curY)) {
        mouse_->moveRelative(BATTLE_PASS_CLAIM_ALL_X - curX, BATTLE_PASS_CLAIM_ALL_Y - curY);
        sleepUs(50000);
        mouse_->tapLeft();
        sleepUs(1500000);
        mouse_->getCursorPosition(curX, curY);
        mouse_->moveRelative(BATTLE_PASS_CLOSE_POPUP_X - curX, BATTLE_PASS_CLOSE_POPUP_Y - curY);
        sleepUs(50000);
        mouse_->tapLeft();
        sleepUs(100000);
    }
}

void BattlePassMode::onModeEnter() {
    int curX, curY;
    if (mouse_->getCursorPosition(curX, curY)) {
        mouse_->moveRelative(BATTLE_PASS_X - curX, BATTLE_PASS_Y - curY);
        sleepUs(250000);
        mouse_->tapLeft();
        sleepUs(250000);
    }
}

void BattlePassMode::onModeExit() {
    int curX, curY;
    if (mouse_->getCursorPosition(curX, curY)) {
        mouse_->moveRelative(BATTLE_PASS_CLOSE_X - curX, BATTLE_PASS_CLOSE_Y - curY);
        sleepUs(250000);
        mouse_->tapLeft();
        sleepUs(250000);
    }
}
