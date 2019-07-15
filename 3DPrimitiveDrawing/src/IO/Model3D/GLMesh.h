//#ifndef GLMesh_H
//#define GLMesh_H
//
//#include "ShaderProgram.h"
//#include "GLMat.h"
//#include "ImageBuffer.h"
//
//class GLMesh
//{
//private:
//	unsigned int _vertexBufferID;
//	unsigned int _normalBufferID;
//	unsigned int _uvBufferID;
//	unsigned int _baseTexID;
//	unsigned int _vertexCount;
//
//	ShaderProgram* _shaderProgram;
//
//	GLMat _modelMat;
//
//	GLMesh* SetVertexArray(const float* arr);
//	GLMesh* SetNormalArray(const float* arr);
//	GLMesh* SetUVArray(const float* arr);
//	GLMesh* SetImageBuffer(ImageBuffer* imgBuf);
//	bool build();
//
//public:
//	static const int OBJ = 1;
//	static const int BINARY = 2;
//
//	GLMesh(string folderPath, int type);
//
//
//    void Draw();
//
//	~GLMesh();
//};
//
//#endif