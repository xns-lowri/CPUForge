#include "ImGuiMenuHandler.h"

static const char* ShortcutToImguiText(const std::optional<MenuShortcut>& shortcut) {
    if (!shortcut.has_value()) {
        return nullptr;
    }

    // Simple version. For production, use a small string builder.
#ifdef __APPLE__
    if (shortcut->commandOrCtrl && shortcut->shift && shortcut->key == 's') return "Cmd+Shift+S";
    if (shortcut->commandOrCtrl && shortcut->key == 's') return "Cmd+S";
    if (shortcut->commandOrCtrl && shortcut->key == 'o') return "Cmd+O";
    if (shortcut->commandOrCtrl && shortcut->key == 'n') return "Cmd+N";
    if (shortcut->commandOrCtrl && shortcut->key == 'z') return "Cmd+Z";
    if (shortcut->commandOrCtrl && shortcut->key == 'x') return "Cmd+X";
    if (shortcut->commandOrCtrl && shortcut->key == 'c') return "Cmd+C";
    if (shortcut->commandOrCtrl && shortcut->key == 'v') return "Cmd+V";
    if (shortcut->commandOrCtrl && shortcut->key == 'a') return "Cmd+A";
#else
    if (shortcut->commandOrCtrl && shortcut->shift && shortcut->key == 's') return "Ctrl+Shift+S";
    if (shortcut->commandOrCtrl && shortcut->key == 's') return "Ctrl+S";
    if (shortcut->commandOrCtrl && shortcut->key == 'o') return "Ctrl+O";
    if (shortcut->commandOrCtrl && shortcut->key == 'n') return "Ctrl+N";
    if (shortcut->commandOrCtrl && shortcut->key == 'z') return "Ctrl+Z";
    if (shortcut->commandOrCtrl && shortcut->key == 'x') return "Ctrl+X";
    if (shortcut->commandOrCtrl && shortcut->key == 'c') return "Ctrl+C";
    if (shortcut->commandOrCtrl && shortcut->key == 'v') return "Ctrl+V";
    if (shortcut->commandOrCtrl && shortcut->key == 'a') return "Ctrl+A";
#endif

    return nullptr;
}

static void DrawMenuItemTree(const AppContext& context, const MenuItem& item) {
    switch (item.kind) {
        case MenuItemKind::Separator:
            ImGui::Separator();
            break;

        case MenuItemKind::Command: {
            const char* shortcutText = ShortcutToImguiText(item.shortcut);

            if (ImGui::MenuItem(
                    item.label.c_str(),
                    shortcutText,
                    item.checked,
                    item.enabled
                )) {
                if (item.command.has_value()) {
                    //DispatchCommand(*item.command);
                    //todo queue command
                    context.commandQueue.Push( { item.command.value() } );
                }
            }

            break;
        }

        case MenuItemKind::Window: {
            if (ImGui::MenuItem(
                    item.label.c_str(),
                    NULL,
                    item.checked,
                    item.enabled
                )) {
                if (item.command.has_value()) {
                    //DispatchCommand(*item.command);
                    //todo queue command

		            fmt::println("Push event '{:s}'", item.id);
                    context.commandQueue.Push( {item.command.value(), item.id} );
                }
            }

            break;
        }

        case MenuItemKind::Submenu:
            if (ImGui::BeginMenu(item.label.c_str(), item.enabled)) {
                for (const MenuItem& child : item.children) {
                    DrawMenuItemTree(context, child);
                }

                ImGui::EndMenu();
            }
            break;
    }
}

void DrawMainMenuBar(MenuBarModel menu, const AppContext& context) {
    //todo list of windows
    //const MenuBarModel menu = BuildMainMenuModel(windows);

    if (ImGui::BeginMainMenuBar()) {
        for (const MenuItem& menuItem : menu) {
            DrawMenuItemTree(context, menuItem);
        }

        ImGui::EndMainMenuBar();
    }
}