#ifndef GAMEPADREALM_TELEPORTMODE_HPP
#define GAMEPADREALM_TELEPORTMODE_HPP
#include "IInputMode.hpp"
#include "virtualinput/VirtualKeyboard.hpp"
#include "virtualinput/VirtualMouse.hpp"

class TeleportMode : public IInputMode {
public:
    TeleportMode(VirtualMouse* mouse, VirtualKeyboard* keyboard) : mouse_(mouse), keyboard_(keyboard) {}

    void updateCursor(Axes leftStick, Axes rightStick) override;

    void onButtonA() override;

    std::string name() override { return  "Teleport mode"; }
    MODE id() override { return teleportMode; };
private:
    VirtualMouse* mouse_;
    VirtualKeyboard* keyboard_;

    float cursorX_, cursorY_;
};


#endif //GAMEPADREALM_TELEPORTMODE_HPP
