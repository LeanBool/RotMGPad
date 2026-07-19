#include "VirtualMouse.hpp"
#include "../platform/PlatformSleep.hpp"

#include <cstdio>
#include <cstring>
#include <fcntl.h>
#include <unistd.h>
#include <linux/uinput.h>
#include <sys/ioctl.h>
#include <X11/Xlib.h>

struct VirtualMouse::Impl {
    int fd = -1;
    Display* display = nullptr;

    Impl() {
        display = XOpenDisplay(nullptr);

        const int uinputFd = open("/dev/uinput", O_WRONLY | O_NONBLOCK);
        if (uinputFd < 0) { perror("open /dev/uinput"); return; }

        ioctl(uinputFd, UI_SET_EVBIT, EV_KEY);
        ioctl(uinputFd, UI_SET_EVBIT, EV_REL);
        ioctl(uinputFd, UI_SET_RELBIT, REL_X);
        ioctl(uinputFd, UI_SET_RELBIT, REL_Y);

        ioctl(uinputFd, UI_SET_KEYBIT, BTN_LEFT);
        ioctl(uinputFd, UI_SET_KEYBIT, BTN_RIGHT);

        struct uinput_setup usetup{};
        usetup.id.bustype = BUS_USB;
        usetup.id.vendor  = 0x1234;
        usetup.id.product = 0x5678;
        std::strcpy(usetup.name, "Virtual PS5 Stick Mouse");

        ioctl(uinputFd, UI_DEV_SETUP, &usetup);
        ioctl(uinputFd, UI_DEV_CREATE, uinputFd);
        sleepUs(100000);

        fd = uinputFd;
    }

    ~Impl() {
        if (fd >= 0) {
            ioctl(fd, UI_DEV_DESTROY);
            close(fd);
        }
        if (display) XCloseDisplay(display);
    }

    void emit(const int type, const int code, const int val) const {
        if (fd < 0) return;
        struct input_event ie{};
        ie.type = type; ie.code = code; ie.value = val;
        write(fd, &ie, sizeof(ie));
    }
};

VirtualMouse::VirtualMouse() : impl_(std::make_unique<Impl>()) {}
VirtualMouse::~VirtualMouse() = default;

void VirtualMouse::pressLeft() const {
    impl_->emit(EV_KEY, BTN_LEFT, 1);
    impl_->emit(EV_SYN, SYN_REPORT, 0);
}

void VirtualMouse::releaseLeft() const {
    impl_->emit(EV_KEY, BTN_LEFT, 0);
    impl_->emit(EV_SYN, SYN_REPORT, 0);
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
    impl_->emit(EV_KEY, BTN_RIGHT, 1);
    impl_->emit(EV_SYN, SYN_REPORT, 0);
}

void VirtualMouse::releaseRight() const {
    impl_->emit(EV_KEY, BTN_RIGHT, 0);
    impl_->emit(EV_SYN, SYN_REPORT, 0);
}

void VirtualMouse::tapRight() const {
    pressRight();
    sleepUs(20000);
    releaseRight();
}

void VirtualMouse::moveRelative(const int dx, const int dy) const {
    if (dx != 0) impl_->emit(EV_REL, REL_X, dx);
    if (dy != 0) impl_->emit(EV_REL, REL_Y, dy);
    impl_->emit(EV_SYN, SYN_REPORT, 0);
}

bool VirtualMouse::getCursorPosition(int &x, int &y) const {
    if (!impl_->display) return false;

    const Window root = DefaultRootWindow(impl_->display);
    Window returnedRoot, returnedChild;
    int rootX, rootY, winX, winY;
    unsigned int mask;

    const Bool ok = XQueryPointer(impl_->display, root, &returnedRoot, &returnedChild,
                             &rootX, &rootY, &winX, &winY, &mask);
    if (!ok) return false;
    x = rootX;
    y = rootY;
    return true;
}
