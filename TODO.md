# Project TODO - Exercise 1

## 🚨 Core Architecture (Mandatory)
- [ ] **Main Menu System**
    - [ ] Implement entry screen: `(1) Start Game`, `(8) Instructions`, `(9) Exit`.
    - [ ] Ensure `main()` loop returns to menu after game ends.
- [ ] **Instructions Screen**
    - [ ] Display controls and game rules when '8' is pressed.
- [ ] **Pause & Exit**
    - [ ] `ESC`: Pause game and show "Game Paused".
    - [ ] `ESC` again: Resume game.
    - [ ] `H` (during pause): Exit to Main Menu.
- [ ] **Win Condition**
    - [ ] Define a "Last Room" (Screen 3).
    - [ ] Trigger Game Over when *both* players are in the Last Room.

## 🚧 Active Development
- [ ] **Switches**
    - [ ] Create `Switch` class/struct.
    - [ ] **Logic:** Toggle state (ON/OFF) when Player steps on it.
    - [ ] **Link:** Connect specific Switches to specific Doors.
    - [ ] **Door Logic:** Update `Door` to open only if linked switches are in the correct state.

## 🎮 Selected Gameplay Features
### 1. Springs
- [ ] **Compression:**
    - [ ] Visual: Collapse spring chars when Player moves onto them.
- [ ] **Release:**
    - [ ] Trigger: When Player hits a wall, presses `STAY`, or changes direction.
- [ ] **Launch (Momentum):**
    - [ ] Speed: Equal to number of compressed chars.
    - [ ] Duration: (Compressed chars) squared.
    - [ ] Restriction: Lock movement to release direction (no stopping/reversing).
    - [ ] Sideways: Allow lateral movement while flying.

### 2. Obstacles
- [ ] **Movement:**
    - [ ] Implement `Obstacle::move(dx, dy)`.
- [ ] **Push Logic:**
    - [ ] Force Check: Player Force (1) vs Obstacle Size (N).
    - [ ] Cooperative Push: Sum force if both players push together.
    - [ ] Collision: Ensure space behind obstacle is empty (no Walls/Doors).

### 3. Riddles
- [ ] **Interaction:**
    - [ ] Trigger prompt when Player steps on `?`.
    - [ ] Pause game loop for input.
- [ ] **Outcome:**
    - [ ] **Correct:** Remove riddle, move player onto tile.
    - [ ] **Incorrect:** Keep player at previous tile (optional: life/score penalty).

### 4. Torches & Light
- [ ] **Fog of War:**
    - [ ] Add "Dark Room" flag to `Room` or `Screen`.
    - [ ] Modify `draw()` to render ` ` (space) for floor/walls in dark rooms.
- [ ] **Item Logic:**
    - [ ] If Player holds `TORCH`, override darkness and draw normally.

## 🗺️ Level Design
- [ ] **Screen 1 (Start):**
    - [ ] Basic layout, Keys, Doors.
- [ ] **Screen 2 (Challenge):**
    - [ ] Switches puzzle.
    - [ ] Obstacle blocking path.
    - [ ] Riddle to unlock area.
- [ ] **Screen 3 (End):**
    - [ ] Empty room to trigger win condition.

## 📝 Submission Docs
- [ ] **readme.txt:** Add student IDs (Mandatory).
- [ ] **bonus.txt:** List the extra features implemented (Colors, Extra items).