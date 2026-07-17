#ifndef GAMEPADREALM_INVENTORYMODE_H
#define GAMEPADREALM_INVENTORYMODE_H

#include "IInputMode.hpp"
#include "virtualinput/VirtualKeyboard.hpp"
#include "virtualinput/VirtualMouse.hpp"

class InventoryMode : public IInputMode {
public:
    InventoryMode(VirtualMouse* mouse, VirtualKeyboard* keyboard, int itemIndex = 0) : mouse_(mouse), keyboard_(keyboard), itemIndex_(itemIndex) {};

    void updateCursor(Axes leftStick, Axes rightStick) override;

    void onButtonA() override;
    void onButtonB() override;

    void onButtonX() override;
    void onButtonY() override;

    void onDpadUp() override;
    void onDpadDown() override;
    void onDpadLeft() override;
    void onDpadRight() override;

    void onShoulderRight() override;

    std::string name() override { return  "Inventory mode"; }
    MODE id() override { return inventoryMode; };
    int itemIndex() override { return itemIndex_; };
    void setItemIndex(int itemIndex) override { itemIndex_ = itemIndex; };
private:
    void dropItem();
    void moveToExtraSlot();
    void moveToDropInventory();
    VirtualMouse* mouse_;
    VirtualKeyboard* keyboard_;
    int itemIndex_ = 0;
};


#endif //GAMEPADREALM_INVENTORYMODE_H
