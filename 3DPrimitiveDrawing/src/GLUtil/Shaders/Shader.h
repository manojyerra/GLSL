#ifndef Shader_H
#define Shader_H
class Shader {

public:
	virtual void SetAlpha(float alpha) = 0;
	virtual void SetModelMatrix(float* mat) = 0;
	virtual void Begin() = 0;
	virtual void SetUniformsAndAttributes() = 0;
	virtual void End() = 0;
};

#endif