#pragma once
#include <string>
#include <string_view>
#include "_types.h"

enum class AppCommand {
    NewProject,
    OpenProject,
    Save,
    SaveAs,
    SaveAll,
    CloseProject,
    Preferences,
    Quit,

    Undo,
    Redo,
    Cut,
    Copy,
    Paste,
    SelectAll,

    ToggleWindow,

    OpenFile,

    About,
};

static std::string_view ToString(AppCommand command) {
    switch (command) {
        case AppCommand::NewProject:   return "NewProject";
        case AppCommand::OpenProject:  return "OpenProject";
        case AppCommand::Save:         return "Save";
        case AppCommand::SaveAs:       return "SaveAs";
        case AppCommand::SaveAll:      return "SaveAll";
        case AppCommand::CloseProject: return "CloseProject";
        case AppCommand::Preferences:  return "Preferences";
        case AppCommand::Quit:         return "Quit";

        case AppCommand::Undo:         return "Undo";
        case AppCommand::Redo:         return "Redo";
        case AppCommand::Cut:          return "Cut";
        case AppCommand::Copy:         return "Copy";
        case AppCommand::Paste:        return "Paste";
        case AppCommand::SelectAll:    return "SelectAll";

        case AppCommand::ToggleWindow: return "ToggleWindow";

        case AppCommand::About:        return "About";
    }

    return "Unknown";
}

struct AppCommandRequest {
    AppCommand command = AppCommand::About;
    std::string id; //window id for ToggleWindow
};