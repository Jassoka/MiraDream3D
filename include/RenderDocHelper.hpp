//
// Created by jassoka on 6/5/26.
//

#ifndef MIRADREAM3D_RENDERDOCHELPER_HPP
#define MIRADREAM3D_RENDERDOCHELPER_HPP

#pragma once
#include "renderdoc_app.h"

extern RENDERDOC_API_1_1_2* rdoc_api;

void initRenderDoc();

#define RDOC_TRIGGER_CAPTURE() \
if (rdoc_api) { rdoc_api->TriggerCapture(); }

#endif //MIRADREAM3D_RENDERDOCHELPER_HPP
