#include "ui.h"

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

int UI::init()
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

    gl_context = SDL_GL_CreateContext(window);

    if (!gl_context)
    {
        fmt::print("OpenGL context failed\n");
        return -1;
    }

    SDL_GL_MakeCurrent(window, gl_context);
    SDL_GL_SetSwapInterval(1);

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

    ImGui_ImplSDL2_InitForOpenGL(window, gl_context);
    ImGui_ImplOpenGL3_Init("#version 330");

	fmt::println("UI initialized");
	return 0;
}

/* Main render function called from main */
bool UI::render(AppContext& context)
{
	bool running = true;
    SDL_Event event;

    while (SDL_PollEvent(&event))
    {
        ImGui_ImplSDL2_ProcessEvent(&event);

        if (event.type == SDL_QUIT)
            running = false;
    }

    // Start frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame();

    ImGui::NewFrame();
	// add all imgui rendering calls after this line

	//add main menu bar before docking space
    running &= UI::renderMainMenuBar(context);

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
    ImGui::End();

    ImGui::PopStyleVar(2);
	// docking area added, add later windows can be docked to this

    /* TODO Render proper UI elements and handle actions */

    // UI
    //if (last_selected)
    //{
    //    ImGui::Begin("Hello", &toolbar_test, TOOLBAR_FLAGS);
    //    ImGui::Text("It vworks!");
    //    ImGui::Text("Test line");
    //    ImGui::Text("Text line");
    //    //if (ImGui::IsWindowFocused()) {
	//	//	fmt::println("Menu 1 is focused");
    //    //}
    //    ImGui::End();
    //}
    //
    //
    //ImGui::Begin("Hello2", nullptr, TOOLBAR_FLAGS);
    //ImGui::Text("It vworks22!");
	//if (ImGui::Button("Open Toolbar"))
    //{
    //    toolbar_test = true;
    //}
    //ImGui::End();

    //draw ui here
	windowManager.DrawAll(context);

	/*ImGuiID current_window = ImGui::GetFocusID();
	if (last_selected != current_window) {
		last_selected = current_window;
        fmt::println("Focus: {:d}", current_window);
	}*/

	// Render - finish all imgui calls before this line
    ImGui::Render();

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

int UI::close()
{
    NFD_Quit();

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();

    ImGui::DestroyContext();

    SDL_GL_DeleteContext(gl_context);
    SDL_DestroyWindow(window);

    SDL_Quit();
	return 0;
}

bool UI::renderMainMenuBar(AppContext& context) {
	bool running = true;
    if (!ImGui::BeginMainMenuBar()) { return running; }

    if (ImGui::BeginMenu("File")) {
        if (ImGui::MenuItem("New Project")) {
			//context.projectManager.NewProject("New Project");
            //open modal somehow??
            windowManager.OpenModal("modal.new_project");
        }
        if (ImGui::MenuItem("Open Project", "Ctrl+O")) {
            windowManager.OpenModal("modal.open_project");
        }
        ImGui::Separator();
        //disable save options if no project loaded
        if (!context.projectManager.HasActiveProject()) 
        { ImGui::BeginDisabled(); }

        if (ImGui::MenuItem("Save Project")) {
        }
        ImGui::Separator();
        if (ImGui::MenuItem("Save", "Ctrl+S")) {
        }
        if (ImGui::MenuItem("Save as..")) {
        }
        if (ImGui::MenuItem("Save All", "Ctrl+Shift+S")) {
        }
        if (ImGui::MenuItem("Close Project")) {
        }

        if (!context.projectManager.HasActiveProject()) 
        { ImGui::EndDisabled(); }

        ImGui::Separator();
        if (ImGui::MenuItem("Quit", "Alt+F4")) {
            running = false;
        }
        ImGui::EndMenu();
    }

    if (ImGui::BeginMenu("Edit")) {
        //todo check workspace manager for active cuttable/copyable item
        if (!context.projectManager.HasActiveProject()) { ImGui::BeginDisabled(); }
        if (ImGui::MenuItem("Cut", "Ctrl+X")) {
        }
        if (ImGui::MenuItem("Copy", "Ctrl+C")) {
        }
        if (ImGui::MenuItem("Paste", "Ctrl+V")) {
        }
        if (!context.projectManager.HasActiveProject()) { ImGui::EndDisabled(); }
        ImGui::EndMenu();
    }

    if (ImGui::BeginMenu("Window")) {
        //todo add items from windowmanager
        //if (ImGui::MenuItem("ISA Design")) {
        //}
		windowManager.DrawWindowMenuItems();
        ImGui::EndMenu();
    }
    //if (ImGui::Button("Test Button")) {}
    ImGui::EndMainMenuBar();
    
    return running;
}