#pragma once

constexpr char WALL_TILE = 'W';
constexpr char OBSTACLE_TILE = '*';
constexpr char KEY_TILE = 'K';
constexpr char SWITCH_ON = '/';
constexpr char SWITCH_OFF = '\\';

// Check if character represents a door - suggested by gemini
inline bool isDoor(char c) { 
	
	return (c >= '1' && c <= '9');
}

inline bool isSwitch(char c) {
	return (c == '\\' || c == '/');
}	