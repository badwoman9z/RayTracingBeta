#pragma once
#include <glm/glm.hpp>
struct  Material
{
	glm::vec3 emessiveColor;
	float roughness;
	glm::vec3 baseColor;
	float metallic;
};
struct Triangle
{
	glm::vec3 p1;
	glm::vec3 p2;
	glm::vec3 p3;
	glm::vec3 n1;
	glm::vec3 n2;
	glm::vec3 n3;
	float materialID;
	float triangleID;

};


struct BVHNode
{
	int left, right;
	int n, index;
	glm::vec3 AA, BB;
};

struct SSBO_Triangle
{
	glm::vec4 p1withTriangleID;
	glm::vec4 n1withMaterialID;
	glm::vec4 p2withTriangleID;
	glm::vec4 n2withMaterialID;
	glm::vec4 p3withTriangleID;
	glm::vec4 n3withMaterialID;

};


struct SSBO_BVHNode {
	int left, right, n, index;
	glm::vec4 AA, BB;
};