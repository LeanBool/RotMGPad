#pragma once
#ifndef GAMEPADREALM_PLATFORMSLEEP_HPP
#define GAMEPADREALM_PLATFORMSLEEP_HPP

#include <thread>
#include <chrono>

inline void sleepUs(const long microseconds) {
    std::this_thread::sleep_for(std::chrono::microseconds(microseconds));
}

#endif //GAMEPADREALM_PLATFORMSLEEP_HPP
