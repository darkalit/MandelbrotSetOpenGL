#pragma once

#include "headers.h"

// Buffer for vertices | input: vertices array, size of array
class VertexBuffer
{
private:
	unsigned int renderer_ID;

public:
	VertexBuffer(const void* data, unsigned int size);
	~VertexBuffer();

	void bind() const;
	void unbind() const;
};

VertexBuffer::VertexBuffer(const void* data, unsigned int size)
{
	glGenBuffers(1, &renderer_ID);
	glBindBuffer(GL_ARRAY_BUFFER, renderer_ID);
	glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}

VertexBuffer::~VertexBuffer()
{
	glDeleteBuffers(1, &renderer_ID);
}

void VertexBuffer::bind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, renderer_ID);
}

void VertexBuffer::unbind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}