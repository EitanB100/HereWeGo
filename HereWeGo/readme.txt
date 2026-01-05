C++ Project - Text Adventure Game (Exercise 2)
================================================================================
Submitters:
ID: [212321525]  | Name: [Eitan Bar]
ID: [322382631]  | Name: [Harel Ben-Abir]
================================================================================

--- Files Description ---
1. Source Code: 
   - main.cpp, Game.cpp/h, Room.cpp/h, Player.cpp/h, Level_Loader.cpp/h, etc.
   - Potion.cpp/h (New element implementation)
2. Level Files: 
   - adv_world_01.screen, adv_world_02.screen, adv_world_03.screen
   - Level_Template.txt (For creating new levels)
3. Data Files:
   - riddles.txt (Database of riddles linked by ID)
4. Documentation:
   - readme.txt
   - bonus.txt
   - Level_Creator_Guide.txt (Instructions on how to build custom maps)

--- How to Run ---
1. Run the executable.
2. Main Menu:
   - (1) Start Game: Begins from Level 1.
   - (7) Settings: Rebind keys or Toggle Colors.
   - (8) Instructions: View default controls.
   - (2/3/4) Debug Shortcuts: Skip to specific levels or ending screen.

--- Default Controls (Case Insensitive) ---
| Action     | Player 1 | Player 2 |
|------------|----------|----------|
| UP         | W        | I        |
| DOWN       | X        | M        |
| LEFT       | A        | J        |
| RIGHT      | D        | L        |
| STAY       | S        | K        |
| DISPOSE    | E        | O        |
* Note: Controls can be changed in the Settings menu.

--- Design Decisions & Implementation Details ---

1. Grid & Coordinates
   - The game uses a 0-indexed coordinate system (0,0 is Top-Left).
   - The Map is managed by the `Room` class, which handles collision detection 
     and object interaction.
   - The HUD (Legend) position is determined by the 'L' character in the 
     .screen file.

2. Spring Physics
   - Springs implement a momentum system. 
   - "Compression": Moving onto a spring increases potential energy.
   - "Launch": Moving off/hitting a wall releases energy.
   - Flight Time: Calculated as (Force^2). While in flight, the player cannot 
     change direction but can strafe sideways.

3. File Format (.screen)
   - We separated the map layout ([MAP]) from the object metadata ([KEYS], [DOORS], [POTIONS]).
   - The loader reads the visual map first to find coordinates, then links them 
     to the metadata sections using IDs.
   - This allows for flexible level design where visual placement is decoupled 
     from logic properties.

4. New Element: Potion
   - We chose to add a healing mechanic. Potions appear on the map as 'P'.
   - Picking one up instantly restores 5 HP (up to Max HP).
   - This adds a layer of strategy to the bomb/riddle damage mechanics.

5. Memory Management
   - `std::vector` is used for managing dynamic lists of game objects.
   - `std::unique_ptr` is used for Switches to ensure safe memory handling.
   - The `Game` class manages the high-level loop, ensuring the `main()` function 
     remains clean and avoids recursion problems (as per requirements).

6. Riddles
   - Riddles are loaded from `riddles.txt`. 
   - Answering incorrectly deals damage (HP penalty) but allows the player to retry.
   - Answering correctly clears the riddle tile.

--- Known Issues / Limitations ---
- Console Window Size: The game expects a standard 80x25 terminal. 
  Resizing the window manually during gameplay may cause visual artifacts.
- Color: If the terminal does not support ANSI colors or Windows Console Attributes, 
  toggle "Color Mode" to OFF in the settings.

--- References ---
- Bresenham's Line Algorithm logic used for Torch/Bomb Line-of-Sight.
- Console color handling adapted from Windows API documentation.
