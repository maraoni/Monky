#include "OpenGLFrameBuffer.h"
#include <glad/glad.h>


namespace Gorilla
{
	OpenGLFrameBuffer::OpenGLFrameBuffer(const FrameBufferSpecification& spec)
	{

	}

	OpenGLFrameBuffer::~OpenGLFrameBuffer()
	{
		glDeleteFramebuffers(1, &myRendererID);
	}

	void OpenGLFrameBuffer::Invalidate()
	{
		glCreateFramebuffers(1, &myRendererID);

		glCreateTextures(GL_TEXTURE_2D, 1, &myColorAttatchement);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, mySpec.width, mySpec.height, 0,
			GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, myColorAttatchement, 0);
	}

	const FrameBufferSpecification& Gorilla::OpenGLFrameBuffer::GetSpec()
	{
		return mySpec;
	}
}
