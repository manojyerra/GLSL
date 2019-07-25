#version 450

layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec4 gAlbedo;

in vec3 vertexVary;
in vec3 normalVary;
in vec2 uvVary;

void main()
{    
    // store the fragment position vector in the first gbuffer texture
    gPosition = vertexVary;
    
	// also store the per-fragment normals into the gbuffer
    gNormal = normalVary;
	
    //gAlbedo.rgb = vec3(0.95);
	gAlbedo = vec4(1.0, 0.0, 0.0, 1.0);
}