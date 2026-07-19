#ifndef GAMEPADREALM_MISSIONPREVIEWMODE_HPP
#define GAMEPADREALM_MISSIONPREVIEWMODE_HPP
#include "IInputMode.hpp"
#include "virtualinput/VirtualKeyboard.hpp"
#include "virtualinput/VirtualMouse.hpp"


class MissionPreviewMode : public IInputMode {
public:
    MissionPreviewMode(VirtualMouse* mouse, VirtualKeyboard* keyboard) : mouse_(mouse), keyboard_(keyboard) {};

    void updateCursor(Axes leftStick, Axes rightStick) override;

    void onButtonA() override;

    std::string name() override { return  "Mission preview mode"; }
    MODE id() override { return missionPreviewMode; };
    int itemIndex() override { return itemIndex_; };
    void setItemIndex(int itemIndex) override { itemIndex_ = itemIndex; };

    void onModeEnter() override;
    void onModeExit() override;
private:
    VirtualMouse* mouse_;
    VirtualKeyboard* keyboard_;
    int itemIndex_ = 0;

    float scrollPos_ = 0.0f;
};


#endif //GAMEPADREALM_MISSIONPREVIEWMODE_HPP
