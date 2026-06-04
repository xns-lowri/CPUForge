// CPUForge.h : Include file for standard system include files,
// or project specific include files.

#ifndef CPUFORGE_
#define CPUFORGE_


#include <string>
#include <optional>
#include <vector>
#include <cstdint>
#include <unordered_map>

#include "_types.h"

#include <imgui.h>
#include <imgui_internal.h>
#include <imgui_stdlib.h>

#include "AppContext.h"

/* Load modules */
#include "queue/AppCommandQueue.h"
#include "queue/AppCommandDispatcher.h"
//include project manager first
//#include "project/Project.h" //includes architecture, instruction
#include "project/ProjectManager.h"
#include "project/WorkspaceManager.h"

//include system logic modules
#include "ux/_UxComponent.h"

#include "ux/modules/ViewProjectTree.h"

//app components
#include "core/AppComponentRegistry.h"

//load ui last
#include "ux/UI.h"


// TODO: Reference additional headers your program requires here.

#endif // !CPUFORGE_