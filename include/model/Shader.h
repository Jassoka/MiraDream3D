//
// Created by jassoka on 6/1/26.
//

#ifndef MIRADREAM3D_SHADER_H
#define MIRADREAM3D_SHADER_H
#include <cstdint>

class Shader
{
private:
    uint32_t mID;
public:
    explicit Shader(uint32_t id);
    void execShader();
};


#endif //MIRADREAM3D_SHADER_H
