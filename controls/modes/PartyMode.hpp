#ifndef GAMEPADREALM_PARTYMODE_H
#define GAMEPADREALM_PARTYMODE_H
#include "../../virtualinput/Keycodes.hpp"

#include "IInputMode.hpp"
#include "virtualinput/VirtualKeyboard.hpp"
#include "virtualinput/VirtualMouse.hpp"


class PartyMode : public IInputMode {
public:
    PartyMode(VirtualMouse* mouse, VirtualKeyboard* keyboard);

    void updateCursor(Axes leftStick, Axes rightStick) override;

    void onButtonA() override;
    void onButtonB() override;
    void onButtonX() override;


    std::string name() override { return  "Party mode"; }
    MODE id() override { return partyMode; };
private:
    VirtualMouse* mouse_;
    VirtualKeyboard* keyboard_;

    int chatIndex_ = 0;
    float cursorX_, cursorY_;
};


#endif //GAMEPADREALM_PARTYMODE_H
