#pragma once
#ifndef GAMEPADREALM_KEYCODES_HPP
#define GAMEPADREALM_KEYCODES_HPP

#ifdef __linux__
#include <linux/input-event-codes.h>
#else

constexpr int KEY_W = 1001;
constexpr int KEY_A = 1002;
constexpr int KEY_S = 1003;
constexpr int KEY_D = 1004;
constexpr int KEY_E = 1005;
constexpr int KEY_Q = 1006;
constexpr int KEY_K = 1007;
constexpr int KEY_SPACE = 1008;
constexpr int KEY_V = 1009;
constexpr int KEY_R = 1010;
constexpr int KEY_I = 1011;
constexpr int KEY_B = 1012;
constexpr int KEY_X = 1013;
constexpr int KEY_Z = 1014;
constexpr int KEY_F = 1015;
constexpr int KEY_LEFTCTRL = 1016;
constexpr int KEY_ENTER = 1017;

#endif // __linux__

#endif //GAMEPADREALM_KEYCODES_HPP
