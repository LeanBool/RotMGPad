#ifndef GAMEPADREALM_MENUBARMODE_HPP
#define GAMEPADREALM_MENUBARMODE_HPP
#include "IInputMode.hpp"
#include "virtualinput/VirtualKeyboard.hpp"
#include "virtualinput/VirtualMouse.hpp"


class MenuBarMode : public IInputMode {
public:
    MenuBarMode(VirtualMouse* mouse, VirtualKeyboard* keyboard) : mouse_(mouse), keyboard_(keyboard) {};

    void updateCursor(Axes leftStick, Axes rightStick) override;

    void onDpadLeft() override;
    void onDpadRight() override;

    void onButtonA() override;

    std::string name() override { return  "Menu bar mode"; }
    MODE id() override { return menuBarMode; };
    int itemIndex() override { return itemIndex_; };
    void setItemIndex(int itemIndex) override { itemIndex_ = itemIndex; };
private:
    VirtualMouse* mouse_;
    VirtualKeyboard* keyboard_;
    int itemIndex_ = 0;
};

#endif //GAMEPADREALM_MENUBARMODE_HPP
