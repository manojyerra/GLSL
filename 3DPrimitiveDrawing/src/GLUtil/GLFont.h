#ifndef GLFont_H
#define GLFont_H

#include <vector>
#include <string>
#include "GLBatch.h"
#include "ShaderProgram.h"
#include "GLMat.h"
#include "Cam2D.h"

class GLStringInfo
{
public:
	std::string text;
	float xPos;
	float yPos;
	float zPos;
	float width;
	float height;
	float horGap;
};


class GLLetterInfo
{
public:
	float x,y,w,h,u,v,cw,ch;

	GLLetterInfo()
	{
		x = y = w = h = u = v = cw = ch = 0.0f;
	}

	~GLLetterInfo()
	{
	}

	void SetBounds(float xx, float yy, float ww, float hh)
	{
		x = xx;
		y = yy;
		w = ww;
		h = hh;
	}

	void SetUVBounds(float U, float V, float CW, float CH)
	{
		u = U;
		v = V;
		cw = CW;
		ch = CH;
	}
};

class GLFont
{
private:
	static GLFont* _ref;
	GLFont();
	~GLFont();

	std::vector<GLLetterInfo*> _normalFontVec;

	float _unitWidth;
	int _r,_g,_b,_a;

	int u[127];
	int v[127];
	int cw[127];
	int ch[127];

	float _borderX1;
	float _borderX2;

	float _horGap;
	
	int _imgW;
	int _imgH;
	unsigned int _textureID;

	GLBatch* _data;
	ShaderProgram* _shaderProgram;
	GLMat _modelMat;
	unsigned int _verBufID;
	unsigned int _uvBufID;

	void FillUVInfo_Verdana_plain_12();
	void FillUVInfoFromTexture();

public:
	static GLFont* GetInstance();
	static void DeleteInstance();

	void SetColor(int r, int g, int b, int a);
	void SetColor(unsigned int color);

	float GetFontSize();
	float GetLength(std::string& text, float fontSize);

	void HorBorder(float x1, float x2);

	void Begin();
	void Draw(std::string& text,	float xPos, float yPos, float fontSize);
	void DrawFromLeft(std::string& text, float xPos, float yPos, float fontSize);
	void DrawFromRight(std::string& text, float xPos, float yPos, float fontSize);
	void DrawFromCenter(std::string& text, float xPos, float yPos, float fontSize);
	void End();
};

#endif
