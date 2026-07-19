#pragma once
#ifndef GAMEPADREALM_VIRTUALKEYBOARD_H
#define GAMEPADREALM_VIRTUALKEYBOARD_H

#include <memory>

class VirtualKeyboard {
public:
    VirtualKeyboard();
    ~VirtualKeyboard();

    VirtualKeyboard(const VirtualKeyboard&) = delete;
    VirtualKeyboard& operator=(const VirtualKeyboard&) = delete;

    void press(int keycode) const;    // key down
    void release(int keycode) const;  // key up
    void tap(int keycode) const;      // press + release

private:
    struct Impl;
    std::unique_ptr<Impl> impl_;
};

#endif //GAMEPADREALM_VIRTUALKEYBOARD_H
