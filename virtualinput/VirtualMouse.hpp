#pragma once

#ifndef GAMEPADREALM_MOUSE_H
#define GAMEPADREALM_MOUSE_H

#include <memory>

// Platform-agnostic virtual mouse: injects clicks/movement at the OS input
// level (so RotMG sees a real mouse) and reports the current cursor
// position. The header carries no platform types (no X11 Display*, no
// windows.h) so every other file can include it freely; the actual
// implementation lives in VirtualMouse_Linux.cpp or VirtualMouse_Windows.cpp,
// selected by CMake for the target platform.
class VirtualMouse {
public:
    VirtualMouse();
    ~VirtualMouse();

    VirtualMouse(const VirtualMouse&) = delete;
    VirtualMouse& operator=(const VirtualMouse&) = delete;

    void pressLeft() const;
    void releaseLeft() const;
    void tapLeft() const;
    void tapLeftDouble() const;
    void pressRight() const;
    void releaseRight() const;
    void tapRight() const;

    void moveRelative(int dx, int dy) const;
    bool getCursorPosition(int &x, int &y) const;

private:
    struct Impl;
    std::unique_ptr<Impl> impl_;
};

#endif //GAMEPADREALM_MOUSE_H
