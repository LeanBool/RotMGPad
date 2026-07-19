#ifndef GAMEPADREALM_ATTRIBUTESMODE_HPP
#define GAMEPADREALM_ATTRIBUTESMODE_HPP
#include "IInputMode.hpp"
#include "virtualinput/VirtualKeyboard.hpp"
#include "virtualinput/VirtualMouse.hpp"


class AttributesMode : public IInputMode {
public:
    AttributesMode(VirtualMouse* mouse, VirtualKeyboard* keyboard) : mouse_(mouse), keyboard_(keyboard) {};

    void updateCursor(Axes leftStick, Axes rightStick) override;

    void onShoulderLeft() override;
    void onShoulderRight() override;

    std::string name() override { return  "Attributes mode"; }
    MODE id() override { return attributesMode; };
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



#endif //GAMEPADREALM_ATTRIBUTESMODE_HPP
