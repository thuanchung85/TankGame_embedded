TECHNICAL DOCUMENTATION: PARALLAX SCROLLING BACKGROUND ENGINE

- Component file names: mountain_object.cpp, building_object.cpp, tree_object.cpp, ground_object.cpp
- Accompanying header files: mountain_object.h, building_object.h, tree_object.h, ground_object.h
- Framework Integration: Active Kernel (AK) Event-Driven Task Management
- Graphic Design Pattern: Multi-Layered Parallax Scrolling (Pseudo-3D Depth Effect)

1. Module Overview
The Parallax Background Engine manages the static and dynamic scenery objects rendered behind the combat entities. To overcome the flat constraints of a 2D monochrome display (OLED 128x64), this engine simulates visual depth (Pseudo-3D) by multi-layering background components and updating their horizontal velocities based on optical depth principles (distant objects scroll slower than foreground objects).

2. Layer Architecture & Velocity Hierarchy
The engine manages 4 separate layers, ordered from the deepest background level to the absolute foreground layer. Velocity throttling is implemented cleanly via `frame_count` modulo dividers inside individual Active Kernel task loops rather than utilizing floating-point math:

| Layer Assignment | Object Component | Asset Dimensions | Base Velocity | Computational Divider Ratio | Optical Depth Class |
| :--- | :--- | :--- | :--- | :--- | :--- |
| **Layer 1 (Deepest)**| `mountain_object` | 90x42 px | -1 px / 6 ticks | `static_mountain.frame_count >= 6` | Sky / Distant Horizon |
| **Layer 2 (Middle)** | `building_object` | 30x30 px | -1 px / 2 ticks | `static_building.frame_count >= 2` | Mid-ground Infrastructure|
| **Layer 3 (Close)** | `tree_object`     | 30x31 px | -1 px / 1 tick  | Instant execution (No divider)   | Near-ground Vegetation |
| **Layer 4 (Ground)** | `ground_object`   | 24x8 px  | +1 px / 1 tick  | Instant looping offset tracking   | Player Baseline Surface|

---

3. Component Technical Breakdown

3.1. Distant Mountain Layer (`mountain_object.cpp`)
- **Mechanics:** Handles the high-altitude horizon backdrop using `bitmap_theMountain`. To simulate extreme distance, it requires 6 game engine update ticks (`MOUNTAIN_UPDATE_SIG`) to move a single pixel leftward.
- **Boundary Recycling:** When the asset vector scrolls completely off-screen (`X < -120`), it instantly warps and buffers ahead to an off-screen safety position (`X = 200 + 90`) to loop seamlessly.

3.2. Mid-Ground Architecture Layer (`building_object.cpp`)
- **Mechanics:** Renders dynamic residential architectural structures. It updates at a moderate pace (1 pixel every 2 updates via `BUILDING_UPDATE_SIG`).
- **Dynamic Asset Randomization:** Utilizes an array pointer lookup framework `building_bitmaps[]` storing 4 unique layout shapes (`bitmap_house1` to `bitmap_house4`). 
- **Recycling & Random Spawning:** When a building exits the screen boundary (`X < -120`), the task triggers a random position padding offset (`130 + rand() % 100`) and randomizes the structural index via `rand() % 4` to prevent repetitive gameplay patterns.

3.3. Near-Ground Vegetation Layer (`tree_object.cpp`)
- **Mechanics:** Manages foreground flora assets using a multi-bitmap storage array (`bitmap_coconutTree`, `bitmap_dryTree`, `bitmap_grassesTree`). Since it is closest to the player's track, it moves at maximum standard velocity (-1 pixel per frame).
- **Recycling Loop:** Upon exceeding boundary conditions (`X < -35`), it warps back to layout location `124 + 35` and assigns a new visual type identifier via `rand() % 3`.

3.4. Dynamic Baseline Tile Surface Layer (`ground_object.cpp`)
- **Mechanics:** Generates a continuous, solid running ground line using modular tile structures (`bitmap_ground`, 24x8 px).
- **Infinite Looping Algorithm:** Instead of tracking large absolute positions, the layout calculation relies on a rolling phase shift counter (`static_ground.x++`). Inside `ground_draw()`, a loop spans the horizontal coordinate canvas width up to 124 pixels in increments of 24, applying a pixel lookup offset: 
  {Render Position} = X % 24
  Once the phase hit counter reaches 24, it zeroes out, achieving a perfectly stable, infinite rendering surface with near-zero memory footprint.

---

4. Event Signal Protocols
Every asset registers an independent Task Handler inside the Active Kernel framework to listen for system-wide clocks:
- `SETUP_SIG` / `RESET_SIG`: Re-initializes coordinate variables to clean default positions (e.g., Mountain to 200, Building to 180, Tree to 130) ensuring clean reboots on game state changes.
- `UPDATE_SIG`: Increments internal frame count clocks, executes velocity boundary calculations, and triggers asset wrapping logic.

5. Rendering Pipeline Integration (Passive Drawing)
All draw methods operate as independent, non-destructive graphics rendering pipelines feeding coordinate variables directly to the core display render agent:
```cpp
void ground_draw();   // Renders tiled ground blocks looping at Y=52
void mountain_draw(); // Renders the horizon backdrop asset at Y=0
void tree_draw();     // Renders dynamic trees relative to the terrain at Y=21 (52 - 31)
void building_draw(); // Renders randomized residential infrastructure tiles at Y=10