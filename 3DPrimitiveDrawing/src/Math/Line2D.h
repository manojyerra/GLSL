#ifndef Line2D_H
#define Line2D_H

#include "glm/glm.hpp"

class Line2D
{
private:

public:
	glm::vec3 P1;
	glm::vec3 P2;

	Line2D();
	Line2D(glm::vec3 p1, glm::vec3 p2 );
	Line2D(glm::vec3* p1, glm::vec3* p2 );
	Line2D( float x1, float y1, float x2, float y2 );
	~Line2D();

	bool AtSameSide(glm::vec3* p1, glm::vec3* p2);
	glm::vec3 IntersectionPoint(Line2D* line, bool* intersectionPointExist);
	float PerpendicularDist(glm::vec3* p);
	static float PerpendicularDist(glm::vec3* p1, glm::vec3* p2, glm::vec3* p );
	static glm::vec3 FootOfPerpendicular(glm::vec3* p1, glm::vec3* p2, glm::vec3* p );
};

#endif