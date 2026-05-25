// CPUForge.h : Include file for standard system include files,
// or project specific include files.

#ifndef CPUFORGE_
#define CPUFORGE_

using UUID = uint64_t;

#include <string>
#include <optional>
#include <vector>
#include <cstdint>
#include <unordered_map>

#include <imgui.h>
#include <imgui_internal.h>
#include <imgui_stdlib.h>

#include "AppContext.h"

/* Load modules */
//include project manager first
//#include "project/Project.h" //includes architecture, instruction
#include "project/ProjectManager.h"
#include "project/WorkspaceManager.h"

//include system logic modules
#include "ux/_UxInterface.h"

#include "ux/modules/ViewProjectTree.h"

//load ui last
#include "ux/UI.h"


// TODO: Reference additional headers your program requires here.

#endif // !CPUFORGE_