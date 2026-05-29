TECHNICAL DOCUMENTATION: ENEMY AND TRAP MANAGEMENT SUBSYSTEMS

- Component file names: enemy_object.cpp, trap_object.cpp
- Accompanying header files: enemy_object.h, trap_object.h
- Framework Integration: Active Kernel (AK) Event-Driven Task Management

1. Module Overview
The enemy and trap subsystems handle the automated generation, spatial movement, bounding updates, and physical threat calculations for automated obstacles and targets attempting to destroy the player. 

2. Enemy Subsystem Framework (`enemy_object.cpp`)

2.1. Multi-Archetype Enemy Spawning Profile
The module operates a dynamic entity model tracking various trap threats via an integer identifier (`enemy_type`). Each target exhibits individual dimensions and is bound to a specific lane:

| Enemy Type | Entity Classification | Hitbox Dimensions (WxH) | Screen Vector Elevation |
| :--- | :--- | :--- | :--- |
| `0` | Armored Enemy Ground Tank | 25x21 px | Ground Line ($Y = 33$) |
| `1` | Tactical Aerial Fighter Jet | 25x21 px | Airspace Line ($Y = 5$) |
| `3` | Infantry Ground Soldier Troop | 15x21 px | Ground Line ($Y = 34$) |

2.2. Processing Loops
- `ENEMY_UPDATE_SIG`: Progresses active targets horizontally towards the player's perimeter (`X -= 1`). Targets exiting the display layout edge (X < -25) are automatically recycled.
- `ENEMY_HIT_SIG`: Decrements the instance health pool (`hp`). Upon hitting 0, it switches the flags from active to `isExploding` and posts high-priority rewards via `SCORE_ADD_SIG` to the global scoring framework task.

3. Overhead Ballistic Trap Subsystem (`trap_object.cpp`)
The trap entity operates an autonomous overhead projectile module mimicking mortar strikes or vertical dropping missile traps:
- Randomizing Cooldown: Implements a pseudo-randomized timing block interval calculation (`50 + rand() % 100`) to gap incoming drop frequencies.
- Vector Propagation: When active, the threat initializes at a randomized horizontal layout vector (`X = rand() % 100`) at sub-screen height (Y = -20) and propagates downwards linearly (`Y += 1`). 
- If it exits the baseline boundary (Y > 65), it triggers a silent internal location reset loop without harming the player.

4. Spatial Collision & Bounding Box Queries (Hit-Testing)
Real-time geometric queries are run continuously during the `UPDATE_SIG` cycle using Axis-Aligned Bounding Box (AABB) intersection metrics:

- Cannon Bullet Intersection: Scans active player cannon bullet nodes against current active enemy configurations. On hit, the projectile deactivates and the enemy suffers damage.
- High-Frequency Defense Test: Iterates through the player's active high-frequency `minigun_pool` array to run overlap tests against the falling mortar trap. A valid hit reduces the trap rocket's health, allowing players to actively shoot down dropping traps.
- Player Crash Detection: Runs continuous bounding checks between active hazards (enemies/dropping trap rockets) and the player tank's bounds (30x30 px). Any overlapping validation instantly trips `BUZZER_PlaySound(BUZZER_SOUND_EXPLOSION)` and initiates the player tank's destruction logic.

5. Graphics Rendering Pipeline (`enemy_draw`, `trap_draw`)
Visual loops map multi-byte flash memory assets (`PROGMEM`) to the screen coordinates based on active state flags:
- Normal Combat: Renders targeted asset sprites (`bitmap_enemy_tank`, `bitmap_enemy_air_plane`, etc.) at current spatial vectors.
- Particle Chain Animation: When an explosion state is active, the asset drawing routine halts the source sprite and passes execution to sequential explosion bitmaps (`bitmap_bum`, `bitmap_bum2`, `bitmap_bum3`) to create a fluid, animated impact explosion effect.