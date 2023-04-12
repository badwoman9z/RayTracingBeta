#pragma once
#include <GL/glew.h>
class Buffer
{
public:
	Buffer(GLenum target,GLsizeiptr size,const void * data,GLenum useage,int base = -1) {
		glGenBuffers(1, &bufferID);
		glBindBuffer(target, bufferID);
		glBufferData(target, size, data, useage);
		if (base != -1) {
			glBindBufferBase(target, base, bufferID);
		}
		glBindBuffer(target, 0);
	}

	unsigned int getBufferID() {
		return this->bufferID;
	}
private:
	unsigned int bufferID;
};


