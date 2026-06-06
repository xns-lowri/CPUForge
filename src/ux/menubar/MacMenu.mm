#import <Cocoa/Cocoa.h>

#include <unordered_map>

#include "MacOsNative.h"

static int gNextMenuTag = 1;
static std::unordered_map<int, AppCommandRequest> gCommandRequests;

@interface CPUForgeMenuTarget : NSObject
- (void)menuAction:(id)sender;
@end

@implementation CPUForgeMenuTarget

- (void)menuAction:(id)sender {
    NSMenuItem* item = (NSMenuItem*)sender;
    int tag = static_cast<int>(item.tag);

    auto it = gCommandRequests.find(tag);
    if (it == gCommandRequests.end()) {
        return;
    }

    NativeCommandBridge::PushCommand(it->second);

}

@end

static CPUForgeMenuTarget* gMenuTarget = nil;


//helper
static NSString* ToNSString(const std::string& value) {
    return [NSString stringWithUTF8String:value.c_str()];
}

//build items
static NSMenuItem* BuildNativeMenuItem(const MenuItem& item);

static NSMenu* BuildNativeSubmenu(
    const std::string& title,
    const std::vector<MenuItem>& children
) {
    NSMenu* menu = [[NSMenu alloc] initWithTitle:ToNSString(title)];

    for (const MenuItem& child : children) {
        [menu addItem:BuildNativeMenuItem(child)];
    }

    return menu;
}

static NSMenuItem* BuildNativeMenuItem(const MenuItem& item) {
    switch (item.kind) {
        case MenuItemKind::Separator:
            return [NSMenuItem separatorItem];

        case MenuItemKind::Command: {
            int tag = gNextMenuTag++;
            gCommandRequests[tag] = { item.command.value(), item.id };

            NSMenuItem* menuItem =
                [[NSMenuItem alloc]
                    initWithTitle:ToNSString(item.label)
                    action:@selector(menuAction:)
                    keyEquivalent:@""];

            menuItem.target = gMenuTarget;
            menuItem.tag = tag;
            menuItem.enabled = item.enabled;
            menuItem.state = item.checked
                ? NSControlStateValueOn
                : NSControlStateValueOff;

            return menuItem;
        }

        case MenuItemKind::Window: {
            int tag = gNextMenuTag++;
            gCommandRequests[tag] = { item.command.value(), item.id };

            NSMenuItem* menuItem =
                [[NSMenuItem alloc]
                    initWithTitle:ToNSString(item.label)
                    action:@selector(menuAction:)
                    keyEquivalent:@""];

            menuItem.target = gMenuTarget;
            menuItem.tag = tag;
            menuItem.enabled = item.enabled;
            menuItem.state = item.checked
                ? NSControlStateValueOn
                : NSControlStateValueOff;
                
            return menuItem;
        }

        case MenuItemKind::Submenu: {
            NSMenuItem* menuItem =
                [[NSMenuItem alloc]
                    initWithTitle:ToNSString(item.label)
                    action:nil
                    keyEquivalent:@""];

            NSMenu* submenu = BuildNativeSubmenu(item.label, item.children);
            [menuItem setSubmenu:submenu];

            return menuItem;
        }
    }

    return [NSMenuItem separatorItem];
}

static void AddAppMenu(NSMenu* mainMenu) {
    NSString* appName = [[NSProcessInfo processInfo] processName];

    NSMenuItem* appMenuItem =
        [[NSMenuItem alloc] initWithTitle:@""
                                   action:nil
                            keyEquivalent:@""];

    NSMenu* appMenu = [[NSMenu alloc] initWithTitle:appName];

    // About via command dispatcher
    {
        int tag = gNextMenuTag++;
        gCommandRequests[tag] = { AppCommand::About, "" };

        NSMenuItem* aboutItem =
            [[NSMenuItem alloc]
                initWithTitle:[NSString stringWithFormat:@"About %@", appName]
                action:@selector(menuAction:)
                keyEquivalent:@""];

        aboutItem.target = gMenuTarget;
        aboutItem.tag = tag;
        [appMenu addItem:aboutItem];
    }

    [appMenu addItem:[NSMenuItem separatorItem]];

    // Native hide/show
    [appMenu addItemWithTitle:[NSString stringWithFormat:@"Hide %@", appName]
                        action:@selector(hide:)
                 keyEquivalent:@"h"];

    NSMenuItem* hideOthers =
        [appMenu addItemWithTitle:@"Hide Others"
                            action:@selector(hideOtherApplications:)
                     keyEquivalent:@"h"];

    hideOthers.keyEquivalentModifierMask =
        NSEventModifierFlagCommand | NSEventModifierFlagOption;

    [appMenu addItemWithTitle:@"Show All"
                        action:@selector(unhideAllApplications:)
                 keyEquivalent:@""];

    [appMenu addItem:[NSMenuItem separatorItem]];

    // Quit via command dispatcher
    {
        int tag = gNextMenuTag++;
        gCommandRequests[tag] = { AppCommand::Quit, "" };

        NSMenuItem* quitItem =
            [[NSMenuItem alloc]
                initWithTitle:[NSString stringWithFormat:@"Quit %@", appName]
                action:@selector(menuAction:)
                keyEquivalent:@"q"];

        quitItem.target = gMenuTarget;
        quitItem.tag = tag;
        [appMenu addItem:quitItem];
    }

    [mainMenu addItem:appMenuItem];
    [mainMenu setSubmenu:appMenu forItem:appMenuItem];
}


extern "C" void MacMenu_Update(const MenuBarModel& menu) {
    @autoreleasepool {
        NSApplication* app = [NSApplication sharedApplication];

        if (!gMenuTarget) {
            gMenuTarget = [[CPUForgeMenuTarget alloc] init];
        }

        gNextMenuTag = 1;
        gCommandRequests.clear();

        NSMenu* mainMenu = [[NSMenu alloc] initWithTitle:@"MainMenu"];

        //add app menu
        AddAppMenu(mainMenu);

        for (const MenuItem& topLevel : menu) {
            if (topLevel.kind != MenuItemKind::Submenu) {
                continue;
            }

            NSMenuItem* topLevelItem =
                [[NSMenuItem alloc]
                    initWithTitle:ToNSString(topLevel.label)
                    action:nil
                    keyEquivalent:@""];

            NSMenu* submenu =
                BuildNativeSubmenu(topLevel.label, topLevel.children);

            [mainMenu addItem:topLevelItem];
            [mainMenu setSubmenu:submenu forItem:topLevelItem];
        }

        app.mainMenu = mainMenu;
    }
}
