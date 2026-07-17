#ifndef GAMEPADREALM_MISSIONMODE_HPP
#define GAMEPADREALM_MISSIONMODE_HPP
#include <vector>

#include "IInputMode.hpp"
#include "virtualinput/VirtualKeyboard.hpp"
#include "virtualinput/VirtualMouse.hpp"

class MissionMode : public IInputMode {
public:
    MissionMode(VirtualMouse* mouse, VirtualKeyboard* keyboard) : mouse_(mouse), keyboard_(keyboard) {};

    void updateCursor(Axes leftStick, Axes rightStick) override;

    void onButtonA() override;

    std::string name() override { return  "Mission mode"; }
    MODE id() override { return missionMode; };
    int itemIndex() override { return itemIndex_; };
    void setItemIndex(int itemIndex) override { itemIndex_ = itemIndex; };

    void onModeEnter() override;
    void onModeExit() override;
private:
    VirtualMouse* mouse_;
    VirtualKeyboard* keyboard_;
    int itemIndex_ = 0;
    float cursorX_, cursorY_;
};

#endif //GAMEPADREALM_MISSIONMODE_HPP
