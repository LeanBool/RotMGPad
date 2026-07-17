#ifndef GAMEPADREALM_VAULTMODE_HPP
#define GAMEPADREALM_VAULTMODE_HPP
#include "IInputMode.hpp"
#include "virtualinput/VirtualKeyboard.hpp"
#include "virtualinput/VirtualMouse.hpp"
#include "../../globals.hpp"

class VaultMode : public IInputMode {
public:
    VaultMode(VirtualMouse* mouse, VirtualKeyboard* keyboard) : mouse_(mouse), keyboard_(keyboard) {};

    void updateCursor(Axes leftStick, Axes rightStick) override;
    void onButtonA() override;

    void onDpadUp() override;
    void onDpadDown() override;
    void onDpadLeft() override;
    void onDpadRight() override;

    std::string name() override { return  "Vault mode"; }
    MODE id() override { return vaultMode; };
    int itemIndex() override { return itemIndex_; };
private:
    VirtualMouse* mouse_;
    VirtualKeyboard* keyboard_;
    int itemIndex_ = 0;
};

#endif //GAMEPADREALM_VAULTMODE_HPP
