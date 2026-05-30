#pragma once
#include "MenuData.h"
#include "../_UxInterface.h"

static const char* ShortcutToImguiText(const std::optional<MenuShortcut>& shortcut);
static void DrawMenuItemTree(const AppContext& context, const MenuItem& item);
void DrawMainMenuBar(MenuBarModel menu, const AppContext& context);