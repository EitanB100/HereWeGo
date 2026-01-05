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
   - (8) Instructions: View controls (updates accordingly).
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

1. File Format (.screen)
   - We separated the map layout ([MAP]) from the object metadata ([KEYS], [DOORS], [POTIONS]).
   - The loader reads the visual map first to find coordinates, then links them 
     to the metadata sections using IDs.
   - This allows for flexible level design where visual placement is decoupled 
     from logic properties.

2. New Element: Potion
   - We chose to add a healing mechanic. Potions appear on the map as 'P'.
   - Picking one up instantly restores 5 HP (up to Max HP).
   - This adds a layer of strategy to the bomb/riddle damage mechanics.

3. Memory Management
   - `std::vector` is used for managing dynamic lists of game objects.
   - `std::unique_ptr` is used for Switches to ensure safe memory handling.
   - The `Game` class manages the high-level loop, ensuring the `main()` function 
     remains clean and avoids recursion problems (as per requirements).

4. Riddles
   - Riddles are loaded from `riddles.txt`. 
   - Answering incorrectly deals damage (HP penalty) but allows the player to retry.
   - Answering correctly clears the riddle tile.

--- References ---
- Bresenham's Line Algorithm logic used for Torch/Bomb Line-of-Sight.
- Console color handling adapted from Windows API documentation.
