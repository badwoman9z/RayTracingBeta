#pragma once
#include<iostream>
#include<vector>
#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <iostream>
#include <numeric>
#include"scene/Shape.h"
#include"render\RenderManager.h"

glm::mat4 getTransformMatrix(glm::vec3 rotateCtrl, glm::vec3 translateCtrl, glm::vec3 scaleCtrl);

// 按照三角形中心排序 -- 比较函数
bool cmpx(const Triangle& t1, const Triangle& t2);
bool cmpy(const Triangle& t1, const Triangle& t2);
bool cmpz(const Triangle& t1, const Triangle& t2);

void   drawQuad();
GLint  createVAO4ScreenQuad();
GLint  createVAO(const GLvoid* vVertices, GLint vVerticesSize, std::initializer_list<GLint> vAttribsLength, const GLint vIndices[] = nullptr, GLint vIndicesSize = 0, int* voVBO = nullptr);
GLint  genBuffer(GLenum vTarget, GLsizeiptr vSize, const GLvoid* vData, GLenum vUsage, GLint vBindingIndex = -1);
float* calculateHdrCache(float* HDR, int width, int height);