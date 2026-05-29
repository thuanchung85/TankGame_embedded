TECHNICAL DOCUMENTATION: BOSS & BOSS2 CONTROL MODULES

- Component file names: boss_object.cpp, boss2_object.cpp
- Accompanying header files: boss_object.h, boss2_object.h
- Architecture: Event-Driven Framework (Active Kernel) with Finite State Machine (FSM)

1. Module Overview
The boss control modules manage the lifecycle, state transitions, movement mechanics, weapon firing systems, and collision detection for the game's primary antagonists: 
- Boss 1 (Mini Boss): A high-speed combat helicopter utilizing dynamic vertical surfing and missile payloads.
- Boss 2 (Big Boss): A heavily armored ground dreadnought with a multi-phase trajectory cannon.

Both modules leverage the Event-Driven architecture of the Active Kernel to offload computing cycles and isolate execution logic into individual Task Handlers.

2. Finite State Machine (FSM) Structure
Both boss entities track their operational lifecycle using state variables embedded within their global structs (`static_boss` and `static_boss2`):

| State Property | Data Type | Function Description |
| :--- | :--- | :--- |
| `is_active` | `bool` | True when the boss is active on screen, running movement and weapon AI. |
| `is_exploding` | `bool` | True during the frame-by-frame destruction sequence (triggers explosion animations). |
| `isDie` | `bool` | True when completely neutralized. Halts actions and initiates Game Over / Victory sequences. |

3. Technical Specifications & Mechanics Breakdowns

3.1. Boss 1 (Mini Boss - Helicopter)
- Core Mechanics: Spawns when specific score milestones are achieved. It enters from the right side of the screen and performs a continuous vertical "surfing" pattern (moving up and down within predefined bounding constraints).
- Weapon System: Periodically fires a fast-moving horizontal project (`rocket`) aimed directly towards the player's lane.
- Collision Clusters:
  - `check_collision_cannon_bullets_with_boss()`: Hitbox test (60x36 px) against the player's active cannon projectile. Decrements HP and sends a common message on hit.
  - `check_collision_tank_with_boss()`: Multi-case collision handler checking if (1) the Boss's rocket impacts the player tank, or (2) the Boss body itself crashes into the player tank. Triggers immediate player explosion.

3.2. Boss 2 (Big Boss - Heavy Tank)
- Core Mechanics: Actively updates its horizontal coordinate (`X-axis`) using a ping-pong velocity model (bouncing left and right between boundaries X=60 and X=120) to suppress player positions.
- Weapon System: Fires an advanced heavy artillery shell (`cannon_bullet`) that tracks along both X and Y dimensions to intersect the player's baseline at coordinate Y=40.
- Dynamic Health Bar: Calculates and renders a real-time scaling structural health indicator over the UI viewport layer based on current-to-max HP ratios.
- Game Loop Control (Victory Transition): When neutralized (`isDie == true`), it triggers a critical sequence that handles the 20-frame victory countdown delay, terminates the main graphics timer loop, cleans up global RAM nodes for all entities, and safely triggers the screen transition macro `SCREEN_TRAN` to invoke the Victory Screen.

4. Event Signal Handling (API Protocol)
The modules listen for and act on distinct pure and common signals handled inside `task_boss_handle` and `task_boss2_handle`:

- `BOSSx_SETUP_SIG` / `BOSSx_RESET_SIG`: Invokes the internal data reset functions, clearing weapon instances, resetting structural coordinates back to defaults, and restoring HP configurations.
- `BOSSx_SPAWN_SIG`: Performs safety assertions on current life flags, wipes stale memory variables, and shifts entity properties into an active combat-ready state.
- `BOSSx_HIT_SIG`: extracts raw payload buffers containing damage metrics, decrements health pools, and checks for terminal states. Upon fatal damage, it shifts flags from active to exploding, activates explosion buzzer sound waves, and dispatches bonus point rewards via `SCORE_ADD_SIG` to the Score Task.
- `BOSSx_UPDATE_SIG`: Cascades continuous computational steps during game loops, shifting positional metrics, running cooldown intervals, evaluating bounding constraints, and driving particle system indexes.

5. Graphics Rendering Pipeline (Draw Implementation)
To strictly enforce the separation of concerns, the rendering methods (`boss_draw()` and `boss2_draw()`) do not contain state modification logic. They execute purely as passive render passes:

- Combat State: Renders the composite multi-byte entity sprite maps extracted from Flash arrays alongside active weapon graphics and health progress metrics.
- Exploding State: Iteratively draws concentric geometric primitive rings (`view_render.drawCircle`) scaled dynamically by elapsed timers layered with timed explosion sprites (`bitmap_bum`, `bitmap_bum2`, `bitmap_bum3`) to form a fluid animated destruction sequence.
- Dead State: Terminate rendering of original entities and print a clean text status indicator overlay ("BOSS IS DESTROY") before handing complete viewport control over to the game screen engine.