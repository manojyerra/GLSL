#ifndef Triangle2D_H
#define Triangle2D_H

#include "glm/glm.hpp"

class Triangle2D
{
private:
	static float Dist(glm::vec3 p1, glm::vec3 p2);
	static glm::vec3 IntersectionPoint(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, glm::vec3 p4);

public:
	glm::vec3 P1;
	glm::vec3 P2;
	glm::vec3 P3;

	Triangle2D();
	Triangle2D(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3 );
	Triangle2D( float x1, float y1, float x2, float y2, float x3, float y3 );
	~Triangle2D();

	bool Contains(glm::vec3* p);
	bool Contains( float x, float y );
	static bool Contains(glm::vec3* p1, glm::vec3* p2, glm::vec3* p3, glm::vec3* p );

	bool IsColliding(Triangle2D* triangle);

	//static TwoPos GetP3(CVector3 p1, float distanceFromP1, CVector3 p2, float distanceFromP2);

	static glm::vec3 FindIntersectionByPercentages(glm::vec3 t1, glm::vec3 t2, glm::vec3 t3, float percent13, float percent23);
	static glm::vec3 FindPercentages(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, glm::vec3 p);
};

#endif