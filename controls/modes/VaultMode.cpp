#include "VaultMode.hpp"

void VaultMode::updateCursor(Axes leftStick, Axes rightStick) {
    int curX, curY;
    if (mouse_->getCursorPosition(curX, curY)) {
        mouse_->moveRelative(
            VAULT_SLOT0_X - curX + itemIndex_ % 8 * INV_SLOT_WIDTH,
            VAULT_SLOT0_Y - curY + itemIndex_ / 8 * INV_SLOT_HEIGHT
        );
    }
}

void VaultMode::onButtonA() {
    mouse_->tapLeftDouble();
}

void VaultMode::onDpadUp() {
    itemIndex_ -= 8;
    if (itemIndex_ < 0) itemIndex_ += 8;
}

void VaultMode::onDpadDown() {
    itemIndex_ += 8;
}

void VaultMode::onDpadLeft() {
    itemIndex_ -= 1;
    if (itemIndex_ < 0) itemIndex_ = 0;
}

void VaultMode::onDpadRight() {
    itemIndex_ += 1;
}
