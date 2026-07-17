#include "VirtualMouse.hpp"
#include <cstdio>
#include <cstring>
#include <fcntl.h>
#include <unistd.h>
#include <linux/uinput.h>
#include <sys/ioctl.h>

VirtualMouse::VirtualMouse(Display *display) {
    // Store the display unconditionally, before any early return below, so that
    // getCursorPosition() and the destructor's XCloseDisplay() always have a
    // valid pointer even if uinput setup fails (e.g. missing permissions).
    display_ = display;

    const int fd = open("/dev/uinput", O_WRONLY | O_NONBLOCK);
    if (fd < 0) { perror("open /dev/uinput"); return; }

    ioctl(fd, UI_SET_EVBIT, EV_KEY);
    ioctl(fd, UI_SET_EVBIT, EV_REL);
    ioctl(fd, UI_SET_RELBIT, REL_X);
    ioctl(fd, UI_SET_RELBIT, REL_Y);

    ioctl(fd, UI_SET_KEYBIT, BTN_LEFT);
    ioctl(fd, UI_SET_KEYBIT, BTN_RIGHT);

    struct uinput_setup usetup{};
    usetup.id.bustype = BUS_USB;
    usetup.id.vendor  = 0x1234;
    usetup.id.product = 0x5678;
    std::strcpy(usetup.name, "Virtual PS5 Stick Mouse");

    ioctl(fd, UI_DEV_SETUP, &usetup);
    ioctl(fd, UI_DEV_CREATE, fd);
    usleep(100000);

    fd_ = fd;
}

VirtualMouse::~VirtualMouse() {
    if (fd_ >= 0) {
        ioctl(fd_, UI_DEV_DESTROY);
        close(fd_);
    }
    if (display_) XCloseDisplay(display_);
}


void VirtualMouse::emit(const int type, const int code, const int val) const {
    if (fd_ < 0) return;
    struct input_event ie{};
    ie.type = type; ie.code = code; ie.value = val;
    write(fd_, &ie, sizeof(ie));
}

void VirtualMouse::pressLeft() const {
    emit(EV_KEY, BTN_LEFT, 1);
    emit(EV_SYN, SYN_REPORT, 0);
}

void VirtualMouse::releaseLeft() const {
    emit(EV_KEY, BTN_LEFT, 0);
    emit(EV_SYN, SYN_REPORT, 0);
}

void VirtualMouse::tapLeft() const {
    pressLeft();
    usleep(20000);
    releaseLeft();
}

void VirtualMouse::tapLeftDouble() const {
    tapLeft();
    usleep(50000);
    tapLeft();
}

void VirtualMouse::pressRight() const {
    emit(EV_KEY, BTN_RIGHT, 1);
    emit(EV_SYN, SYN_REPORT, 0);
}

void VirtualMouse::releaseRight() const {
    emit(EV_KEY, BTN_RIGHT, 0);
    emit(EV_SYN, SYN_REPORT, 0);
}

void VirtualMouse::tapRight() const {
    pressRight();
    usleep(20000);
    releaseRight();
}

void VirtualMouse::moveRelative(const int dx, const int dy) const {
    if (dx != 0) emit(EV_REL, REL_X, dx);
    if (dy != 0) emit(EV_REL, REL_Y, dy);
    emit(EV_SYN, SYN_REPORT, 0);
}

bool VirtualMouse::getCursorPosition(int &x, int &y) const {
    const Window root = DefaultRootWindow(display_);
    Window returnedRoot, returnedChild;
    int rootX, rootY, winX, winY;
    unsigned int mask;

    const Bool ok = XQueryPointer(display_, root, &returnedRoot, &returnedChild,
                             &rootX, &rootY, &winX, &winY, &mask);
    if (!ok) return false;
    x = rootX;
    y = rootY;
    return true;
}
