#ifndef GAMEPADREALM_PICKUPMODE_HPP
#define GAMEPADREALM_PICKUPMODE_HPP
#include "IInputMode.hpp"
#include "virtualinput/VirtualKeyboard.hpp"


class VirtualMouse;

class PickupMode : public IInputMode {
public:
    PickupMode(VirtualMouse* mouse, VirtualKeyboard* keyboard) : mouse_(mouse), keyboard_(keyboard) {};

    void updateCursor(Axes leftStick, Axes rightStick) override;
    void onButtonA() override;
    void onDpadUp() override;
    void onDpadDown() override;
    void onDpadLeft() override;
    void onDpadRight() override;

    std::string name() override { return  "Pickup mode"; }
    MODE id() override { return pickupMode; };
    int itemIndex() override { return itemIndex_; };
    void setItemIndex(int itemIndex) override { itemIndex_ = itemIndex; };
private:
    VirtualMouse* mouse_;
    VirtualKeyboard* keyboard_;
    int itemIndex_ = 0;
};

#endif //GAMEPADREALM_PICKUPMODE_HPP
