#version 330


in vec3 nearUnProj;
in vec3 farUnProj;
in vec3 lookAtPoint;
out vec4 fragColor;

uniform mat4 viewMatrix;
uniform mat4 projMatrix;


//On se base sur la fonction affine R(t)=n + t(f-n) t dans R. On regarde l'intersection à y=0
//t>0 indique que l'on regarde a travers la surface y=0

void main() {
    float denom = farUnProj.y - nearUnProj.y ;

    if (abs(denom)<1e-6){
        fragColor=vec4(0.0);
    }
    else {
        float t = -nearUnProj.y/denom;
        vec3 planePos=nearUnProj + t * (farUnProj - nearUnProj);

        vec4 clip_space_pos = projMatrix * viewMatrix * vec4(planePos, 1.0);
        float clip_space_depth = clip_space_pos.z / clip_space_pos.w;
        gl_FragDepth = ((gl_DepthRange.diff * clip_space_depth) + gl_DepthRange.near + gl_DepthRange.far) / 2.0;

        float tileSize=1.0;


        float wireSize=1e-2;


        vec2 coord = planePos.xz / tileSize;
        vec2 derivative = fwidth(coord);
        vec2 grid = abs(fract(coord - 0.5) - 0.5) / derivative;
        float line = min(grid.x, grid.y);
        float alpha = 1.0 - min(line, 1.0);
        if (alpha < 0.1) {
            discard;
        }
        fragColor = vec4(vec3(0.5), alpha) * float(t > 0);
        //fragColor=vec4(vec3( xFrac < wireSize || yFrac < wireSize ),1.0) * float(t>0);
    }


}