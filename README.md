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
4. RotMGPad will start paused, in order to pause/unpause, press the touchpad on your controller.
5. To quit, press the chord button on your controller.

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

### Missions mode
| Input               | Action                     |
|---------------------|----------------------------|
| A                   | Left click                 |
| B                   | Close missions mode        |

### Battle pass mode
| Input | Action             |
|-------|--------------------|
| B     | Close battle pass  |
| X     | Claim all missions |

### Party mode
| Input               | Action                                                     |
|---------------------|------------------------------------------------------------|
| A                   | Join (party/guild member)                                  |
| B                   | Teleport (unreliable as it uses a fixed offset, TODO: fix) |
