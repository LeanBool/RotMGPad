#include "ControllerManager.hpp"

#include "../virtualinput/Keycodes.hpp"
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_timer.h>
#include <cmath>
#include <array>
#include <memory>
#include <optional>

#include "modes/AttributesMode.hpp"
#include "modes/BattlePassMode.hpp"
#include "modes/EquipmentMode.hpp"
#include "modes/InventoryMode.hpp"
#include "modes/MenuBarMode.hpp"
#include "modes/MissionMode.hpp"
#include "modes/MissionPreviewMode.hpp"
#include "modes/NormalMode.hpp"
#include "modes/PartyMode.hpp"
#include "modes/PetInventoryMode.hpp"
#include "modes/PickupMode.hpp"
#include "modes/PotionMode.hpp"
#include "modes/TeleportMode.hpp"
#include "modes/VaultMode.hpp"

namespace {
template <typename ModeT>
std::unique_ptr<IInputMode> makeMode(VirtualMouse* mouse, VirtualKeyboard* keyboard) {
    return std::make_unique<ModeT>(mouse, keyboard);
}

struct ModeFactoryEntry {
    MODE mode;
    ControllerManager::ModeFactory factory;
};

constexpr std::array<ModeFactoryEntry, 14> kModeFactories = {{
    { normalMode,       &makeMode<NormalMode> },
    { teleportMode,     &makeMode<TeleportMode> },
    { partyMode,        &makeMode<PartyMode> },
    { inventoryMode,    &makeMode<InventoryMode> },
    { pickupMode,       &makeMode<PickupMode> },
    { equipmentMode,    &makeMode<EquipmentMode> },
    { menuBarMode,      &makeMode<MenuBarMode> },
    { vaultMode,        &makeMode<VaultMode> },
    { potionMode,       &makeMode<PotionMode> },
    { battlePassMode,   &makeMode<BattlePassMode> },
    { petInventoryMode, &makeMode<PetInventoryMode> },
    { missionMode,      &makeMode<MissionMode> },
    { missionPreviewMode,   &makeMode<MissionPreviewMode> },
    { attributesMode,      &makeMode<AttributesMode> },
}};

ControllerManager::ModeFactory factoryFor(const MODE mode) {
    for (const auto& entry : kModeFactories) {
        if (entry.mode == mode) return entry.factory;
    }
    return nullptr;
}

template <MODE M>
bool modeIsNot(IInputMode& mode) { return mode.id() != M; }

template <int N>
bool idxLessThan(IInputMode& mode) { return mode.itemIndex() < N; }

template <int N>
bool idxAtLeast(IInputMode& mode) { return mode.itemIndex() >= N; }

template <int N>
bool idxEquals(IInputMode& mode) { return mode.itemIndex() == N; }

int identity(int i) { return i; }

template <int N>
int addN(int i) { return i + N; }

template <int N>
int constant(int) { return N; }

struct ButtonTransition {
    std::optional<MODE> from;         // required current mode, or std::nullopt for "any mode"
    Uint8 button;                     // SDL_GameControllerButton
    bool (*guard)(IInputMode&);       // nullptr = always allowed
    MODE to;                          // mode to switch into
    int (*indexTransform)(int);       // nullptr = don't touch itemIndex
};

constexpr std::array<ButtonTransition, 28> kButtonTransitions = {{
    // Right stick: toggle Teleport mode.
    { teleportMode, SDL_CONTROLLER_BUTTON_RIGHTSTICK, nullptr,                 normalMode,   nullptr },
    { std::nullopt, SDL_CONTROLLER_BUTTON_RIGHTSTICK, modeIsNot<teleportMode>, teleportMode, nullptr },

    // Left stick: toggle Party mode.
    { partyMode,    SDL_CONTROLLER_BUTTON_LEFTSTICK, nullptr,              normalMode, nullptr },
    { std::nullopt, SDL_CONTROLLER_BUTTON_LEFTSTICK, modeIsNot<partyMode>, partyMode,  nullptr },

    // Start: toggle Inventory mode.
    { inventoryMode, SDL_CONTROLLER_BUTTON_START, nullptr,                  normalMode,    nullptr },
    { std::nullopt,  SDL_CONTROLLER_BUTTON_START, modeIsNot<inventoryMode>, inventoryMode, nullptr },

    // A button: mode.onButtonA() is invoked separately in handleButtonDown;
    // these rows are the follow-up mode changes.
    { partyMode,    SDL_CONTROLLER_BUTTON_A, nullptr,      normalMode,       nullptr },
    { teleportMode, SDL_CONTROLLER_BUTTON_A, nullptr,      normalMode,       nullptr },
    { menuBarMode,  SDL_CONTROLLER_BUTTON_A, idxEquals<0>, attributesMode, nullptr },
    { menuBarMode,  SDL_CONTROLLER_BUTTON_A, idxEquals<1>, petInventoryMode, nullptr },
    { menuBarMode,  SDL_CONTROLLER_BUTTON_A, idxEquals<3>, missionPreviewMode,      nullptr },
    { missionPreviewMode,  SDL_CONTROLLER_BUTTON_A, nullptr, missionMode,      nullptr },

    // B button: back out to Normal mode from anywhere else.
    { attributesMode, SDL_CONTROLLER_BUTTON_B, nullptr, menuBarMode, constant<0> },
    { petInventoryMode, SDL_CONTROLLER_BUTTON_B, nullptr, menuBarMode, constant<1> },
    { missionPreviewMode, SDL_CONTROLLER_BUTTON_B, nullptr, menuBarMode, constant<3> },
    { std::nullopt, SDL_CONTROLLER_BUTTON_B, modeIsNot<normalMode>, normalMode, nullptr },

    // D-pad up: climb the pickup/inventory/equipment/menu stack.
    { normalMode,    SDL_CONTROLLER_BUTTON_DPAD_UP, nullptr,        pickupMode,    nullptr },
    { inventoryMode, SDL_CONTROLLER_BUTTON_DPAD_UP, idxLessThan<4>, equipmentMode, identity },
    { pickupMode,    SDL_CONTROLLER_BUTTON_DPAD_UP, idxLessThan<4>, inventoryMode, addN<4> },
    { equipmentMode, SDL_CONTROLLER_BUTTON_DPAD_UP, nullptr,        menuBarMode,   identity },

    // D-pad down: descend the same stack.
    { inventoryMode, SDL_CONTROLLER_BUTTON_DPAD_DOWN, idxAtLeast<4>, pickupMode,    addN<-4> },
    { equipmentMode, SDL_CONTROLLER_BUTTON_DPAD_DOWN, nullptr,       inventoryMode, identity },
    { menuBarMode,   SDL_CONTROLLER_BUTTON_DPAD_DOWN, nullptr,       equipmentMode, identity },

    // D-pad right: jump from the pet inventory's last slot into the menu bar.
    { petInventoryMode, SDL_CONTROLLER_BUTTON_DPAD_RIGHT, idxEquals<3>, menuBarMode, constant<1> },

    // Shoulders: cycle Inventory -> Vault -> Potion (left) and back (right).
    { inventoryMode, SDL_CONTROLLER_BUTTON_LEFTSHOULDER,  nullptr, vaultMode,     nullptr },
    { vaultMode,     SDL_CONTROLLER_BUTTON_LEFTSHOULDER,  nullptr, potionMode,    nullptr },
    { vaultMode,     SDL_CONTROLLER_BUTTON_RIGHTSHOULDER, nullptr, inventoryMode, nullptr },
    { potionMode,    SDL_CONTROLLER_BUTTON_RIGHTSHOULDER, nullptr, vaultMode,     nullptr },
}};

} // namespace

ControllerManager::ControllerManager(SDL_GameController *controller, VirtualKeyboard *keyboard, VirtualMouse *mouse) {
    controller_ = controller;
    keyboard_ = keyboard;
    mouse_ = mouse;
    currentMode_ = std::make_unique<NormalMode>(mouse_, keyboard_);
}

ControllerManager::~ControllerManager() {
    SDL_GameControllerClose(controller_);
}

void ControllerManager::switchTo(const ModeFactory factory) {
    currentMode_->onModeExit();
    currentMode_ = factory(mouse_, keyboard_);
    currentMode_->onModeEnter();
}

void ControllerManager::switchTo(const MODE mode) {
    if (const ModeFactory factory = factoryFor(mode)) {
        switchTo(factory);
    }
}

bool ControllerManager::applyTransition(const Uint8 button) {
    const MODE from = currentMode_->id();

    for (const auto& t : kButtonTransitions) {
        if (t.button != button) continue;
        if (t.from.has_value() && *t.from != from) continue;
        if (t.guard && !t.guard(*currentMode_)) continue;

        const int prevIndex = currentMode_->itemIndex();
        switchTo(t.to);
        if (t.indexTransform) {
            currentMode_->setItemIndex(t.indexTransform(prevIndex));
        }
        return true;
    }
    return false;
}

void ControllerManager::updateKeyboard() const {
    if (paused) {
        return;
    }

    const std::array<std::pair<bool, int>, 6> keyStates{{
        {rotateRight, KEY_E},
        {rotateLeft,  KEY_Q},
        {walkLeft,    KEY_A},
        {walkRight,   KEY_D},
        {walkUp,      KEY_W},
        {walkDown,    KEY_S},
    }};

    for (const auto& [pressed, key] : keyStates) {
        if (pressed) keyboard_->press(key);
        else keyboard_->release(key);
    }

    if (currentMode_->id() == teleportMode) {
        keyboard_->tap(KEY_K);
    }
}

void ControllerManager::handleJoystickAxes() {
    if (paused && !wheelOpen_) {
        return;
    }

    leftStickRawAxes = {
        SDL_GameControllerGetAxis(controller_, SDL_CONTROLLER_AXIS_LEFTX),
        SDL_GameControllerGetAxis(controller_, SDL_CONTROLLER_AXIS_LEFTY)
    };
    rightStickRawAxes = {
        SDL_GameControllerGetAxis(controller_, SDL_CONTROLLER_AXIS_RIGHTX),
        SDL_GameControllerGetAxis(controller_, SDL_CONTROLLER_AXIS_RIGHTY)
    };

    normaliseAxes();

    if (wheelOpen_) {
        wheelSelection_ = computeWheelSelection(rightStickAxes.x, rightStickAxes.y);
        if (wheelSelection_ >= 0) {
            lastValidWheelSelection_ = wheelSelection_;
            lastValidWheelSelectionAt_ = SDL_GetTicks();
        }
        return;
    }

    if (currentMode_->id() != teleportMode && currentMode_->id() != partyMode) {
        walkLeft = leftStickAxes.x < -WALK_DEADZONE;
        walkRight = leftStickAxes.x > WALK_DEADZONE;
        walkUp = leftStickAxes.y < -WALK_DEADZONE;
        walkDown = leftStickAxes.y > WALK_DEADZONE;
    }
}

void ControllerManager::handleButtonDown(const SDL_Event& e) {
    const auto button = e.cbutton.button;

    if (paused && button != SDL_CONTROLLER_BUTTON_TOUCHPAD && button != SDL_CONTROLLER_BUTTON_BACK) {
        return;
    }

    // Buttons that never drive a mode transition, handled up front.
    switch (button) {
        case SDL_CONTROLLER_BUTTON_BACK:
            wheelOpen_ = true;
            touchpadPressedAt_ = SDL_GetTicks();
            wheelSelection_ = -1;
            lastValidWheelSelection_ = -1;
            return;

        case SDL_CONTROLLER_BUTTON_TOUCHPAD:
            paused = !paused;
            return;

        case SDL_CONTROLLER_BUTTON_GUIDE:
            running = false;
            return;

        case SDL_CONTROLLER_BUTTON_X:
            currentMode_->onButtonX();
            return;

        case SDL_CONTROLLER_BUTTON_Y:
            currentMode_->onButtonY();
            return;

        case SDL_CONTROLLER_BUTTON_DPAD_LEFT:
            currentMode_->onDpadLeft();
            return;

        default:
            break;
    }

    // Give the active mode first refusal on the button (its own visible
    // effect), independent of whether it also triggers a mode change below.
    switch (button) {
        case SDL_CONTROLLER_BUTTON_A:              currentMode_->onButtonA();       break;
        case SDL_CONTROLLER_BUTTON_B:               currentMode_->onButtonB();       break;
        case SDL_CONTROLLER_BUTTON_LEFTSHOULDER:   currentMode_->onShoulderLeft();  break;
        case SDL_CONTROLLER_BUTTON_RIGHTSHOULDER:  currentMode_->onShoulderRight(); break;
        default: break;
    }

    // Then consult the transition table for any resulting mode change.
    if (applyTransition(button)) {
        return;
    }

    // No table entry matched: let the mode handle dpad up/down/right itself.
    switch (button) {
        case SDL_CONTROLLER_BUTTON_DPAD_UP:    currentMode_->onDpadUp();    break;
        case SDL_CONTROLLER_BUTTON_DPAD_DOWN:  currentMode_->onDpadDown();  break;
        case SDL_CONTROLLER_BUTTON_DPAD_RIGHT: currentMode_->onDpadRight(); break;
        default: break;
    }
}

void ControllerManager::handleButtonUp(const SDL_Event& e) {
    switch (e.cbutton.button) {
        case SDL_CONTROLLER_BUTTON_LEFTSHOULDER:
            rotateLeft = false;
            break;
        case SDL_CONTROLLER_BUTTON_RIGHTSHOULDER:
            rotateRight = false;
            break;
        default:
            break;

        // handleButtonUp — fall back to the cached selection if the live one dropped out
        case SDL_CONTROLLER_BUTTON_BACK: {
            const Uint32 releasedAt = SDL_GetTicks();
            const Uint32 held = releasedAt - touchpadPressedAt_;
            wheelOpen_ = false;

            int committed = wheelSelection_;
            if (committed < 0 && lastValidWheelSelection_ >= 0 &&
                releasedAt - lastValidWheelSelectionAt_ < WHEEL_SELECTION_GRACE_MS) {
                committed = lastValidWheelSelection_;
            }

            if (held >= WHEEL_HOLD_THRESHOLD_MS && committed >= 0) {
                switchTo(kWheelEntries[committed].mode);
            } else {
                switchTo(normalMode);
            }

            wheelSelection_ = -1;
            lastValidWheelSelection_ = -1;
            break;
        }
    }
}

bool ControllerManager::mainLoop() {
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        switch (e.type) {
            case SDL_QUIT:
                running = false;
                break;
            case SDL_CONTROLLERBUTTONDOWN:
                handleButtonDown(e);
                break;
            case SDL_CONTROLLERBUTTONUP:
                handleButtonUp(e);
                break;
            case SDL_CONTROLLERAXISMOTION:
                handleJoystickAxes();
                break;
            default:
                break;
        }

        rotateRight = SDL_GameControllerGetAxis(controller_, SDL_CONTROLLER_AXIS_TRIGGERRIGHT);
        rotateLeft = SDL_GameControllerGetAxis(controller_, SDL_CONTROLLER_AXIS_TRIGGERLEFT);
    }

    if (!paused && !wheelOpen_) {
        updateKeyboard();
        currentMode_->updateCursor(leftStickAxes, rightStickAxes);
    }

    return true;
}

std::string ControllerManager::getModeName() const {
    return currentMode_->name();
}

void ControllerManager::normaliseAxes() {
    float rx = static_cast<float>(rightStickRawAxes[0]) / 32768.0f;
    float ry = static_cast<float>(rightStickRawAxes[1]) / 32768.0f;
    filterDeadzone(rx, ry);
    rightStickAxes.x = rx;
    rightStickAxes.y = ry;

    float lx = static_cast<float>(leftStickRawAxes[0]) / 32768.0f;
    float ly = static_cast<float>(leftStickRawAxes[1]) / 32768.0f;
    filterDeadzone(lx, ly);
    leftStickAxes.x = lx;
    leftStickAxes.y = ly;
}

bool ControllerManager::filterDeadzone(float& x, float& y) const {
    const float mag = std::sqrt(x*x + y*y);
    if (mag < DEADZONE) {
        x = y = 0.0f;
        return true;
    }
    const float scale = (mag - DEADZONE) / (1.0f - DEADZONE);
    x = (x / mag) * scale;
    y = (y / mag) * scale;
    return false;
}

int ControllerManager::computeWheelSelection(float x, float y) {
    if (const float mag = std::sqrt(x*x + y*y); mag < WHEEL_SELECT_DEADZONE) {
        return -1;
    }
    constexpr float kPi = 3.14159265358979323846f;
    float angleDeg = std::atan2(x, -y) * (180.0f / kPi);
    if (angleDeg < 0.0f) angleDeg += 360.0f;
    constexpr int kCount = static_cast<int>(ControllerManager::kWheelEntries.size());
    const float wedgeDeg = 360.0f / kCount;
    return static_cast<int>((angleDeg + wedgeDeg / 2.0f) / wedgeDeg) % kCount;
}