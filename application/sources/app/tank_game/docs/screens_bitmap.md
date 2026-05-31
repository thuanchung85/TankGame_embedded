TECHNICAL DOCUMENTATION: GRAPHICS ENCODING MODULE (BITMAP)

- Component file name: screens_bitmap.cpp
- Accompanying header file: screens_bitmap.h
- Target platform: Embedded System / Microcontroller using a graphics library.

**1. Module Overview**

The `screens_bitmap.cpp` module acts as the static graphics resource repository for the Tank game. Due to the severely limited RAM of microcontrollers and embedded chips, all image data (Sprites/UI) in this file is structured as monochrome unsigned char arrays and is configured to be stored directly in Program Memory (Flash Memory) instead of being loaded into RAM during runtime.

**2. Core Technical Components**

- Optimization Keyword: `PROGMEM`
- Nature: A compiler attribute macro in the embedded environment (as specified by AVR/ARM toolchains).
- Function: Instructs the compiler to place these graphics data arrays into Flash memory (Program Memory).
- Benefits: Completely frees up static RAM space, preventing stack/heap overflow errors during game execution.

**3. Detailed List of Graphics Resources (Sprites)**

Below is a summary of all graphics structures defined in the file, categorized by function:

3.1. Entities & Combat Group
- `bitmap_boss2`: 42x34 px (204 Bytes)| The main big boss. It is a heavy tank capable of firing cannon bullets.
- `bitmap_boss`: 60x36 px (288 Bytes)| The mini-boss. It is a combat helicopter capable of firing rockets.
- `bitmap_boss2_cannon_bullet`: 13x5 px (10 Bytes)| High-caliber cannon shells fired by Boss 2.
- `bitmap_boss_rocket`: 17x11 px (33 Bytes)| Homing rockets fired by the mini-boss.
- `bitmap_tank`: 30x30 px (120 Bytes) | The player's tank. Designed with a solid block shape and an integrated cannon barrel.
- `bitmap_enemy_tank`: 25x21 px (66 Bytes) | Enemy tanks. Smaller than the player's tank to differentiate the hitbox.
- `bitmap_enemy_air_plane`: 25x21 px (66 Bytes) | Enemy fighter jets providing aerial attack support.
- `bitmap_enemy_mine`: 22x21 px (63 Bytes) | Landmines planted by enemies acting as destructive static obstacles.
- `bitmap_enemy_rocket`: 11x17 px (34 Bytes) | Falling trap rockets or large-caliber artillery shells.
- `bitmap_enemy_troop`: 15x21 px (42 Bytes) | Enemy infantry soldiers moving on the ground.
- `bitmap_myTank_cannon_bullet`: 5x3 px (3 Bytes) | Player's main cannon shells (High damage, normal cooldown).
- `bitmap_myTank_gun_bullet`: 2x1 px (1 Byte) | Secondary gun ammunition / Upgraded Minigun (Ultra-small size, extremely fast firing rate).

3.2. Environment & Terrain Group
- `bitmap_ground`: 24x8 px (24 Bytes) | A sample ground surface tile. Used for seamless looping to generate the ground terrain.
- `bitmap_theMountain`: 90x42 px (472 Bytes) | Large background mountain range to increase spatial depth.
- `bitmap_coconutTree`: 30x31 px (120 Bytes) | Decorative object: Tropical environment coconut tree.
- `bitmap_dryTree`: 30x31 px (104 Bytes) | Decorative object: Dry withered tree from the conflict zone.
- `bitmap_grassesTree`: 30x31 px (120 Bytes) | Decorative object: Dense clump of grass.
- `bitmap_house1` ... `house4`: 30x30 px (120 Bytes each) | Residential architecture building models used to construct different maps/levels.

3.3. Effects & UI Group
- `bitmap_HP`: 10x8 px (16 Bytes) | Heart icon. Displays the player's health points on the UI heads-up display.
- `bitmap_bum`: 28x20 px (80 Bytes) | Stage 1 explosion effect (Impact point).
- `bitmap_bum2`: 30x26 px (104 Bytes) | Stage 2 explosion effect (Spreading flare).
- `bitmap_bum3`: 30x31 px (120 Bytes) | Stage 3 explosion effect (Dissipating smoke). Creates a smooth frame-by-frame explosion animation sequence when linked.
- `bitmap_banner_game`: 124x60 px (930 Bytes) | The title screen banner (Start Menu) displaying the game logo.
- `bitmap_game_pause`: 124x60 px (930 Bytes) | Full-screen UI displayed when the player pauses the game.
- `bitmap_game_over`: 124x60 px (930 Bytes) | Full-screen UI displayed when the player runs out of HP.
- `bitmap_victory`: 124x60 px (930 Bytes) | Full-screen UI displayed upon defeating the final boss.

3.4. Helper Functions
- `check_collision`: An optimized geometric AABB (Axis-Aligned Bounding Box) function used for real-time hit testing between game objects.

**4. Decoding and Display Principles (Hex-to-Pixel Encoding)**

All the data arrays above follow the principle of encoding monochrome images specifically for embedded displays (OLED/LCD):

- Data Format: Each bit `1` represents an active pixel (light color/drawing color), and bit `0` represents an inactive pixel (background color).
- Scanning Mechanism: Data is scanned in rows horizontally from left to right, and vertically from top to bottom.
- Array Size Calculation Formula: size = array.length

Example:* For `bitmap_myTank_gun_bullet`  = 1 Byte. 'bitmap_boss2' = 204 Bytes

**5. Instructions for Using & Calling Resources in Source Code**

To render these resources on the OLED display, developers must call the bitmap rendering API that reads directly from the microcontroller's Flash memory.

Example function call syntax:
```cpp
view_render.drawBitmap(player_X, player_Y, bitmap_tank, 30, 30, WHITE);
