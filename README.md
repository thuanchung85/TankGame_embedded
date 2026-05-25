

https://github.com/user-attachments/assets/0a2d2ca1-4aab-4f7d-9217-8a006b4d772b



## TANK GAME Embedded- AKOS -STM32L151

This classic tank shooting game is optimized for embedded systems using C/C++ and hardware performance optimization.
1. HOW TO RUN:
   - Open terminal, and go to folder application by command:  cd '/home/luong/workspace/Sources/ChungGame_embedded/application' 
   - Build and run by command:  sudo make flash dev=/dev/ttyUSB0
     
2. KEY FEATURES:

   A. GAMEPLAY:
   - You have 1 tank with 1 live point (HP). Your mission is move and destroy enemies get about 200 point and destroy the big BOSS.
   - Use button UP and DOWN to move your tank, try avoid hit by enemies.
   - Use Mode button for tank fire cannon, it will take 50ms for reload next cannon bullet.
   - User hold MODE button for 0.5s, then tank will fire minigun to destroy air targets.
   <div align="center">
      <img width="700" height="" src="https://i.postimg.cc/brBvWn91/Screenshot-2026-05-18-143159.jpg"/>
   </div>
   B. TECHNICAL FEATURES (Embedded): 
   
   Optimized memory management (Dynamic/Static Memory management), multitasking or game loop handling without interrupts/timers, optimized screen refresh rate (Frame rate/FPS control), button debouncing control...

      
4. USE CASE:
   <div align="center">
   <img width="700" height="" src="https://i.postimg.cc/CLYjjsDC/user-case.png"/>
   </div>

5. Overall:
   <div align="center">
   <img width="700" height="" alt="Screenshot 2026-05-23 122801" src="https://github.com/user-attachments/assets/a588e88e-406c-417a-86dd-d25ba867cfba" />
   </div>
   
6. Main Sequential Flow:

   Player → Tank: Send movement or fire commands.

   Tank → Game Engine: Update position, generate ammunition.
   
   Game Engine → Enemy System: Generate new enemies every second.
   
   Tank ↔ Enemy System: Check collisions, process damage.
   
   Game Engine → Score System: Add points based on the type of enemy destroyed.
   
   Game Engine → Boss: Activate when the score threshold is reached.
   
   Boss ↔ Tank: Engage in combat, check victory or game over.
   
   Game Engine → EEPROM: Save score upon victory.
   
   Game Engine → Menu Screen: Display top score, restart or exit options.

   

7. CASE VICTORY SEQUENCE:
<div align="center">
<img width="700" height="" alt="Screenshot 2026-05-23 123852" src="https://github.com/user-attachments/assets/4f064502-7415-4f96-81fa-54198e93f4bb" />
</div>

7. CASE GAMEOVER SEQUENCE:
<div align="center">
<img width="700" height="" alt="gameover" src="https://github.com/user-attachments/assets/d2dcc6aa-bce2-4329-b02a-14a54ff68a00" />
</div>

8. GAMEPLAY SEQUENCE:
<div align="center">
<img width="700" height="" alt="sequenceDiagram" src="https://github.com/user-attachments/assets/0eb040e5-76dc-4d22-af58-050e429d48e6" /><svg xmlns="http://www.w3.org/2000/svg" xmlns:xlink="http://www.w3.org/1999/xlink" version="1.1" width="1917.316850081621" height="1791.4943114520001"></svg>
</div>

9. BITMAP DATA:

   Download link: https://drive.google.com/drive/folders/1s_6J5NmODzScs4ymTYlF7k-WHXQxQIwn?usp=drive_link

10. SOURCE CODE (in 4 folders):
    - bitmap_data: The location where bitmap data of game objects is stored.
    - game_managers: This is where in-game events like victory or game over are managed, as well as boss summoning. It also manages collision detection between objects in the game.
    - game_objects: This is where all the game objects are located, such as the player's tank, enemy objects like soldiers, mines, tanks, planes, boss enemies, various types of trees, mountains, buildings, and the ground. It also contains bullets and booby trap rockets.
    - game_screens: This area contains three game screens: the game banner with the game menu, the main game screen, and the top score screen. 

## AK Embedded Base Kit - STM32L151 - HARDWARE

[<img src="hardware/images/ak-foundation-logo.png" width="240"/>](https://github.com/the-ak-foundation)

Heartfelt thanks to [EPCB](https://epcb.vn/pages/frontpage) for standing with the AK Foundation and empowering our mission.

AK Embedded Base Kit is an evaluation kit for advanced embedded software learners.
- The KIT integrates 1.54" Oled LCD, 3 push buttons, and 1 Buzzers that play music, to learn the event-driven system through hands-on game machine design.
- The KIT also integrates RS485, Qwiic Connect System, and Grove Ecosystems, suitable for prototyping practical applications in embedded systems.

[<img src="hardware/images/ak-embedded-base-kit-version-3.jpg" width="480"/>](https://epcb.vn/products/ak-embedded-base-kit-lap-trinh-nhung-vi-dieu-khien-mcu)

### Memory map
- [ 0x08000000 ] : **Boot** [[ak-base-kit-stm32l151-boot.bin]](https://github.com/ak-embedded-software/ak-base-kit-stm32l151/blob/main/hardware/bin/ak-base-kit-stm32l151-boot.bin)
- [ 0x08002000 ] : **BSF** [ Memory for data sharing between Boot and Application ]
- [ 0x08003000 ] : **Application** [[ak-base-kit-stm32l151-application.bin]](https://github.com/ak-embedded-software/ak-base-kit-stm32l151/blob/main/hardware/bin/ak-base-kit-stm32l151-application.bin)

**Note:** After loading boot & application firmware, you can use [AK - Flash](https://github.com/ak-embedded-software/ak-flash) to load the application directly through the **USB** port on the KIT
```sh
ak_flash /dev/ttyUSB0 ak-base-kit-stm32l151-application.bin 0x08003000
```

### Hardware
**Schematic** [[schematic-ak-embedded-base-kit-version-3.pdf]](https://github.com/ak-embedded-software/ak-base-kit-stm32l151/blob/main/hardware/schematic/schematic-ak-embedded-base-kit-version-3.pdf)

[<img src="hardware/images/board-view-top.png" width="480"/>](https://epcb.vn/products/ak-embedded-base-kit-lap-trinh-nhung-vi-dieu-khien-mcu)

[<img src="hardware/images/board-view-bottom.png" width="480"/>](https://epcb.vn/products/ak-embedded-base-kit-lap-trinh-nhung-vi-dieu-khien-mcu)

### Reference
| Topic | Link |
| ------ | ------ |
| Blog & Tutorial | https://epcb.vn/blogs/ak-embedded-software |
| Where to buy KIT? | https://epcb.vn/products/ak-embedded-base-kit-lap-trinh-nhung-vi-dieu-khien-mcu |
