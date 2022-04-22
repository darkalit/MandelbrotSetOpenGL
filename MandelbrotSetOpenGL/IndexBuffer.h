	#pragma once

#include "headers.h"

class IndexBuffer
{
private:
	unsigned int m_renderer_ID;
	unsigned int m_count;

public:
	IndexBuffer(const void* data, unsigned int count);
	~IndexBuffer();

	void bind() const;
	void unbind() const;

	inline unsigned int get_count() const { return m_count; }
};

IndexBuffer::IndexBuffer(const void* data, unsigned int count)
	: m_count(count)
{
	glGenBuffers(1, &m_renderer_ID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_renderer_ID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW);
}

IndexBuffer::~IndexBuffer()
{
	glDeleteBuffers(1, &m_renderer_ID);
}

void IndexBuffer::bind() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_renderer_ID);
}

void IndexBuffer::unbind() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
