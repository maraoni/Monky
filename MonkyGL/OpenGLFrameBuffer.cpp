#include "OpenGLFrameBuffer.h"
#include <glad/glad.h>


namespace Gorilla
{
	OpenGLFrameBuffer::OpenGLFrameBuffer(const FrameBufferSpecification& spec)
	{
		mySpec = spec;
		Invalidate();
	}

	OpenGLFrameBuffer::~OpenGLFrameBuffer()
	{
		glDeleteFramebuffers(1, &myRendererID);
		glDeleteTextures(1, &myColorAttatchement);
		glDeleteTextures(1, &myDepthAttatchement);
	}

	void OpenGLFrameBuffer::Invalidate()
	{
		if (myRendererID) 
		{
			glDeleteFramebuffers(1, &myRendererID);
			glDeleteTextures(1, &myColorAttatchement);
			glDeleteTextures(1, &myDepthAttatchement);
		}

		glCreateFramebuffers(1, &myRendererID);
		glBindFramebuffer(GL_FRAMEBUFFER, myRendererID);


		glCreateTextures(GL_TEXTURE_2D, 1, &myColorAttatchement);
		glBindTexture(GL_TEXTURE_2D, myColorAttatchement);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, mySpec.width, mySpec.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, myColorAttatchement, 0);

		glCreateTextures(GL_TEXTURE_2D, 1, &myDepthAttatchement);
		glBindTexture(GL_TEXTURE_2D, myDepthAttatchement);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, mySpec.width, mySpec.height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, nullptr);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, myDepthAttatchement, 0);

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE)
		{
			int kek = 0;
			//wow works
		}
		else
		{
			int no;
		}

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	const FrameBufferSpecification& Gorilla::OpenGLFrameBuffer::GetSpec()
	{
		return mySpec;
	}

	unsigned int OpenGLFrameBuffer::GetColorAttatchmentRendererID()
	{
		return myColorAttatchement;
	}

	void OpenGLFrameBuffer::Resize(const unsigned int& aWidth, const unsigned int& aHeight)
	{
		mySpec.width = aWidth;
		mySpec.height = aHeight;
		Invalidate();
	}

	void OpenGLFrameBuffer::Bind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, myRendererID);
		glViewport(0, 0, mySpec.width, mySpec.height);
	}
	void OpenGLFrameBuffer::Unbind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
}
