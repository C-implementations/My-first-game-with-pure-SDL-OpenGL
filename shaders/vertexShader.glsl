#version 410 core

layout(location=0) in vec3 position;
layout(location=1) in vec3 vertexColors;

uniform mat4 u_ModelMatrix; // uniform variable
uniform mat4 u_Projection; // uniform variable
uniform mat4 u_ViewMatrix; // uniform variable

out vec3 v_vertexColors;

void main()
{
   v_vertexColors = vertexColors;
   vec4 newPosition = u_Projection * u_ViewMatrix * u_ModelMatrix * vec4(position, 1.0f);
                                                               // do not forget 'w'
   gl_Position = vec4(newPosition.x, newPosition.y, newPosition.z, newPosition.w);
}