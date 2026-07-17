#ifndef GAMEPADREALM_NORMALMODE_HPP
#define GAMEPADREALM_NORMALMODE_HPP
#include "IInputMode.hpp"
#include "virtualinput/VirtualKeyboard.hpp"
#include "virtualinput/VirtualMouse.hpp"


class NormalMode : public IInputMode {
public:
    NormalMode(VirtualMouse* mouse, VirtualKeyboard* keyboard) : mouse_(mouse), keyboard_(keyboard) {
        this->NormalMode::updateCursor({0.0, 0.0 }, {0.0, 0.0 });
    }
    void updateCursor(Axes leftStick, Axes rightStick) override;

    void onButtonA() override;
    void onButtonB() override;
    void onButtonX() override;
    void onButtonY() override;

    void onDpadRight() override;
    void onDpadLeft() override;
    void onDpadDown() override;

    void onTriggerLeft() override;
    void onTriggerRight() override;
    void onShoulderLeft() override;
    void onShoulderRight() override;

    std::string name() override { return  "Normal mode"; }
    MODE id() override { return normalMode; };
private:
    VirtualMouse* mouse_;
    VirtualKeyboard* keyboard_;
};


#endif //GAMEPADREALM_NORMALMODE_HPP
