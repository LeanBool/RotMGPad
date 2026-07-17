#pragma once
#ifndef GAMEPADREALM_VIRTUALKEYBOARD_H
#define GAMEPADREALM_VIRTUALKEYBOARD_H

#include <memory>

// Platform-agnostic virtual keyboard. Callers pass the portable codes from
// Keycodes.hpp (KEY_W, KEY_SPACE, ...); the Linux backend feeds them to
// uinput as-is, the Windows backend translates them to Win32 VK_ codes.
class VirtualKeyboard {
public:
    VirtualKeyboard();
    ~VirtualKeyboard();

    VirtualKeyboard(const VirtualKeyboard&) = delete;
    VirtualKeyboard& operator=(const VirtualKeyboard&) = delete;

    void press(int keycode) const;    // key down
    void release(int keycode) const;  // key up
    void tap(int keycode) const;      // press + release, e.g. for a single keystroke

private:
    struct Impl;
    std::unique_ptr<Impl> impl_;
};

#endif //GAMEPADREALM_VIRTUALKEYBOARD_H
