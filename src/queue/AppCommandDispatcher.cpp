// AppCommandDispatcher.cpp
#include <fmt/core.h>
#include "AppCommandDispatcher.h"

bool AppCommandDispatcher::CanDispatch(
    AppCommand command,
    const AppContext& context
) {
    switch (command) {
        case AppCommand::NewProject:
            return true;

        case AppCommand::OpenProject:
            return true;

        case AppCommand::Save:
            return context.projectManager->HasActiveProject()
                && context.projectManager->IsDirty();

        case AppCommand::SaveAs:
            return context.projectManager->HasActiveProject();

        case AppCommand::CloseProject:
            return context.projectManager->HasActiveProject();

        case AppCommand::Undo:
            //return ctx.projectManager.CanUndo();
            return false;

        case AppCommand::Redo:
            //return ctx.projectManager.CanRedo();
            return false;

        case AppCommand::Cut:
        case AppCommand::Copy:
        case AppCommand::Paste:
        case AppCommand::SelectAll:
            return true;

        case AppCommand::ToggleWindow:
            return true;

        case AppCommand::OpenWindow:
        case AppCommand::CloseWindow:
            return true;

        case AppCommand::NewFile:
        case AppCommand::OpenFile:
            return true;

        case AppCommand::Preferences:
            return true;

        case AppCommand::About:
            return true;

        case AppCommand::Quit:
            return true;
    }

    return false;
}

void AppCommandDispatcher::Dispatch(
    AppCommandRequest command,
    WindowManager& window,
    AppContext& context,
    AppComponentRegistry& components
    //todo ref to app components (open file)
) {
    if (!CanDispatch(command.command, context)) {
        return;
    }

    fmt::println("Dispatching command {:s}", ToString(command.command));

    switch (command.command) {
        case AppCommand::NewProject:
            window.OpenModal(context, "modal.new_project");
            break;

        case AppCommand::OpenProject:
            window.OpenModal(context, "modal.open_project");
            break;

        case AppCommand::Save:
            //ctx.projectManager.SaveCurrentProject(ctx);
            break;

        case AppCommand::SaveAs:
            //ctx.modalManager.OpenSaveProjectAsModal();
            break;

        case AppCommand::SaveAll:
            //ctx.modalManager.OpenSaveProjectAsModal();
            break;

        case AppCommand::CloseProject:
            context.projectManager->CloseProject();
            break;

        case AppCommand::Undo:
            //ctx.projectManager.Undo();
            break;

        case AppCommand::Redo:
            //ctx.projectManager.Redo();
            break;

        case AppCommand::Cut:
            //ctx.windowManager.RequestCut(ctx);
            break;

        case AppCommand::Copy:
            //ctx.windowManager.RequestCopy(ctx);
            break;

        case AppCommand::Paste:
            //ctx.windowManager.RequestPaste(ctx);
            break;

        case AppCommand::SelectAll:
            //ctx.windowManager.RequestSelectAll(ctx);
            break;

        case AppCommand::Preferences:
            //ctx.modalManager.OpenPreferencesModal();
            break;

        case AppCommand::About:
            //ctx.modalManager.OpenAboutModal();
            break;

        case AppCommand::ToggleWindow:
		    fmt::println("Toggle window '{:s}'", command.id);
            window.ToggleWindowVisibility(command.id);
            break;

        case AppCommand::OpenWindow:
            //todo done direct by module
            //add window
            break;

        case AppCommand::CloseWindow:
            fmt::println("Remove window '{:s}'", command.id);
            window.RemoveWindow(command.id);
            break;

        case AppCommand::NewFile:
            fmt::println("New file '{:s}'", command.id);
            //todo create file entry in project
            //todo create data object in module
			//todo open file
            
            components.HandleCommand(
                context, window, command);
			//easy fix: split get component (registry)
            //directly handle command
            break;

            //todo open project file (in editor)
		case AppCommand::OpenFile:
            fmt::println("Open file '{:s}'", command.id);

            //todo get file path
			components.HandleCommand(
                context, window, command);
            
			break;

        case AppCommand::Quit:
            context.requestQuit = true;
            break;
    }
}