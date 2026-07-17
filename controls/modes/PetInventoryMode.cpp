#include "PetInventoryMode.hpp"

void PetInventoryMode::updateCursor(Axes leftStick, Axes rightStick) {int curX, curY;
    if (mouse_->getCursorPosition(curX, curY)) {
        mouse_->moveRelative(
            PET_INVENTORY_X - curX + itemIndex_ * EQUIPMENT_SLOT_WIDTH,
            PET_INVENTORY_Y - curY
        );
    }
}

void PetInventoryMode::onDpadLeft() {
    itemIndex_--;
    if (itemIndex_ < 0) itemIndex_ = 3;
}

void PetInventoryMode::onDpadRight() {
    itemIndex_++;
}

void PetInventoryMode::onModeExit() {
    int curX, curY;
    if (mouse_->getCursorPosition(curX, curY)) {
        mouse_->moveRelative(
            MENUBAR_X - curX + 1 * MENUBAR_WIDTH,
            MENUBAR_Y - curY
        );
        mouse_->tapLeft();
    }

}
