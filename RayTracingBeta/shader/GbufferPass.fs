#version 430 core
layout (location = 0) out vec4 PositionWithDepth;
layout (location = 1) out vec4 NormalWithMaterial;


in vec3 v_worldPos;
in vec3 v_normal;
in float v_materialID;
in float v_triangleID;
//out vec4 color;


void main()
{   
    PositionWithDepth = vec4(v_worldPos,gl_FragCoord.z);
    //PositionWithDepth = vec4(1.0);
     NormalWithMaterial =vec4(normalize(v_normal),v_materialID);
        
   // color = vec4(NormalWithMaterial.xyz,1.0);
}