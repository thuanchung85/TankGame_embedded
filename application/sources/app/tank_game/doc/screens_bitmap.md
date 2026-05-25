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
    [Entities & Combat Group.pdf](https://github.com/user-attachments/files/28209502/Entities.Combat.Group.pdf)
   
    3.2. Environment & Terrain Group
    [Environment & Terrain Group.pdf](https://github.com/user-attachments/files/28209499/Environment.Terrain.Group.pdf)
   
    3.3. Effects & UI Group
    [Effects & UI Group.pdf](https://github.com/user-attachments/files/28209489/Effects.UI.Group.pdf)

5. Decoding and Display Principles (Hex to Pixel Encoding)
    All the data arrays above follow the principle of encoding monochrome images specifically for embedded displays (OLED/LCD):

    - Data format: Each bit 1 represents a pixel that is turned on (light color/drawing color), and bit 0 represents a pixel that is turned off (background color).

    - Scanning mechanism: Data is scanned in rows from left to right, from top to bottom.

    - How to calculate array size: Size (Bytes) = 8Width × Height (For example, with a bitmap_tank of size 30×30: Each 30px row needs 30/8 = 4 Bytes. The total of 30 rows will be 4×30 = 120 Bytes, which is completely accurate with the actual code).

6. Instructions for Using & Calling Functions in Source Code
    To draw these resources on the screen, programmers need to use bitmap drawing functions from Flash memory of compatible graphics libraries.

    Example function call syntax: display.drawBitmap(player_X, player_Y, bitmap_tank, 30, 30, WHITE);

Download link: https://drive.google.com/drive/folders/1s_6J5NmODzScs4ymTYlF7k-WHXQxQIwn?usp=drive_link
