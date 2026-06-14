#include "UI.h"

ImGuiWindowFlags DOCKING_SPACE_FLAGS =
    ImGuiWindowFlags_NoDocking |
    ImGuiWindowFlags_NoTitleBar |
    ImGuiWindowFlags_NoCollapse |
    ImGuiWindowFlags_NoResize |
    ImGuiWindowFlags_NoMove |
    ImGuiWindowFlags_NoBringToFrontOnFocus |
    ImGuiWindowFlags_NoNavFocus |
    ImGuiWindowFlags_NoBackground;

ImGuiWindowFlags TOOLBAR_FLAGS =
    ImGuiWindowFlags_NoCollapse;

int UI::Init(AppContext& context)
{
    //init sdl
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        fmt::print("SDL init failed: {}\n", SDL_GetError());
        return -1;
    }

    //init nativefiledialog
	if (NFD_Init() != NFD_OKAY) {
		fmt::print("NativeFileDialog init failed\n");
		return -1;
	}

    // OpenGL version
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,
        SDL_GL_CONTEXT_PROFILE_CORE);

    window = SDL_CreateWindow(
        "CPUForge",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        1280,
        720,
        SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_MAXIMIZED
    );

    if (!window)
    {
        fmt::print("Window creation failed\n");
        return -1;
    }

    glContext = SDL_GL_CreateContext(window);

    if (!glContext)
    {
        fmt::print("OpenGL context failed\n");
        return -1;
    }

    SDL_GL_MakeCurrent(window, glContext);
    SDL_GL_SetSwapInterval(1);

    //
    //TODO macos native integration - menu bar work
#ifdef __APPLE__
    fmt::println("Using MacOS native menu bar");
    //CPUForge_InstallMacMenu();
#else
    fmt::println("Using menu bar rendered in-app");
#endif
    // */

    // ImGui setup
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGuiIO& io = ImGui::GetIO();
    //(void)io;

    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    //io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable; // optional (multi-window OS windows)


    //ImGuiIO& io = ImGui::GetIO();
    //(void)io;
    viewport = ImGui::GetMainViewport();
    
    ImGui::StyleColorsDark();

    ImGuiIO& imio = ImGui::GetIO();
    ImFont* newFont = imio.Fonts->AddFontFromFileTTF("DroidSans.ttf", 16.f); //AddFontDefaultVector();
    //ImFontConfig fontConfig;
    //fontConfig.SizePixels = 14;
    //ImFont* newFont = imio.Fonts->AddFontDefaultVector(&fontConfig);
    imio.FontDefault = newFont;

    ImGui_ImplSDL2_InitForOpenGL(window, glContext);
    ImGui_ImplOpenGL3_Init("#version 330");

	fmt::println("UI initialized");
	return 0;
}

/* Main render function called from main */
bool UI::Render(AppContext& context)
{
	bool running = true;
    static bool firstPass = true;
    SDL_Event event;

    while (SDL_PollEvent(&event))
    {
        ImGui_ImplSDL2_ProcessEvent(&event);

        if (event.type == SDL_QUIT)
            running = false;
    }

    std::string title = "CPUForge";
    if (context.projectManager->HasActiveProject()) {
        title += " - " 
            + context.projectManager->GetProjectName()
            + (context.projectManager->IsDirty() ? "*" : "");
    }
    SDL_SetWindowTitle(window, title.c_str());

    // Start frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame();

    ImGui::NewFrame();
	// add all imgui rendering calls after this line

	//add main menu bar before docking space
    //#ifndef __APPLE__
    //TODO remove commented ifndef once menu bars are both working
    //perhaps include option to choose native or in-app menu bar?
    //running &= windowManager.RenderMainMenuBar(context);

    const MenuBarModel menu = BuildMainMenuModel(windowManager.GetWindows());

    //#endif    

#ifdef __APPLE__
    //fmt::println("Using MacOS native menu bar");
    MacMenu_Update(menu);
#else
    //fmt::print("Using menu bar rendered in-app");
    DrawMainMenuBar(menu, context);
#endif

	//set params for docking space root window
    ImGui::SetNextWindowPos(viewport->WorkPos);
    ImGui::SetNextWindowSize(viewport->WorkSize);
    ImGui::SetNextWindowViewport(viewport->ID);

	//set style vars for docking space
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

    ImGui::Begin("DockSpace Root", nullptr, DOCKING_SPACE_FLAGS);
    ImGuiID dockspace_id = ImGui::GetID("MainDockSpace");
    ImGui::DockSpace(dockspace_id);

    if (firstPass) {
        firstPass = false;
        //set default docking
        auto dock_id_left = ImGui::DockBuilderSplitNode(
            dockspace_id, ImGuiDir_Left, 0.2f, nullptr, &dockspace_id);
        auto dock_id_bottom = ImGui::DockBuilderSplitNode(
            dockspace_id, ImGuiDir_Down, 0.2f, nullptr, &dockspace_id);
        ImGui::DockBuilderDockWindow("Project Tree", dock_id_left);
        ImGui::DockBuilderDockWindow("Console", dock_id_bottom);
        ImGui::DockBuilderFinish(dockspace_id);
    }

    ImGui::End();

    ImGui::PopStyleVar(2);
	// docking area added, add later windows can be docked to this

    //draw ui here
	windowManager.DrawAll(context); //all drawing handled by window manager

	// Render - finish all imgui calls before this line
    ImGui::Render();

    if (ImGui::GetIO().KeyCtrl && ImGui::IsKeyPressed(ImGuiKey_S, false)) {
        //save
        fmt::println("[Keyboard Handler] Save current file");
        context.appCommandQueue->Push(AppCommand::Save);
    }

    //clear screen
    int display_w, display_h;
    //SDL_GetWindowSize(window, &display_w, &display_h);
	SDL_GL_GetDrawableSize(window, &display_w, &display_h); //play nice with high-dpi
    glViewport(0, 0, display_w, display_h);
    glClearColor(0.01f, 0.012f, 0.015f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    //render imgui data
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    SDL_GL_SwapWindow(window);
	return running;
}

int UI::Close()
{
    NFD_Quit();

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();

    ImGui::DestroyContext();

    SDL_GL_DeleteContext(glContext);
    SDL_DestroyWindow(window);

    SDL_Quit();
	return 0;
}

WindowManager& UI::GetWindowManager() {
    return windowManager;
}