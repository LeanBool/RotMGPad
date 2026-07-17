# RotMGPad

Play Realm of the Mad God with a controller instead of a mouse and
keyboard. RotMGPad runs alongside the game on Linux, reads your gamepad,
and turns button presses and stick movements into mouse clicks and key
presses. A light
on-screen overlay shows you which "mode" the controller is currently in, so
you always know what your buttons will do.

## Getting started

1. Plug in your controller.
2. Build the app (see below) or run the provided executable.
3. Launch RotMGPad, then switch to your game.
4. Play using the controls below — RotMGPad keeps running in the
   background and reacts automatically as you open and close menus.

### Building from source

```bash
mkdir build && cd build
cmake ..
cmake --build .
./RotMGPad
```

## Controls

### Normal mode

| Input               | Action                |
|---------------------|-----------------------|
| Right stick         | Move the mouse cursor |
| Left stick (click)  | Open/close chat       |
| Right stick (click) | Open/close the map    |
| A                   | Left click / interact |
| B                   | Use item slot 2       |
| X                   | Nexus                 |
| Y                   | Toggle auto shoot     |
| D-pad up            | Open the pickup tray  |
| D-pad left          | Toggle camera offset  |
| D-pad right         | Switch inventory tabs |
| D-pad down          | Reset camera rotation |
| Left shoulder       | Use item slot 1       |
| Right shoulder      | Use spell             |
| Start               | Open inventory        |
| Back                | Open wheel overlay    |

### Inventory mode

Once you're in a RotMG menu, the D-pad and shoulder buttons move you between
a stack of related screens instead of controlling your character:


| Input               | Action                     |
|---------------------|----------------------------|
| A                   | Double left click          |
| B                   | Return to normal mode      |
| X                   | Drop item                  |
| Y                   | Deposit item               |
| D-pad               | Move through inventory     |
| Left shoulder       | Use item slot 2            |
| Right shoulder      | Move item to potion slot 1 |
| Start               | Close inventory            |
| Back                | Open wheel overlay         | 

### Top menu bar

When you reach the top menu bar, D-pad left/right moves between its icons
and **A** opens that section — attributes, pets, or missions, depending on
which icon is highlighted. **B** returns you to the menu bar from any of
those sections. **Right stick** can be used to scroll through supported screens.

### Missions and battle pass

- In the missions list, the left stick moves a cursor and **A** triggers left click to claim a mission.
- In a mission preview, the right stick scrolls the details and **A** takes
  you to the full list.
- The battle pass screen opens its own view where **X** claims everything at once.

### Party / chat

- The left stick moves a cursor over the chat panel.
- **A** joins the sender's location through party join; **B** teleports to them (not working properly); **X** opens the
  chat box (through pressing enter).

