#pragma once
#include <string>

struct WindowMenuItemModel {
    std::string id;
    std::string title;
    bool open;
    bool enabled = true;
};