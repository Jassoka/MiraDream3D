#version 330 core

out vec4 fragColor;

#if RENDER_MODE == 0
in vec3 color;
#endif

#if RENDER_MODE == 2 || RENDER_MODE == 3
in vec3 n, v, l;

vec4 computeLight(vec3 Ka,
                  vec3 Kd,
                  vec3 Ks,
                  float alpha,
                  float Ns,
                  vec4 textureColor) {
    vec3 N = normalize(n);
    vec3 L = normalize(l);
    vec3 V = normalize(v);
    float diffuse = max(dot(N,L),0.0);
    float specular = 0.0;
    float ambiant = 0.15;
    if (diffuse > 0.0) {
        vec3 r = 2.0 * dot(N, L) * N - L;
        specular = pow(max(dot(r, V), 0.0), Ns);
    }
    vec3 phong = (ambiant*Ka + Kd * diffuse + Ks * specular)*textureColor.rgb ;
    return vec4( phong , alpha*textureColor.a );
}
#if RENDER_MODE == 3
in vec2 uvTexCoords;
uniform vec3 Ka,Kd,Ks;
uniform float alpha;
uniform float Ns;
uniform sampler2D colorTexture;
#endif
#endif

/*
#if RENDER_MODE == 1
in vec3 color;
#endif

vec4 getHalfEdgeColor(vec3 color) {

    if (color.x + color.z < 1.0)
        return vec4(0.8, 0.8, 0.8 ,1.0);
    return vec4(vec3(1.0,0.35,0.0) ,1.0);
}
*/

void main() {
    #if RENDER_MODE == 0
    if (color.x < 1.0) {
        fragColor = vec4(vec3(1.0,1.0,1.0),1.0);
    }
    else {
        fragColor = vec4(vec3(0.9,0.5,0.2),1.0);
    }
    #elif RENDER_MODE == 1
    fragColor = vec4(vec3(0.9,0.5,0.2),1.0);
    #elif RENDER_MODE == 2
    fragColor = computeLight(vec3(1.0), vec3(0.70), vec3(0.15), 1.0, 32.0, vec4(vec3(0.2,1.0,0.2), 1.0));
    #elif RENDER_MODE == 3
    fragColor = computeLight(Ka, Kd, Ks, alpha, Ns, texture(colorTexture,uvTexCoords));
    /*
    #elif RENDER_MODE == 3
    fragColor = getHalfEdgeColor(color);*/
    #endif
}