#include "InventoryMode.hpp"
#include "../../globals.hpp"
#include "../../platform/PlatformSleep.hpp"


void InventoryMode::onButtonA() {
    mouse_->tapLeftDouble();
}

void InventoryMode::onButtonB() {
}

void InventoryMode::onButtonX() {
    dropItem();
}

void InventoryMode::onButtonY() {
    moveToDropInventory();
}

void InventoryMode::onShoulderRight() {
    moveToExtraSlot();
}

void InventoryMode::updateCursor(Axes leftStick, Axes rightStick) {
    int curX, curY;
    if (mouse_->getCursorPosition(curX, curY)) {
        mouse_->moveRelative(
            INV_SLOT0_X - curX + itemIndex_ % 4 * INV_SLOT_WIDTH,
            INV_SLOT0_Y - curY + itemIndex_ / 4 * INV_SLOT_HEIGHT
        );
    }
}

void InventoryMode::onDpadUp() {
    itemIndex_ -= 4;
    if (itemIndex_ < 0) {
        itemIndex_ += 8;
    }
}

void InventoryMode::onDpadDown() {
    itemIndex_ += 4;
    itemIndex_ %= 8;
}

void InventoryMode::onDpadLeft() {
    itemIndex_ -= 1;
    if (itemIndex_ < 0) {
        itemIndex_ = 3;
    } else if (itemIndex_ == 3) {
        itemIndex_ = 7;
    }
}

void InventoryMode::onDpadRight() {
    itemIndex_ += 1;
    if (itemIndex_ % 4 == 0) {
        itemIndex_ -= 4;
    }
    itemIndex_ %= 8;
}

void InventoryMode::moveToDropInventory() {
    mouse_->pressLeft();
    sleepUs(100000);
    mouse_->moveRelative(0, 300);
    sleepUs(100000);
    mouse_->releaseLeft();
    sleepUs(100000);
}
void InventoryMode::dropItem() {
    mouse_->pressLeft();
    sleepUs(100000);
    mouse_->moveRelative(-600, 0);
    sleepUs(100000);
    mouse_->releaseLeft();
    sleepUs(100000);
}
void InventoryMode::moveToExtraSlot() {
    mouse_->pressLeft();
    sleepUs(100000);

    int curX, curY;
    if (mouse_->getCursorPosition(curX, curY)) {
        mouse_->moveRelative(
            EXTRA_SLOT0_X - curX,
            EXTRA_SLOT0_Y - curY
        );
    }

    sleepUs(100000);
    mouse_->releaseLeft();
    sleepUs(100000);
}
