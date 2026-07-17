//
// Created by alex on 7/9/26.
//

#ifndef GAMEPADREALM_GLOBALS_HPP
#define GAMEPADREALM_GLOBALS_HPP
#include <SDL2/SDL_stdinc.h>

enum MODE {
    normalMode,
    pickupMode,
    teleportMode,
    inventoryMode,
    partyMode,
    vaultMode,
    potionMode,
    equipmentMode,
    menuBarMode,
    petInventoryMode,
    missionMode,
    battlePassMode,
    missionPreviewMode,
    attributesMode
};

// ---------------------------------------------------------------------------
// All the coordinates below were measured by hand on a 1920x1080 RotMG
// window. Base:: holds those original measurements untouched, as a reference.
// The plain names (BATTLE_PASS_X, INV_SLOT0_X, ...) are runtime values,
// computed once at startup by initScaledGlobals() so they land in the right
// place at whatever resolution the game is actually running at. Every call
// site elsewhere in the codebase keeps using the plain names exactly as
// before — only globals.hpp/.cpp change.
// ---------------------------------------------------------------------------

namespace Base {
    constexpr int SCREEN_WIDTH = 1920;
    constexpr int SCREEN_HEIGHT = 1080;

    constexpr int BATTLE_PASS_CLOSE_POPUP_X = 1890;
    constexpr int BATTLE_PASS_CLOSE_POPUP_Y = 35;
    constexpr int BATTLE_PASS_CLAIM_ALL_X = 965;
    constexpr int BATTLE_PASS_CLAIM_ALL_Y = 940;
    constexpr int BATTLE_PASS_X = 190;
    constexpr int BATTLE_PASS_Y = 240;
    constexpr int BATTLE_PASS_CLOSE_X = 1685;
    constexpr int BATTLE_PASS_CLOSE_Y = 70;

    constexpr int MISSION_MODE_SEE_ALL_X = 1330;
    constexpr int MISSION_MODE_SEE_ALL_Y = 750;
    constexpr int MISSION_MODE_CLOSE_X = 1670;
    constexpr int MISSION_MODE_CLOSE_Y = 90;

    constexpr int MISSION_PREVIEW_MODE_SCROLL_X = 1545;
    constexpr int MISSION_PREVIEW_MODE_SCROLL_Y = 305;
    constexpr int MISSION_PREVIEW_MODE_SCROLL_WIDTH = 5;
    constexpr int MISSION_PREVIEW_MODE_SCROLL_HEIGHT = 400;

    constexpr int ATTRIBUTES_DUNGEONS_TAB_X = 1450;
    constexpr int ATTRIBUTES_DUNGEONS_TAB_Y = 440;
    constexpr int ATTRIBUTES_STATS_TAB_X = 1235;
    constexpr int ATTRIBUTES_STATS_TAB_Y = 440;
    constexpr int ATTRIBUTES_SCROLL_X = 1525;
    constexpr int ATTRIBUTES_SCROLL_Y = 500;
    constexpr int ATTRIBUTES_SCROLL_WIDTH = 5;
    constexpr int ATTRIBUTES_SCROLL_HEIGHT = 500;

    constexpr int PET_INVENTORY_X = 1230;
    constexpr int PET_INVENTORY_Y = 730;
    constexpr int PET_INVENTORY_WIDTH = 85;
    constexpr int PET_INVENTORY_HEIGHT = 85;

    constexpr int MENUBAR_X = 1585;
    constexpr int MENUBAR_Y = 390;
    constexpr int MENUBAR_WIDTH = 42;
    constexpr int MENUBAR_HEIGHT = 42;

    constexpr int EQUIPMENT_SLOT0_X = 1615;
    constexpr int EQUIPMENT_SLOT0_Y = 585;
    constexpr int EQUIPMENT_SLOT_WIDTH = 85;
    constexpr int EQUIPMENT_SLOT_HEIGHT = 85;

    constexpr int POTION_RACK_SLOT0_X = 1275;
    constexpr int POTION_RACK_SLOT0_Y = 185;
    constexpr int POTION_RACK_SLOT_OFFSET_X = 225;
    constexpr int POTION_RACK_SLOT_OFFSET_Y = 180;
    constexpr int POTION_RACK_SLOT_WIDTH = 60;
    constexpr int POTION_RACK_SLOT_HEIGHT = 30;

    constexpr int CENTER_X = 777;
    constexpr int CENTER_Y = 552;
    constexpr int RADIUS   = 400;

    constexpr int CHAT_X = 360;
    constexpr int CHAT_Y = 1020;
    constexpr int CHAT_HEIGHT = 450;
    constexpr int CHAT_WIDTH = 650;
    constexpr int CHAT_JOIN_BUTTON_OFFSET_X = 160;
    constexpr int CHAT_JOIN_BUTTON_OFFSET_Y = -200;
    constexpr int CHAT_TELEPORT_BUTTON_OFFSET_X = 160;
    constexpr int CHAT_TELEPORT_BUTTON_OFFSET_Y = -280;

    constexpr int PICKUP_SLOT0_X = 1615;
    constexpr int PICKUP_SLOT0_Y = 937;
    constexpr int PICKUP_SLOT_WIDTH = 85;
    constexpr int PICKUP_SLOT_HEIGHT = 85;

    constexpr int MAP_CENTER_X = 1745;
    constexpr int MAP_CENTER_Y = 185;
    constexpr int MAP_RADIUS   = 170;
    constexpr int MAP_TELEPORT_BUTTON_OFFSET_X = -40;
    constexpr int MAP_TELEPORT_BUTTON_OFFSET_Y = 40;

    constexpr int INV_SLOT_WIDTH = 85;
    constexpr int INV_SLOT_HEIGHT = 85;
    constexpr int INV_SLOT0_X = 1615;
    constexpr int INV_SLOT0_Y = 725;

    constexpr int EXTRA_SLOT0_WIDTH = 160;
    constexpr int EXTRA_SLOT0_X = 1660;
    constexpr int EXTRA_SLOT0_Y = 870;

    constexpr int VAULT_SLOT0_X = 878;
    constexpr int VAULT_SLOT0_Y = 263;

    constexpr int WHEEL_RADIUS = 400;
}

// Runtime, resolution-scaled values — same names every mode file already
// uses. Populated by initScaledGlobals(); valid only after that has run.
extern int BATTLE_PASS_CLOSE_POPUP_X;
extern int BATTLE_PASS_CLOSE_POPUP_Y;
extern int BATTLE_PASS_CLAIM_ALL_X;
extern int BATTLE_PASS_CLAIM_ALL_Y;
extern int BATTLE_PASS_X;
extern int BATTLE_PASS_Y;
extern int BATTLE_PASS_CLOSE_X;
extern int BATTLE_PASS_CLOSE_Y;

extern int MISSION_MODE_SEE_ALL_X;
extern int MISSION_MODE_SEE_ALL_Y;
extern int MISSION_MODE_CLOSE_X;
extern int MISSION_MODE_CLOSE_Y;

extern int MISSION_PREVIEW_MODE_SCROLL_X;
extern int MISSION_PREVIEW_MODE_SCROLL_Y;
extern int MISSION_PREVIEW_MODE_SCROLL_WIDTH;
extern int MISSION_PREVIEW_MODE_SCROLL_HEIGHT;

extern int ATTRIBUTES_DUNGEONS_TAB_X;
extern int ATTRIBUTES_DUNGEONS_TAB_Y;
extern int ATTRIBUTES_STATS_TAB_X;
extern int ATTRIBUTES_STATS_TAB_Y;
extern int ATTRIBUTES_SCROLL_X;
extern int ATTRIBUTES_SCROLL_Y;
extern int ATTRIBUTES_SCROLL_WIDTH;
extern int ATTRIBUTES_SCROLL_HEIGHT;

extern int PET_INVENTORY_X;
extern int PET_INVENTORY_Y;
extern int PET_INVENTORY_WIDTH;
extern int PET_INVENTORY_HEIGHT;

extern int MENUBAR_X;
extern int MENUBAR_Y;
extern int MENUBAR_WIDTH;
extern int MENUBAR_HEIGHT;

extern int EQUIPMENT_SLOT0_X;
extern int EQUIPMENT_SLOT0_Y;
extern int EQUIPMENT_SLOT_WIDTH;
extern int EQUIPMENT_SLOT_HEIGHT;

extern int POTION_RACK_SLOT0_X;
extern int POTION_RACK_SLOT0_Y;
extern int POTION_RACK_SLOT_OFFSET_X;
extern int POTION_RACK_SLOT_OFFSET_Y;
extern int POTION_RACK_SLOT_WIDTH;
extern int POTION_RACK_SLOT_HEIGHT;

extern int CENTER_X;
extern int CENTER_Y;
extern int RADIUS;

extern int CHAT_X;
extern int CHAT_Y;
extern int CHAT_HEIGHT;
extern int CHAT_WIDTH;
extern int CHAT_JOIN_BUTTON_OFFSET_X;
extern int CHAT_JOIN_BUTTON_OFFSET_Y;
extern int CHAT_TELEPORT_BUTTON_OFFSET_X;
extern int CHAT_TELEPORT_BUTTON_OFFSET_Y;

extern int PICKUP_SLOT0_X;
extern int PICKUP_SLOT0_Y;
extern int PICKUP_SLOT_WIDTH;
extern int PICKUP_SLOT_HEIGHT;

extern int MAP_CENTER_X;
extern int MAP_CENTER_Y;
extern int MAP_RADIUS;
extern int MAP_TELEPORT_BUTTON_OFFSET_X;
extern int MAP_TELEPORT_BUTTON_OFFSET_Y;

extern int INV_SLOT_WIDTH;
extern int INV_SLOT_HEIGHT;
extern int INV_SLOT0_X;
extern int INV_SLOT0_Y;

extern int EXTRA_SLOT0_WIDTH;
extern int EXTRA_SLOT0_X;
extern int EXTRA_SLOT0_Y;

extern int VAULT_SLOT0_X;
extern int VAULT_SLOT0_Y;

extern int SCREEN_WIDTH;
extern int SCREEN_HEIGHT;
extern int SCREEN_CENTER_X;
extern int SCREEN_CENTER_Y;

extern int WHEEL_CENTER_X;
extern int WHEEL_CENTER_Y;
extern int WHEEL_RADIUS;

// Not resolution-dependent — a movement-speed multiplier and timing/deadzone
// constants for the radial wheel, so they stay fixed regardless of screen size.
constexpr float ALPHA_CURSOR_SPEED = 0.1f;
static constexpr Uint32 WHEEL_HOLD_THRESHOLD_MS = 50;
static constexpr float WHEEL_SELECT_DEADZONE = 0.3f;
static constexpr Uint32 WHEEL_SELECTION_GRACE_MS = 250;

// Call once at startup with the actual screen/window resolution GamePadRealm
// is overlaying (e.g. from glfwGetVideoMode). Computes independent X/Y scale
// factors against the 1920x1080 reference and fills in every variable above.
// Safe to call again if the resolution changes at runtime.
void initScaledGlobals(int actualScreenWidth, int actualScreenHeight);

#endif //GAMEPADREALM_GLOBALS_HPP
