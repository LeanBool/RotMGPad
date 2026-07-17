#include "VirtualKeyboard.hpp"
#include <fcntl.h>
#include <unistd.h>
#include <cstring>
#include <cstdio>
#include <linux/uinput.h>
#include <sys/ioctl.h>

VirtualKeyboard::VirtualKeyboard() {
    fd_ = open("/dev/uinput", O_WRONLY | O_NONBLOCK);
    if (fd_ < 0) { perror("open /dev/uinput"); return; }

    ioctl(fd_, UI_SET_EVBIT, EV_KEY);
    // Enable every keycode 0-255 so any key can be sent without extra setup
    for (int i = 0; i < 256; i++) {
        ioctl(fd_, UI_SET_KEYBIT, i);
    }

    struct uinput_setup usetup{};
    usetup.id.bustype = BUS_USB;
    usetup.id.vendor  = 0x1234;
    usetup.id.product = 0x5680;
    strcpy(usetup.name, "Virtual PS5 Keyboard");

    ioctl(fd_, UI_DEV_SETUP, &usetup);
    ioctl(fd_, UI_DEV_CREATE, fd_);
    usleep(100000); // let the device register before first use
}

VirtualKeyboard::~VirtualKeyboard() {
    if (fd_ >= 0) {
        ioctl(fd_, UI_DEV_DESTROY);
        close(fd_);
    }
}

void VirtualKeyboard::emit(const int type, const int code, const int val) const {
    if (fd_ < 0) return;
    struct input_event ie{};
    ie.type = type; ie.code = code; ie.value = val;
    write(fd_, &ie, sizeof(ie));
}

void VirtualKeyboard::press(const int keycode) const {
    emit(EV_KEY, keycode, 1);
    emit(EV_SYN, SYN_REPORT, 0);
}

void VirtualKeyboard::release(const int keycode) const {
    emit(EV_KEY, keycode, 0);
    emit(EV_SYN, SYN_REPORT, 0);
}

void VirtualKeyboard::tap(const int keycode) const {
    press(keycode);
    usleep(10000); // 10ms, gives apps time to register the keystroke
    release(keycode);
}