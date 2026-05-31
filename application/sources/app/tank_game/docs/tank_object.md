TECHNICAL DOCUMENTATION: PLAYER TANK CONTROL MODULE

- Component file name: tank_object.cpp
- Accompanying header file: tank_object.h
- Framework Integration: Active Kernel (AK) Event-Driven Task Management

**1. Module Overview**

The `tank_object.cpp` module manages the complete lifecycle, state machine transitions, movement mechanics, and death routines for the player-controlled combat tank avatar. It intercepts system signals from the Active Kernel to update internal data frames and drive smooth visual physics.

**2. Core Properties**

The operational life cycle of the player vehicle is tracked inside the global `static_tank` structure using the following primary metrics:
- `x`: Current horizontal render coordinate on the display canvas.
- `target_x`: The destination coordinate to achieve smooth linear movement.
- `isMoving`: True when interpolating between positions.
- `isExploding`: Triggers the dynamic frame-by-frame destruction particle loop.
- `isDie`: State lock activated once destruction completes, triggering game over countdowns.

**3. Technical Specifications & Mechanical Breakdowns**

3.1. Positional Interpolation (Movement Physics)
Instead of snapping instantly across boundaries, the tank implements a time-sliced linear interpolation mechanism handled during the `TANK_UPDATE_SIG` event loop:
- Incoming control signals (`TANK_MOVE_FORWARD_SIG` / TANK_MOVE_BACKWARD_SIG`) modify the `target_x` bounding metric (rigidly clamped between `0` and `60` pixels to avoid clipping out of the visible screen boundaries).
- For each update cycle, if `x` differs from `target_x`, it creeps incrementally by 1 pixel per frame until alignment is accomplished, setting `isMoving` to false.

3.2. Lifecycle Routines & Game Over Sequence
When critical collision impacts reduce health states, the tank initializes its terminal loop:
- `static_tank.isExploding` is turned on, suspending active movement inputs.
- Upon completion of the 20-frame explosion animation, `isExploding` turns false, and `static_tank.isDie` locks to true.
- Under the dead state, an internal `gameover_timer_countdown` increments on each update loop.
- Once the counter hits a threshold of 40 frames (~2.4 seconds), the handler flushes active memory layout nodes for enemies, stops redundant display timers, and fires the `SCREEN_TRAN(scr_game_over_handle, &scr_game_over)` macro to change viewports.

**4. Event Signal Protocols**

The task intercepts distinct signal macros inside the main `task_tank_handle` switch-case controller:
- `TANK_SETUP_SIG` / `TANK_RESET_SIG`: Overwrites current memory configurations back to clean defaults (spawning at position X=20) and flushes stale countdown timers.
- `TANK_UPDATE_SIG`: Runs continuous mathematical iterations for physics interpolation, tracks animation timers, and ticks the game over countdown routine.
- `TANK_MOVE_FORWARD_SIG` / `TANK_MOVE_BACKWARD_SIG`: Safely offsets the target X destination coordinate boundary by 10 pixels.

**5. Rendering (`tank_draw`)**

To enforce strict separation of concerns, the drawing method executes as a passive layout pass without altering structural data:
- Operational Phase: Draws the raw composite bitmap data array `bitmap_tank` at spatial location (X, 30).
- Destruction Phase: When `isExploding` is active, it sequences through consecutive visual fragments (`bitmap_bum`, `bitmap_bum2`, `bitmap_bum3`) driven by the elapsed explosion timer ticks.
- Terminal Phase: Erases original bitmap assets and outputs a static text warning message overlay ("YOUR TANK IS DESTROY").
