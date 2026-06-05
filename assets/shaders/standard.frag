#version 330 core
//uniform float couleurBleu;

//uniform vect3 lightPosition;

out vec4 fragColor;


in vec4 viewPos;
in vec3 fragNormal;
uniform vec3 lightPos;
uniform vec3 cameraPos;
//uniform vec4 material;//alpha,ki,kd,ks

void main()
{
    float alpha= 32.0;
    float ki = 0.2;
    float kd = 0.9;
    float ks = 0.3;


    vec3 n = normalize(fragNormal);
    vec3 v = normalize(-viewPos.xyz);
    vec3 l = -normalize(lightPos-viewPos.xyz);


    float diffuse = max(dot(n,l),0);
    float ambiant = ki;
    vec3 r = 2.0 * dot(n,l)*n-l;
    float specular = pow(max(dot(r,v),0),alpha);
    float phong = ambiant + kd * diffuse + ks* specular;
    fragColor = vec4( phong * vec3(0.5,1.0,0.5),1.0);
}