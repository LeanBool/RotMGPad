// Windows backend for VirtualKeyboard, using SendInput with the keyboard
// scan-code path (KEYEVENTF_SCANCODE) rather than virtual-key codes: RotMG,
// like most games, reads physical scan codes for movement/hotkeys, and
// scan codes are also layout-independent (KEY_W is always the key at the
// "W" position, regardless of the user's keyboard layout) which matches
// how the Linux evdev codes in Keycodes.hpp behave.
#include "VirtualKeyboard.hpp"
#include "../platform/PlatformSleep.hpp"
#include "Keycodes.hpp"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <unordered_map>

namespace {
    // Portable KEY_* code (from Keycodes.hpp) -> PS/2 scan code.
    WORD scanCodeFor(const int keycode) {
        static const std::unordered_map<int, WORD> table = {
            {KEY_W, 0x11}, {KEY_A, 0x1E}, {KEY_S, 0x1F}, {KEY_D, 0x20},
            {KEY_E, 0x12}, {KEY_Q, 0x10}, {KEY_K, 0x25}, {KEY_SPACE, 0x39},
            {KEY_V, 0x2F}, {KEY_R, 0x13}, {KEY_I, 0x17}, {KEY_B, 0x30},
            {KEY_X, 0x2D}, {KEY_Z, 0x2C}, {KEY_F, 0x21},
            {KEY_LEFTCTRL, 0x1D}, {KEY_ENTER, 0x1C},
        };
        const auto it = table.find(keycode);
        return it != table.end() ? it->second : 0;
    }

    void sendKeyEvent(const int keycode, const bool down) {
        const WORD scanCode = scanCodeFor(keycode);
        if (scanCode == 0) return; // unmapped code — add it to the table above

        INPUT input{};
        input.type = INPUT_KEYBOARD;
        input.ki.wScan = scanCode;
        input.ki.dwFlags = KEYEVENTF_SCANCODE | (down ? 0 : KEYEVENTF_KEYUP);
        SendInput(1, &input, sizeof(INPUT));
    }
}

struct VirtualKeyboard::Impl {
    // Nothing to set up ahead of time — SendInput needs no registered device.
};

VirtualKeyboard::VirtualKeyboard() : impl_(std::make_unique<Impl>()) {}
VirtualKeyboard::~VirtualKeyboard() = default;

void VirtualKeyboard::press(const int keycode) const {
    sendKeyEvent(keycode, true);
}

void VirtualKeyboard::release(const int keycode) const {
    sendKeyEvent(keycode, false);
}

void VirtualKeyboard::tap(const int keycode) const {
    press(keycode);
    sleepUs(10000);
    release(keycode);
}
