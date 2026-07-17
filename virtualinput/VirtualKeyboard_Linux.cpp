// Linux backend for VirtualKeyboard: a uinput virtual HID keyboard. Works
// under both X11 and Wayland/gamescope since uinput sits below the display
// server. Codes come straight from Keycodes.hpp, which on Linux is just
// <linux/input-event-codes.h> passed through unchanged.
#include "VirtualKeyboard.hpp"
#include "../platform/PlatformSleep.hpp"

#include <fcntl.h>
#include <unistd.h>
#include <cstring>
#include <cstdio>
#include <linux/uinput.h>
#include <sys/ioctl.h>

struct VirtualKeyboard::Impl {
    int fd = -1;

    Impl() {
        fd = open("/dev/uinput", O_WRONLY | O_NONBLOCK);
        if (fd < 0) { perror("open /dev/uinput"); return; }

        ioctl(fd, UI_SET_EVBIT, EV_KEY);
        // Enable every keycode 0-255 so any key can be sent without extra setup
        for (int i = 0; i < 256; i++) {
            ioctl(fd, UI_SET_KEYBIT, i);
        }

        struct uinput_setup usetup{};
        usetup.id.bustype = BUS_USB;
        usetup.id.vendor  = 0x1234;
        usetup.id.product = 0x5680;
        strcpy(usetup.name, "Virtual PS5 Keyboard");

        ioctl(fd, UI_DEV_SETUP, &usetup);
        ioctl(fd, UI_DEV_CREATE, fd);
        sleepUs(100000); // let the device register before first use
    }

    ~Impl() {
        if (fd >= 0) {
            ioctl(fd, UI_DEV_DESTROY);
            close(fd);
        }
    }

    void emit(const int type, const int code, const int val) const {
        if (fd < 0) return;
        struct input_event ie{};
        ie.type = type; ie.code = code; ie.value = val;
        write(fd, &ie, sizeof(ie));
    }
};

VirtualKeyboard::VirtualKeyboard() : impl_(std::make_unique<Impl>()) {}
VirtualKeyboard::~VirtualKeyboard() = default;

void VirtualKeyboard::press(const int keycode) const {
    impl_->emit(EV_KEY, keycode, 1);
    impl_->emit(EV_SYN, SYN_REPORT, 0);
}

void VirtualKeyboard::release(const int keycode) const {
    impl_->emit(EV_KEY, keycode, 0);
    impl_->emit(EV_SYN, SYN_REPORT, 0);
}

void VirtualKeyboard::tap(const int keycode) const {
    press(keycode);
    sleepUs(10000); // 10ms, gives apps time to register the keystroke
    release(keycode);
}
