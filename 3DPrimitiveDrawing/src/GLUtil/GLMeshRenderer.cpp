#include "GLMeshRenderer.h"
#include "GLMemory.h"
#include "GLMeshBuilder.h"

GLMeshRenderer::GLMeshRenderer(ObjReader* reader)
{
	GLMeshBuilder* meshBuilder = new GLMeshBuilder();
	meshBuilder->SetVertexBuffer(reader->GetVertexBuffer(), reader->GetVertexBufferSize());
	meshBuilder->SetUVBuffer(reader->GetUVBuffer(), reader->GetUVBufferSize());
	meshBuilder->SetNormalBuffer(reader->GetNormalBuffer(), reader->GetNoralBufferSize());
	meshBuilder->SetImageBuffer(reader->GetImageBuffer());
	meshBuilder->build();
}



GLMeshRenderer::~GLMeshRenderer()
{
	
}
