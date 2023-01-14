#pragma once
#include <vector>
#include <iostream>
#include <glm/glm.hpp>
using namespace std;

#include "base/vertex.h"
#include "base/bounding_box.h"
#include "base/camera.h"
// 贝塞尔旋转曲面类
class BezierFace : public Object3D
{
	// 顶点数
	int numVertices;
	// 索引数(每个四边形片6个顶点)
	int numIndices;

	float texrange_l, texrange_r;

	// uv 分段数

	// attributes
	vector<Vertex> vertices;
	vector<float> us;
	vector<float> rs;
	vector<float> ys;

	vector<int> indices;
	// 生成
	void generate(int prec);
	// 控制点
	float *controlPoints;
	vector<glm::vec2> controlPointsVector;

	float toRadians(float degrees);
	float Bernstein(float u, int index);

public:
	BezierFace();
	// BezierFace(int i);
	BezierFace(vector<glm::vec2> controlPointsVector, float l = 0, float r = 1);

	float getLength();
	int getNumVertices();
	int getNumIndices();
	vector<Vertex> getVertices();
	vector<int> getIndices();

	bool getRadiance(float pos, float &y, float &r);
};
