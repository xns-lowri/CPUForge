#pragma once
#include "MenuData.h"
#include "../_UxComponent.h"

static const std::string ShortcutToImguiText(const std::optional<MenuShortcut>& shortcut);
static void DrawMenuItemTree(const AppContext& context, const MenuItem& item);
void DrawMainMenuBar(MenuBarModel menu, const AppContext& context);