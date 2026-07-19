#pragma once

#ifndef GAMEPADREALM_MOUSE_H
#define GAMEPADREALM_MOUSE_H

#include <memory>

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
