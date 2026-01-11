#version 330 core
out vec4 FragColor;

in vec3 ourColor;
in vec3 Normal;

void main(){
    vec3 normColor = (Normal*0.5f)+0.5f;
    FragColor = vec4(normColor,1.0f);
}