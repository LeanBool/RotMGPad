// Windows backend for VirtualMouse. There's no uinput equivalent here, so
// this uses SendInput to inject real mouse events directly (RotMG sees them
// exactly like hardware input) and GetCursorPos to read the position back.
#include "VirtualMouse.hpp"
#include "../platform/PlatformSleep.hpp"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

namespace {
    void sendMouseEvent(const DWORD flags, const LONG dx = 0, const LONG dy = 0) {
        INPUT input{};
        input.type = INPUT_MOUSE;
        input.mi.dx = dx;
        input.mi.dy = dy;
        input.mi.dwFlags = flags;
        SendInput(1, &input, sizeof(INPUT));
    }
}

struct VirtualMouse::Impl {
    // Nothing to set up ahead of time on Windows — SendInput needs no
    // virtual device registration the way uinput does.
};

VirtualMouse::VirtualMouse() : impl_(std::make_unique<Impl>()) {}
VirtualMouse::~VirtualMouse() = default;

void VirtualMouse::pressLeft() const {
    sendMouseEvent(MOUSEEVENTF_LEFTDOWN);
}

void VirtualMouse::releaseLeft() const {
    sendMouseEvent(MOUSEEVENTF_LEFTUP);
}

void VirtualMouse::tapLeft() const {
    pressLeft();
    sleepUs(20000);
    releaseLeft();
}

void VirtualMouse::tapLeftDouble() const {
    tapLeft();
    sleepUs(50000);
    tapLeft();
}

void VirtualMouse::pressRight() const {
    sendMouseEvent(MOUSEEVENTF_RIGHTDOWN);
}

void VirtualMouse::releaseRight() const {
    sendMouseEvent(MOUSEEVENTF_RIGHTUP);
}

void VirtualMouse::tapRight() const {
    pressRight();
    sleepUs(20000);
    releaseRight();
}

void VirtualMouse::moveRelative(const int dx, const int dy) const {
    if (dx != 0 || dy != 0) {
        sendMouseEvent(MOUSEEVENTF_MOVE, dx, dy);
    }
}

bool VirtualMouse::getCursorPosition(int &x, int &y) const {
    POINT p;
    if (!GetCursorPos(&p)) return false;
    x = p.x;
    y = p.y;
    return true;
}
