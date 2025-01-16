#pragma once
namespace Gorilla
{
	struct FrameBufferSpecification
	{
		unsigned int width, height;
		unsigned int samples = 1;
		bool SwapChainTarget = false;


	};

	class OpenGLFrameBuffer
	{

	public:
		OpenGLFrameBuffer(const FrameBufferSpecification& spec);
		~OpenGLFrameBuffer();
		void Invalidate();
		const FrameBufferSpecification& GetSpec();
	private:

		FrameBufferSpecification mySpec;

		unsigned int samples = 1;
		unsigned int myRendererID;
		unsigned int myColorAttatchement;
	};
}

