TECHNICAL DOCUMENTATION: GAME SCREENS MANAGEMENT & EEPROM SCORE SYSTEM

- Component file names: scr_banner_game.cpp, scr_game.cpp, scr_game_over.cpp, scr_victory.cpp, scr_topscore_game.cpp
- Framework Integration: Active Kernel (AK) Screen State Machine Framework
- Storage Component: Non-volatile EEPROM (Electrically Erasable Programmable ROM)

**1. Module Overview**

This module acts as the global finite state machine (FSM) infrastructure governing visual interface routing and persistent data metrics. It abstracts separate display viewports into isolated, discrete handler components and securely retains top high-score profiles in physical EEPROM clusters to preserve player records across full hardware reboots.

**2. Unified Screen State Machine**

<img width="1290" height="768" alt="599902487-4928bf2c-301c-4a87-8bf5-3895d0c65a08" src="https://github.com/user-attachments/assets/47fae547-15ca-423e-926a-b3556fa917bd" />

The user interface transitions seamlessly between 5 unique viewports utilizing the Active Kernel platform macro `SCREEN_TRAN(next_handler, &next_screen_struct)`.



---
**3. Component Technical Breakdown**

3.1. Banner Start Menu (`scr_banner_game.cpp`)
- **Visual Architecture:** Renders a fullscreen introductory graphics logo `bitmap_banner_game` (124x60 px) and maintains a volatile dynamic index pointer `menu_index` tracking options: `0: START`, `1: TOP SCORE`, `2: EXIT`.
- **Control Handlers:** - `UP_RELEASED` / `DOWN_RELEASED`: Rotates the selection marker arrow `>` up or down with a modular wraparound check and dispatches an audio click via `BUZZER_PlaySound(BUZZER_SOUND_CLICK)`.
  - `MODE_RELEASED`: Evaluates the finalized `menu_index` block to fire target context transfers (e.g., jumping to the active tactical battle viewport `scr_game`).

3.2. Main Battle Engine Viewport (`scr_game.cpp`)
- **Visual Architecture:** The absolute operational core loop. The rendering pipeline `view_scr_game()` compiles active render passes layer-by-layer: background mountains, urban buildings, rolling ground surface tiles, the player tank, incoming mortar hazards, enemy waves, vegetation, and active ammunition pools.
- **Milestone Spawning Logic:** Monitors live game state score progression metrics asynchronously inside the update tick. Once score registers $\ge 70$, it commands `check_and_spawn_boss()`. Upon achieving an advanced score of $\ge 250$, it invokes the ultimate antagonist stage via `check_and_spawn_boss2()`.
- **Advanced Weapon Interception:** Overrides hardware button tracking flags to handle weapon fire splits: taps on the `MODE` button trigger discrete high-damage heavy `cannon_bullet` launches, while depressing and holding down `MODE` activates continuous rapid-fire `minigun_pool` spray tracking.

3.3. Game Over Screen (`scr_game_over.cpp`)
- **Visual Architecture:** Triggered when the player tank is destroyed. It overlays `bitmap_game_over` alongside a score rendering parse extraction displaying the current run statistics.
- **System Cleanses:** Upon the `SCREEN_ENTRY` hook event, it blocks ongoing gameplay update timers via `timer_remove_attr(...)` to halt asynchronous calculations, fires a sad tone via `BUZZER_PlaySound(BUZZER_SOUND_GOODBYE)`, and pipelines the score payload into the storage validator array.

3.4. Victory Screen (`scr_victory.cpp`)
- **Visual Architecture:** Triggered after the player successfully defeats Boss 2. It overrides display panels with `bitmap_victory`, dumps final score results, and fires a festive theme via `BUZZER_PlaySound(BUZZER_SOUND_MERRY_CHRISTMAS)`.
- **Navigation:** Pressing the `MODE` button releases the hardware lock state, purging display loop ticks, and routes users back to the safe startup master banner interface.

---

**4. High Score Persistence Framework (`scr_topscore_game.cpp`)**

4.1. Memory Allocation Mapping
To safeguard score leaderboards without a file system, the engine interfaces directly with primitive EEPROM byte sectors via physical memory boundaries. It maps a leaderboard table storing three 32-bit unsigned integers (`uint32_t`, 4 bytes each):

| Leaderboard Rank Index | EEPROM Hex Memory Bound | Data Allocation | Size |
| :--- | :--- | :--- | :--- |
| **1st Place Score (Top 1)** | `0x00` - `0x03` | `(uint8_t*)&top_list[0]` | 4 Bytes |
| **2nd Place Score (Top 2)** | `0x04` - `0x07` | `(uint8_t*)&top_list[1]` | 4 Bytes |
| **3rd Place Score (Top 3)** | `0x08` - `0x0B` | `(uint8_t*)&top_list[2]` | 4 Bytes |

4.2. Read & Write Validation Mechanics
- **Asynchronous Pull Extraction:** Upon launching the leaderboard screen viewport (`SCREEN_ENTRY`), the module executes sequential reading invocations:
  ```cpp
  eeprom_read(0x00, (uint8_t*)&top_list[0], 4);
  eeprom_read(0x04, (uint8_t*)&top_list[1], 4);
  eeprom_read(0x08, (uint8_t*)&top_list[2], 4);
  
  Sorting & High-Score Verification Algorithm: When a game loop concludes (entering Game Over or Victory states), update_top_scores(current_score) is called to test the new record against the stored array elements. It performs a descending replacement check:

        If current_score > top_list[0]: Shifts 2nd down to 3rd, 1st down to 2nd, and writes the new score into 1st place (0x00).

        Else if current_score > top_list[1]: Shifts 2nd down to 3rd, and writes the new score into 2nd place (0x04).

        Else if current_score > top_list[2]: Directly overwrites 3rd place (0x08).

All shifting maneuvers trigger immediate physical flash writes via eeprom_write(...) to ensure data persistence, even if the system is abruptly powered down.
