#ifndef GAMEPADREALM_POTIONMODE_HPP
#define GAMEPADREALM_POTIONMODE_HPP

#include "IInputMode.hpp"
#include "virtualinput/VirtualKeyboard.hpp"
#include "virtualinput/VirtualMouse.hpp"

class PotionMode : public IInputMode {
public:
    PotionMode(VirtualMouse* mouse, VirtualKeyboard* keyboard) : mouse_(mouse), keyboard_(keyboard) {};

    void updateCursor(Axes leftStick, Axes rightStick) override;
    void onButtonA() override;
    void onButtonB() override;
    void onDpadUp() override;
    void onDpadDown() override;
    void onDpadLeft() override;
    void onDpadRight() override;

    std::string name() override { return  "Potion mode"; }
    MODE id() override { return potionMode; };
    int itemIndex() override { return itemIndex_; };
private:
    VirtualMouse* mouse_;
    VirtualKeyboard* keyboard_;
    int itemIndex_ = 0;
};


#endif //GAMEPADREALM_POTIONMODE_HPP
