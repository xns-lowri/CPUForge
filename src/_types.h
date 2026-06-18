#pragma once
#include <vector>
using UUID = uint64_t; //uuid type used everywhere

//generic key command struct, for keyboard handler and menu bar data
struct KeyboardCommand {
    char key = 0;

    bool ctrl = false;
    bool shift = false;
    bool alt = false;
};

//helpers for building keyboard commands
inline KeyboardCommand Ctrl(char key) {
    return KeyboardCommand{ key, true, false, false };
}
inline KeyboardCommand Shift(char key) {
    return KeyboardCommand{ key, false, true, false };
}
inline KeyboardCommand Alt(char key) {
    return KeyboardCommand{ key, false, false, true };
}
inline KeyboardCommand CtrlShift(char key) {
    return KeyboardCommand{ key, true, true, false };
}
inline KeyboardCommand CtrlAlt(char key) {
    return KeyboardCommand{ key, true, false, true };
}
inline KeyboardCommand AltShift(char key) {
    return KeyboardCommand{ key, false, true, true };
}
inline KeyboardCommand CtrlAltShift(char key) {
    return KeyboardCommand{ key, true, true, true };
}