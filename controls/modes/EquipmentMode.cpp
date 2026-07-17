#include "EquipmentMode.hpp"

void EquipmentMode::updateCursor(Axes leftStick, Axes rightStick) {
    int curX, curY;
    if (mouse_->getCursorPosition(curX, curY)) {
        mouse_->moveRelative(
            EQUIPMENT_SLOT0_X - curX + itemIndex_ * EQUIPMENT_SLOT_WIDTH,
            EQUIPMENT_SLOT0_Y - curY
        );
    }
}

void EquipmentMode::onDpadLeft() {
    itemIndex_--;
    if (itemIndex_ < 0) itemIndex_ = 3;
}

void EquipmentMode::onDpadRight() {
    itemIndex_++;
    if (itemIndex_ > 3) itemIndex_ = 0;
}
