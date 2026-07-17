#ifndef GAMEPADREALM_PETINVENTORYMODE_HPP
#define GAMEPADREALM_PETINVENTORYMODE_HPP
#include "IInputMode.hpp"
#include "virtualinput/VirtualKeyboard.hpp"
#include "virtualinput/VirtualMouse.hpp"


class PetInventoryMode : public IInputMode {
public:
    PetInventoryMode(VirtualMouse* mouse, VirtualKeyboard* keyboard) : mouse_(mouse), keyboard_(keyboard) {};

    void updateCursor(Axes leftStick, Axes rightStick) override;

    void onDpadLeft() override;
    void onDpadRight() override;

    std::string name() override { return  "Pet inventory mode"; }
    MODE id() override { return petInventoryMode; };
    int itemIndex() override { return itemIndex_; };
    void setItemIndex(int itemIndex) override { itemIndex_ = itemIndex; };

    void onModeExit() override;
private:
    VirtualMouse* mouse_;
    VirtualKeyboard* keyboard_;
    int itemIndex_ = 0;
};

#endif //GAMEPADREALM_PETINVENTORYMODE_HPP
