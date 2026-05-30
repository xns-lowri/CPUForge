#import <Cocoa/Cocoa.h>

#include <unordered_map>

#include "MacOsNative.h"

static int gNextMenuTag = 1;
static std::unordered_map<int, AppCommandRequest> gCommandRequests;
/*
typedef NS_ENUM(NSInteger, CPUForgeMenuCommand) {
    CPUForgeMenuCommandNewProject = 1001,
    CPUForgeMenuCommandOpenProject,
    CPUForgeMenuCommandSaveProject,
    CPUForgeMenuCommandSaveProjectAs,
    CPUForgeMenuCommandPreferences,
};*/

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
/*
    switch ((CPUForgeMenuCommand)item.tag) {
        case CPUForgeMenuCommandNewProject:
            // TODO: call into your C++ command queue
            NSLog(@"New Project");
            break;

        case CPUForgeMenuCommandOpenProject:
            NSLog(@"Open Project");
            break;

        case CPUForgeMenuCommandSaveProject:
            NSLog(@"Save Project");
            break;

        case CPUForgeMenuCommandSaveProjectAs:
            NSLog(@"Save Project As");
            break;

        case CPUForgeMenuCommandPreferences:
            NSLog(@"Preferences");
            break;
    }
*/
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

/*
static NSMenuItem* AddItem(
    NSMenu* menu,
    NSString* title,
    SEL action,
    NSString* key,
    NSInteger tag
) {
    NSMenuItem* item = [[NSMenuItem alloc]
        initWithTitle:title
        action:action
        keyEquivalent:key
    ];

    item.target = gMenuTarget;
    item.tag = tag;

    [menu addItem:item];
    return item;
}*/

/*
extern "C" void CPUForge_InstallMacMenu() {
    @autoreleasepool {
        NSApplication* app = [NSApplication sharedApplication];

        gMenuTarget = [[CPUForgeMenuTarget alloc] init];

        NSString* appName =
            [[NSProcessInfo processInfo] processName];

        NSMenu* mainMenu = [[NSMenu alloc] initWithTitle:@"MainMenu"];

        // App menu: appears under app name next to Apple logo
        NSMenuItem* appMenuItem =
            [[NSMenuItem alloc] initWithTitle:@""
                                       action:nil
                                keyEquivalent:@""];

        NSMenu* appMenu =
            [[NSMenu alloc] initWithTitle:appName];

        [appMenu addItemWithTitle:[NSString stringWithFormat:@"About %@", appName]
                            action:@selector(orderFrontStandardAboutPanel:)
                     keyEquivalent:@""];

        [appMenu addItem:[NSMenuItem separatorItem]];

        AddItem(appMenu,
                @"Preferences…",
                @selector(menuAction:),
                @",",
                CPUForgeMenuCommandPreferences);

        [appMenu addItem:[NSMenuItem separatorItem]];

        [appMenu addItemWithTitle:[NSString stringWithFormat:@"Hide %@", appName]
                            action:@selector(hide:)
                     keyEquivalent:@"h"];

        [appMenu addItemWithTitle:@"Hide Others"
                            action:@selector(hideOtherApplications:)
                     keyEquivalent:@"h"]
            .keyEquivalentModifierMask =
                NSEventModifierFlagCommand | NSEventModifierFlagOption;

        [appMenu addItemWithTitle:@"Show All"
                            action:@selector(unhideAllApplications:)
                     keyEquivalent:@""];

        [appMenu addItem:[NSMenuItem separatorItem]];

        [appMenu addItemWithTitle:[NSString stringWithFormat:@"Quit %@", appName]
                            action:@selector(terminate:)
                     keyEquivalent:@"q"];

        [mainMenu addItem:appMenuItem];
        [mainMenu setSubmenu:appMenu forItem:appMenuItem];

        // File menu
        NSMenuItem* fileMenuItem =
            [[NSMenuItem alloc] initWithTitle:@"File"
                                       action:nil
                                keyEquivalent:@""];

        NSMenu* fileMenu = [[NSMenu alloc] initWithTitle:@"File"];

        AddItem(fileMenu, @"New Project", @selector(menuAction:), @"n",
                CPUForgeMenuCommandNewProject);

        AddItem(fileMenu, @"Open Project…", @selector(menuAction:), @"o",
                CPUForgeMenuCommandOpenProject);

        [fileMenu addItem:[NSMenuItem separatorItem]];

        AddItem(fileMenu, @"Save", @selector(menuAction:), @"s",
                CPUForgeMenuCommandSaveProject);

        NSMenuItem* saveAs =
            AddItem(fileMenu, @"Save As…", @selector(menuAction:), @"S",
                    CPUForgeMenuCommandSaveProjectAs);

        saveAs.keyEquivalentModifierMask =
            NSEventModifierFlagCommand | NSEventModifierFlagShift;

        [mainMenu addItem:fileMenuItem];
        [mainMenu setSubmenu:fileMenu forItem:fileMenuItem];

        // Edit menu, with native selectors so Cmd+C/V/etc work
        NSMenuItem* editMenuItem =
            [[NSMenuItem alloc] initWithTitle:@"Edit"
                                       action:nil
                                keyEquivalent:@""];

        NSMenu* editMenu = [[NSMenu alloc] initWithTitle:@"Edit"];

        [editMenu addItemWithTitle:@"Undo"
                            action:@selector(undo:)
                     keyEquivalent:@"z"];

        [editMenu addItemWithTitle:@"Redo"
                            action:@selector(redo:)
                     keyEquivalent:@"Z"];

        [editMenu addItem:[NSMenuItem separatorItem]];

        [editMenu addItemWithTitle:@"Cut"
                            action:@selector(cut:)
                     keyEquivalent:@"x"];

        [editMenu addItemWithTitle:@"Copy"
                            action:@selector(copy:)
                     keyEquivalent:@"c"];

        [editMenu addItemWithTitle:@"Paste"
                            action:@selector(paste:)
                     keyEquivalent:@"v"];

        [editMenu addItemWithTitle:@"Select All"
                            action:@selector(selectAll:)
                     keyEquivalent:@"a"];

        [mainMenu addItem:editMenuItem];
        [mainMenu setSubmenu:editMenu forItem:editMenuItem];

        app.mainMenu = mainMenu;
    }
}*/

static void AddAppMenu(NSMenu* mainMenu) {
    NSString* appName = [[NSProcessInfo processInfo] processName];

    NSMenuItem* appMenuItem =
        [[NSMenuItem alloc] initWithTitle:@""
                                   action:nil
                            keyEquivalent:@""];

    NSMenu* appMenu = [[NSMenu alloc] initWithTitle:appName];

    // About via your command dispatcher
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

    // Quit via your command dispatcher
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
