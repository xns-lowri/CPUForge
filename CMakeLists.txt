# CMakeList.txt : CMake project for CPUForge, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.10)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("CPUForge")

# find packages to include
find_package(fmt CONFIG REQUIRED)
find_package(OpenGL REQUIRED)
find_package(sdl2 CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)
# find_package(nativefiledialog-extended REQUIRED)

add_subdirectory("external/nativefiledialog-extended")

# Add source to this project's executable.
add_executable (CPUForge)

target_sources(CPUForge PRIVATE 
    "src/CPUForge.cpp" 
    "src/CPUForge.h"
    "external/imgui_impl_sdl2.cpp" 
    "external/imgui_impl_sdl2.h" 
    "external/imgui_impl_opengl3.cpp" 
    "external/imgui_impl_opengl3.h" 
    "external/imgui_impl_opengl3_loader.h"

    "src/AppContext.h" 
    "src/_types.h"
    "src/enumAppCommands.h"

    "src/core/Instruction.h" 
    "src/core/Architecture.h" 

    "src/project/Project.h" 
    "src/project/ProjectManager.h" 
    "src/project/ProjectManager.cpp"    
    "src/project/WorkspaceManager.h"  
    "src/project/WorkspaceManager.cpp"  
    "src/project/FileHandler.h" 
    
    "src/queue/AppCommandQueue.h"
    "src/queue/AppCommandQueue.cpp"
    "src/queue/AppCommandDispatcher.h"
    "src/queue/AppCommandDispatcher.cpp"

    "src/ux/ui.cpp" 
    "src/ux/UI.h" 
    "src/ux/modules/ViewProjectTree.h" 
    "src/ux/_UxInterface.h" 
    "src/ux/WindowManager.h" 
    "src/ux/WindowManager.cpp" 

    "src/ux/core/ModalNewFolder.h" 
    "src/ux/core/ModalNewProject.h" 
    "src/ux/core/ModalOpenProject.h" 

    "src/ux/menubar/MenuData.h"
    "src/ux/menubar/ImGuiMenuHandler.h"
    "src/ux/menubar/ImGuiMenuHandler.cpp"
    "src/ux/menubar/WindowMenuData.h"
      "src/core/Register.h")

if(APPLE)
    target_sources(CPUForge PRIVATE
        "src/ux/menubar/MacOsNative.h"
        "src/ux/menubar/MacOsNative.cpp"
        "src/ux/menubar/MacMenu.mm"
    )

    target_link_libraries(CPUForge PRIVATE
        "-framework Cocoa"
    )
endif()

#add_executable (CPUForge "CPUForge.cpp" "CPUForge.h")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET CPUForge PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
    #target_link_libraries(CPUForge PRIVATE fmt::fmt)
    #target_link_libraries(CPUForge PRIVATE OpenGL::GL)
    #target_link_libraries(CPUForge PRIVATE
    #    $<TARGET_NAME_IF_EXISTS:SDL2::SDL2main>
    #    $<IF:$<TARGET_EXISTS:SDL2::SDL2>,SDL2::SDL2,SDL2::SDL2-static>
    #)
target_link_libraries(CPUForge PRIVATE
    fmt::fmt
    SDL2::SDL2main
    SDL2::SDL2
    OpenGL::GL
    imgui::imgui
    nfd::nfd
    nlohmann_json::nlohmann_json
)