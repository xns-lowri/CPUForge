#include "ImGuiMenuHandler.h"

static const std::string ShortcutToImguiText(const std::optional<MenuShortcut>& shortcut) {
    if (!shortcut.has_value()) {
        return "";
    }

    std::string shortcutString = "";
    if (shortcut->ctrl) {
#ifdef __APPLE__
        shortcutString += "Cmd+";
#else
        shortcutString += "Ctrl+";
#endif
    }

    if (shortcut->alt) {
#ifdef __APPLE__
        shortcutString += "Opt+";
#else
        shortcutString += "Alt+";
#endif
    }

    if (shortcut->shift) {
        shortcutString += "Shift+";
    }
    
    shortcutString += std::toupper(shortcut->key);

    return shortcutString;
}

static void DrawMenuItemTree(const AppContext& context, const MenuItem& item) {
    switch (item.kind) {
        case MenuItemKind::Separator:
            ImGui::Separator();
            break;

        case MenuItemKind::Command: {
            const std::string shortcutText = ShortcutToImguiText(item.shortcut);

            if (ImGui::MenuItem(
                    item.label.c_str(),
                    shortcutText.c_str(),
                    item.checked,
                    item.enabled
                )) {
                if (item.command.has_value()) {
                    //DispatchCommand(*item.command);
                    //todo queue command
                    context.appCommandQueue->Push(
                        item.command.value() 
                    );
                }
            }

            break;
        }

        case MenuItemKind::Window: {
            if (ImGui::MenuItem(
                    item.label.c_str(),
                    "",
                    item.checked,
                    item.enabled
                )) {
                if (item.command.has_value()) {
                    //DispatchCommand(*item.command);
                    //todo queue command

		            fmt::println("Push event '{:s}'", item.id);
                    context.appCommandQueue->Push(AppCommandRequest{
                        item.command.value(), 
                        item.id
                    });
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