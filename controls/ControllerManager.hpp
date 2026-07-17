//
// Created by alex on 7/8/26.
//

#ifndef GAMEPADREALM_CONTROLLERMANAGER_H
#define GAMEPADREALM_CONTROLLERMANAGER_H
#include <random>
#include <array>
#include <memory>
#include <SDL2/SDL_gamecontroller.h>

#include "virtualinput/VirtualKeyboard.hpp"
#include <SDL2/SDL_events.h>

#include "modes/IInputMode.hpp"
#include "virtualinput/VirtualMouse.hpp"

class ControllerManager {
public:
    ControllerManager(SDL_GameController *controller, VirtualKeyboard *keyboard, VirtualMouse *mouse);

    ~ControllerManager();

    // Constructs a mode instance without needing the concrete type at the
    // call site, so mode transitions can be stored as plain data (see
    // kButtonTransitions / kModeFactories in ControllerManager.cpp).
    using ModeFactory = std::unique_ptr<IInputMode> (*)(VirtualMouse*, VirtualKeyboard*);

    void updateKeyboard() const;
    void handleJoystickAxes();
    void handleButtonDown(const SDL_Event& e);
    void handleButtonUp(const SDL_Event& e);

    bool mainLoop();
    bool running = true;
    bool paused = true;

    [[nodiscard]] std::string getModeName() const;
    [[nodiscard]] IInputMode* currentMode() const { return currentMode_.get(); };

    struct WheelEntry {
        MODE mode;
        const char* label;
    };

    static constexpr std::array<WheelEntry, 3> kWheelEntries = {{
    {vaultMode,    "Vault"},
    {potionMode,   "Potion"},
    {battlePassMode, "Battle pass"},
    }};

    [[nodiscard]] bool isWheelOpen() const { return wheelOpen_; }
    [[nodiscard]] int wheelSelection() const { return wheelSelection_; } // -1 = none
private:
    static int computeWheelSelection(float x, float y);
    bool filterDeadzone(float& x, float& y) const;
    void normaliseAxes();

    // Performs the actual mode swap (onModeExit -> construct -> onModeEnter).
    void switchTo(ModeFactory factory);
    // Convenience overload for switching by MODE enum value (looks the
    // factory up in kModeFactories). No-op if the mode isn't registered.
    void switchTo(MODE mode);

    // Looks up `button` for the current mode in kButtonTransitions and, if a
    // matching row is found, performs the transition (including any
    // itemIndex carry-over) and returns true. Returns false if no row
    // matched, so the caller can fall back to the mode's own button handler.
    bool applyTransition(Uint8 button);

    bool wheelOpen_ = false;
    Uint32 touchpadPressedAt_ = 0;
    int wheelSelection_ = -1;
    int lastValidWheelSelection_ = -1;
    Uint32 lastValidWheelSelectionAt_ = 0;

    const float DEADZONE = 0.12f;
    const float WALK_DEADZONE = 0.5f;

    Axes leftStickAxes{};
    Axes rightStickAxes{};

    std::array<Sint16, 2> leftStickRawAxes{};
    std::array<Sint16, 2> rightStickRawAxes{};

    SDL_GameController* controller_;
    VirtualKeyboard* keyboard_;
    VirtualMouse* mouse_;

    std::unique_ptr<IInputMode> currentMode_;

    bool rotateLeft = false;
    bool rotateRight = false;

    bool walkLeft = false;
    bool walkRight = false;
    bool walkUp = false;
    bool walkDown = false;
};


#endif //GAMEPADREALM_CONTROLLERMANAGER_H