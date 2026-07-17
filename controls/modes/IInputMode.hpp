#ifndef GAMEPADREALM_IACTIONMODE_H
#define GAMEPADREALM_IACTIONMODE_H
#include <string>
#include "../../globals.hpp"

struct Axes {
    float x;
    float y;
};

class IInputMode {
public:
    virtual ~IInputMode() = default;

    virtual void updateCursor(Axes leftStick, Axes rightStick) {}

    virtual void onButtonA() {}
    virtual void onButtonB() {}
    virtual void onButtonX() {}
    virtual void onButtonY() {}

    virtual void onTriggerLeft() {}
    virtual void onTriggerRight() {}
    virtual void onShoulderLeft() {}
    virtual void onShoulderRight() {}

    virtual void onDpadUp() {}
    virtual void onDpadDown() {}
    virtual void onDpadLeft() {}
    virtual void onDpadRight() {}

    virtual void onModeEnter() {}
    virtual void onModeExit() {}

    virtual std::string name() {
        return "IInputMode";
    }

    [[nodiscard]] virtual MODE id() = 0;
    virtual int itemIndex() { return 0; }
    virtual void setItemIndex(int itemIndex) {}
};

#endif //GAMEPADREALM_IACTIONMODE_H
