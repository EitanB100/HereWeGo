#pragma once

constexpr char WALL_TILE = 'W';
constexpr char OBSTACLE_TILE = '*';
constexpr char KEY_TILE = 'K';
constexpr char SWITCH_ON = '/';
constexpr char SWITCH_OFF = '\\';
constexpr char TORCH_TILE = '!';
constexpr char UNKNOWN_TILE = 'R';
constexpr char GLASS_TILE = 'G';
constexpr char SPRING_TILE = '#';


// Check if character represents a door - suggested by gemini
inline bool isDoorTile(char c) {

	return (c >= '1' && c <= '9');
}

inline bool isSwitchTile(char c) {
	return (c == '\\' || c == '/');
}

inline bool isTorchTile(char c) {
	return (c == '!');
}

inline bool isUnKownTile(char c) {
	return (c == 'R');
}