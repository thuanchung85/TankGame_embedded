TECHNICAL DOCUMENTATION: GRAPHICS ENCODING MODULE (BITMAP)

    - Component file name: screens_bitmap.cpp
    - Accompanying header file: screens_bitmap.h
    - Target platform: Embedded System / Microcontroller using graphics library.

1. Module Overview
    The screens_bitmap.cpp module acts as the static graphics resource repository for the Tank game. Due to the limited RAM of microcontrollers or embedded chips, all image data (Sprite/UI) in this file is structured as monochrome unsigned char arrays and is configured to be stored directly in Program Memory (Flash Memory) instead of being loaded into RAM during operation.

2. Core Technical Components
    - Optimization Keyword: PROGMEM
    - Nature: A macro in the embedded environment (as specified by AVR/ARM).
    - Function: Instructs the compiler to place these graphics data arrays into Flash memory.
    - Benefits: Completely frees up static RAM space, preventing stack/heap overflow errors during game execution.

3. Detailed List of Graphics Resources (Sprites)
    Below is a table summarizing all graphics structures defined in the file, categorized by function:

    3.1. Entities & Combat Group
    - bitmap_boss2: 42x34px for big boss draw, it is a big tank can fire cannon bullet. 
    - bitmap_boss: 60x36px for mini boss draw, it is a helicopter can fire rocket.
    - bitmap_boss2_cannon_bullet: cannon bullet 13x5px
    - bitmap_boss_rocket: rocket of mini boss 17x11px
    - bitmap_tank 30×30 px 120 Bytes The player's tank. Designed in a block shape, integrated with a cannon barrel.
    - bitmap_enemy_tank 25×21 px 66 Bytes Enemy tanks. Smaller in size than our tanks to create a difference in hitbox.
    - bitmap_enemy_air_plane 25×21 px 66 Bytes Enemy fighter jets, providing aerial attack support.
    - bitmap_enemy_mine 22×21 px 63 Bytes Landmines planted by the enemy act as obstacles that cause damage.
    - bitmap_enemy_rocket 11×17 px 34 Bytes Enemy thermal-range missiles or large-caliber shells.
    - bitmap_enemy_troop 15×21 px 42 Bytes The enemy infantry soldiers move on the ground.
    - bitmap_myTank_cannon_bullet 5×3 px 3 Bytes Player's main cannon shells (High damage, normal firing speed).
    - bitmap_myTank_gun_bullet 2×1 px 1 Byte Secondary gun ammunition / Upgraded Minigun (Ultra-small size, extremely fast firing rate).
   
    3.2. Environment & Terrain Group:
    - bitmap_ground: 24×8 px 24 Bytes A sample ground surface block (Tile). Used for looping to create a ground terrain surface.
    - bitmap_theMountain: 90×42 px 472 Bytes The large mountain range forms the background, increasing the spatial depth for the game.
    - bitmap_coconutTree: 30×31 px 120 Bytes Decorative object: Tropical environment coconut tree.
    - bitmap_dryTree: 30×31 px 104 Bytes Decorative object: Dry tree from the conflict zone.
    - bitmap_grassesTree: 30×31 px 120 Bytes Decorative object: Dense clump of grass.
    - bitmap_house1 ... house4: 30×30 px 120 Bytes / file Models of residential architecture, buildings, used to construct different maps or levels
   
    3.3. Effects & UI Group: 
   
    - bitmap_HP: 10×8 px 16 Bytes Heart Icon (Blood). Displays the player's health bar on the UI.
    - bitmap_bum: 28×20 px 80 Bytes Stage 1 explosion effect (When the bullet hits the target).
    - bitmap_bum2: 30×26 px 104 Bytes Stage 2 explosion effect (Spreading).
    - bitmap_bum3: 30×31 px 120 Bytes Stage 3 explosion effect (Completely dissipated). Create a smooth explosion animation sequence.
    - bitmap_banner_game: 124×60 px 930 Bytes The welcome screen banner (Start Menu) displays the game name.
    - bitmap_game_pause: 124×60 px 930 Bytes The full-screen interface displays when the player pauses the game.
    - bitmap_game_over: 124×60 px 930 Bytes The full-screen interface displays when the player runs out of HP (Game Over).
    - bitmap_victory:  124×60 px 930 Bytes The full-screen interface displays when the player victory.

    3.4. helper functions: check_collision function use for hit test between game objects.
   
5. Decoding and Display Principles (Hex to Pixel Encoding)
    All the data arrays above follow the principle of encoding monochrome images specifically for embedded displays (OLED/LCD):

    - Data format: Each bit 1 represents a pixel that is turned on (light color/drawing color), and bit 0 represents a pixel that is turned off (background color).

    - Scanning mechanism: Data is scanned in rows from left to right, from top to bottom.

    - How to calculate array size: Size (Bytes) = 8Width × Height (For example, with a bitmap_tank of size 30×30: Each 30px row needs 30/8 = 4 Bytes. The total of 30 rows will be 4×30 = 120 Bytes, which is completely accurate with the actual code).

6. Instructions for Using & Calling Functions in Source Code
    To draw these resources on the screen, programmers need to use bitmap drawing functions from Flash memory of compatible graphics libraries.

    Example function call syntax: display.drawBitmap(player_X, player_Y, bitmap_tank, 30, 30, WHITE);

Download link: https://drive.google.com/drive/folders/1s_6J5NmODzScs4ymTYlF7k-WHXQxQIwn?usp=drive_link