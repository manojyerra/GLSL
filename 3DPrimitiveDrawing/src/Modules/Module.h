#ifndef Module_H
#define Module_H

class Module
{
private:

public:
	virtual void SetScreenSize(unsigned int sw, unsigned int sh) = 0;
	virtual void OnModuleChange(unsigned int sw, unsigned int sh) = 0;
	virtual void Update(float deltaTime) = 0;
	virtual void Draw() = 0;
};

#endif
