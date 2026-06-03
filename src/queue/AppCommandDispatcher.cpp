// AppCommandDispatcher.cpp
#include <fmt/core.h>
#include "AppCommandDispatcher.h"
#include "../AppContext.h"
#include "../ux/WindowManager.h"

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
    AppContext& context
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

            //todo open project file (in editor)
		case AppCommand::OpenFile:
            fmt::println("Open file '{:s}'", command.id);
			break;

        case AppCommand::Quit:
            context.requestQuit = true;
            break;
    }
}