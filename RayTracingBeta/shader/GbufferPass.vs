#version 430 core
layout (location = 0) in vec4 aPos;
layout (location = 1) in vec4 aNormal;

uniform mat4 view;
uniform mat4 projection;


out vec3 v_worldPos;
out vec3 v_normal;
out float v_materialID;
out float v_triangleID;


void main()
{   

    v_worldPos = aPos.xyz;
    v_triangleID = aPos.a;
    v_normal = aNormal.xyz;
    //v_normal.y*=-1.0;
    v_materialID = aNormal.a;
    
     gl_Position = projection*view*vec4( aPos.xyz, 1.0);
    
    

}