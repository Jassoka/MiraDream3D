//
// Created by jassoka on 7/26/26.
//
#include "controller/EditorController.h"

#include "controller/RenderController.h"

void EditorController::setTool(EditorTool tool)
{
    mRenderController->toolChanged(tool);
}
