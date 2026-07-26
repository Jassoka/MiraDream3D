//
// Created by jassoka on 6/10/26.
//

#ifndef MIRADREAM3D_TYPES_H
#define MIRADREAM3D_TYPES_H

enum class ViewportMode
{
    SOLID,
    WIREFRAME,
    MATERIAL
};

struct DefineField
{
    std::string field;
    std::string value = "";
};

enum class EditorTool
{
    NAVIGATE,
    SELECTION,
};

enum class SelectionMode
{
    NONE = 0, VERTEX, EDGE, FACE
};

#endif //MIRADREAM3D_TYPES_H
