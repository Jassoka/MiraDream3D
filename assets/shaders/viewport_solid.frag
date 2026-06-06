#version 330 core

out vec4 fragColor;
in vec3 n, v, l;


void main()
{
    float alpha= 32.0;
    float ki = 0.3;
    float kd = 0.9;
    float ks = 0.3;

    vec3 N = normalize(n);
    vec3 L = normalize(l);
    vec3 V = normalize(v);

    float diffuse = max(dot(N,L),0.0);
    float ambiant = ki;
    vec3 r = 2.0 * dot(N,L)*N-L;
    float specular = pow(max(dot(r,V),0.0),alpha);
    float phong = ambiant + kd * diffuse + ks * specular;
    fragColor = vec4( phong * vec3(0.2,1.0,0.2),1.0);


}