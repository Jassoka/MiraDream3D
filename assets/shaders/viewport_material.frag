#version 330 core

out vec4 fragColor;
in vec3 n, v, l;

in vec2 uvTexCoords;


uniform vec3 Ks,Ka,Kd;
uniform float alpha;
uniform float Ns;
uniform sampler2D colorTexture;

void main()
{
    vec3 N = normalize(n);
    vec3 L = normalize(l);
    vec3 V = normalize(v);

    vec4 textureColor=texture(colorTexture,uvTexCoords);

    float diffuse = max(dot(N,L),0.0);
    vec3 ambiant = Ka;
    vec3 r = 2.0 * dot(N,L)*N-L;
    float specular = pow(max(dot(r,V),0.0),Ns);
    vec3 phong = ambiant + Kd * diffuse*textureColor.rgb + Ks * specular;
    fragColor = vec4( phong , alpha*textureColor.a );
}