#ifndef GAMEPADREALM_EQUIPMENTMODE_HPP
#define GAMEPADREALM_EQUIPMENTMODE_HPP
#include "IInputMode.hpp"
#include "virtualinput/VirtualKeyboard.hpp"
#include "virtualinput/VirtualMouse.hpp"

class EquipmentMode : public IInputMode {
public:
    EquipmentMode(VirtualMouse* mouse, VirtualKeyboard* keyboard) : mouse_(mouse), keyboard_(keyboard) {};

    void updateCursor(Axes leftStick, Axes rightStick) override;

    void onDpadLeft() override;
    void onDpadRight() override;

    std::string name() override { return  "Equipment mode"; }
    MODE id() override { return equipmentMode; };
    int itemIndex() override { return itemIndex_; };
    void setItemIndex(int itemIndex) override { itemIndex_ = itemIndex; };
private:
    VirtualMouse* mouse_;
    VirtualKeyboard* keyboard_;
    int itemIndex_ = 0;
};

#endif //GAMEPADREALM_EQUIPMENTMODE_HPP
