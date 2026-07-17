#ifndef GAMEPADREALM_BATTLEPASSMODE_HPP
#define GAMEPADREALM_BATTLEPASSMODE_HPP

#include "IInputMode.hpp"
#include "virtualinput/VirtualKeyboard.hpp"
#include "virtualinput/VirtualMouse.hpp"

class BattlePassMode : public IInputMode {
public:
    BattlePassMode(VirtualMouse* mouse, VirtualKeyboard* keyboard) : mouse_(mouse), keyboard_(keyboard) {};

    void updateCursor(Axes leftStick, Axes rightStick) override;

    void onButtonA() override;
    void onButtonX() override;
    void onModeEnter() override;
    void onModeExit() override;


    std::string name() override { return  "Battle pass mode"; }
    MODE id() override { return battlePassMode; };
    int itemIndex() override { return itemIndex_; };
    void setItemIndex(int itemIndex) override { itemIndex_ = itemIndex; };

private:
    VirtualMouse* mouse_;
    VirtualKeyboard* keyboard_;
    int itemIndex_ = 0;
};


#endif //GAMEPADREALM_BATTLEPASSMODE_HPP
