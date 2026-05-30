#pragma once

#include <string>
#include <vector>
#include <optional>
#include "../../enumAppCommands.h"
#include "../_UxInterface.h"

enum class MenuItemKind {
    Command,
    Separator,
    Submenu,
    Window
};

struct MenuShortcut {
    char key = 0;

    bool commandOrCtrl = false;
    bool shift = false;
    bool alt = false;
};

struct MenuItem {
    MenuItemKind kind = MenuItemKind::Command;

    std::string label;
    std::string id = "";
    std::optional<AppCommand> command;
    std::optional<MenuShortcut> shortcut;

    std::vector<MenuItem> children;

    bool enabled = true;
    bool checked = false;

    static MenuItem Command(
        std::string label,
        AppCommand command,
        std::optional<MenuShortcut> shortcut = std::nullopt)
    {
        MenuItem item;
        item.kind = MenuItemKind::Command;
        item.label = std::move(label);
        item.command = command;
        item.shortcut = shortcut;
        return item;
    }

    static MenuItem Separator() {
        MenuItem item;
        item.kind = MenuItemKind::Separator;
        return item;
    }

    static MenuItem Submenu(
        std::string label, 
        std::vector<MenuItem> children) 
    {
        MenuItem item;
        item.kind = MenuItemKind::Submenu;
        item.label = std::move(label);
        item.children = std::move(children);
        return item;
    }

    static MenuItem Window(
        std::string label, 
        std::string windowId, 
        bool isOpen) 
    {
        MenuItem item;
        item.kind = MenuItemKind::Window;
        item.command = AppCommand::ToggleWindow;
        item.label = std::move(label);
        item.id = std::move(windowId);
        item.checked = isOpen;
        return item;
    }
};

using MenuBarModel = std::vector<MenuItem>;

inline MenuShortcut Cmd(char key) {
    return MenuShortcut{ key, true, false, false };
}
inline MenuShortcut Alt(char key) {
    return MenuShortcut{ key, false, false, true };
}
inline MenuShortcut CmdShift(char key) {
    return MenuShortcut{ key, true, true, false };
}
inline MenuShortcut CmdAlt(char key) {
    return MenuShortcut{ key, true, false, true };
}

inline MenuItem BuildWindowMenuModel(std::unordered_map<std::string, std::unique_ptr<IWindow>>& windows) {
    MenuBarModel windowMenuItems = MenuBarModel();

    for(auto& window : windows) {
        MenuItem next = MenuItem::Window(
            window.second->GetTitle(), 
            window.second->GetId(),
            window.second->IsOpen()
        );
        windowMenuItems.push_back(next);
    }
   return MenuItem::Submenu("Window", windowMenuItems);
}

inline MenuBarModel BuildMainMenuModel(std::unordered_map<std::string, std::unique_ptr<IWindow>>& windows) {   
    return {
        MenuItem::Submenu("File", {
            MenuItem::Command("New Project", AppCommand::NewProject, Cmd('n')),
            MenuItem::Command("Open Project...", AppCommand::OpenProject, Cmd('o')),

            MenuItem::Separator(),

            MenuItem::Command("Save", AppCommand::Save, Cmd('s')),
            MenuItem::Command("Save As...", AppCommand::SaveAs, CmdShift('s')),
            MenuItem::Command("Save All", AppCommand::SaveAll, CmdAlt('s')),

            MenuItem::Separator(),

            MenuItem::Command("Close Project", AppCommand::CloseProject, Cmd('w')),

#ifndef __APPLE__
            MenuItem::Separator(),
            MenuItem::Command("Preferences…", AppCommand::Preferences),
            MenuItem::Command("Quit", AppCommand::Quit, Cmd('q')),
#endif
        }),

        MenuItem::Submenu("Edit", {
            MenuItem::Command("Undo", AppCommand::Undo, Cmd('z')),
            MenuItem::Command("Redo", AppCommand::Redo, CmdShift('z')),

            MenuItem::Separator(),

            MenuItem::Command("Cut", AppCommand::Cut, Cmd('x')),
            MenuItem::Command("Copy", AppCommand::Copy, Cmd('c')),
            MenuItem::Command("Paste", AppCommand::Paste, Cmd('v')),
            MenuItem::Command("Select All", AppCommand::SelectAll, Cmd('a')),
        }),

        BuildWindowMenuModel(windows),

        MenuItem::Submenu("Help", {
            MenuItem::Command("About CPUForge", AppCommand::About),
        }),
    };
}
