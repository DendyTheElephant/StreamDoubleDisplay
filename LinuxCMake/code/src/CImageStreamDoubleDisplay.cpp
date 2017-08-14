#include "CImageStreamDoubleDisplay.h"

namespace PixPhetamine2D {

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	////  ================================================================================================================================  ////
	////    ---- Internal methods -----                                                                                                     ////
	////  ================================================================================================================================  ////
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	//----------------------------------------------------------------------------------------------------------------------------------------//
	///
	//----------------------------------------------------------------------------------------------------------------------------------------//
	void CImageStreamDoubleDisplay::_configureContext() {
		STACK_TRACE;
		/* Initialize SDL's Video subsystem */
		if (SDL_Init(SDL_INIT_VIDEO) < 0) {
			STACK_TRACE;
			ERROR("Unable to initialize SDL!");
		}

		/* Request opengl 3.2 context.
		* SDL doesn't have the ability to choose which profile at this time of writing,
		* but it should default to the core profile */
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);

		/* Enable double buffering (one frame delay but complete rendering before update) */
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
		//SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

		/* SDL's built-in MSAA is disabled */
		glDisable(GL_MULTISAMPLE);
		/* No Z axis = no Z Buffer, no Culling test etc etc */
		glDisable(GL_DEPTH_TEST);

		/* Make shure that the powerful GPU will execute this context
		* (Need when two GPU with Optimus for example) */
		SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);

		/* Create our windows centered */
		m_upSDLWindow1 = SDL_CreateWindow(m_Windows1Caption.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, m_Windows1Width, m_Windows1Height, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
		if (m_upSDLWindow1 == nullptr) {
			STACK_TRACE;
			ERROR("Unable to create the window 1 ! Titled: "+m_Windows1Caption);
		}
		m_upSDLWindow2 = SDL_CreateWindow(m_Windows2Caption.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, m_Windows2Width, m_Windows2Height, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
		if (m_upSDLWindow2 == nullptr) {
			STACK_TRACE;
			ERROR("Unable to create the window 2 ! Titled: " + m_Windows2Caption);
		}

		/* Create our opengl context and attach it to our window */
		m_GLContext = SDL_GL_CreateContext(m_upSDLWindow2);

		/* Enable vertical synchronization */
		//SDL_GL_SetSwapInterval(1);

		STACK_MESSAGE("Checking for OpenGL errors");
		Utility::UErrorHandler::checkOpenGLErrors();

		/* Sometimes needed to run in "Experimental" to have access to VBO and stuff */
		glewExperimental = GL_TRUE;
		GLenum status = glewInit();
		if (status != GLEW_OK) {
			STACK_TRACE;
			ERROR("GLEW Error: ");
		}

		// Catch the GL_INVALID_ENUM error caused by glewInit()! It seems to be non harmfull...
		glGetError();

		STACK_MESSAGE("Checking for SDL errors");
		_checkSDLError();

		// Retrieve the GPU - OpenGL Current specs for the platform --> Log file
		std::cerr << "=============[ PixPhetamine2D log-file ]=========================" << std::endl;
		std::cerr << ">Graphics Successfully Initialized !" << std::endl;
		std::cerr << "OpenGL Info" << std::endl;
		std::cerr << "    Version: " << glGetString(GL_VERSION) << std::endl;
		std::cerr << "     Vendor: " << glGetString(GL_VENDOR) << std::endl;
		std::cerr << "   Renderer: " << glGetString(GL_RENDERER) << std::endl;
		std::cerr << "    Shading: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
		std::cerr << "----------------------------------------------------------------" << std::endl;
		std::cerr << ">GPU Specifications for modern GLSL:" << std::endl;
		pxInt32 uboBindings, uboSize, uboVertex, uboFragment, uboGeometry;
		glGetIntegerv(GL_MAX_UNIFORM_BUFFER_BINDINGS, &uboBindings);
		glGetIntegerv(GL_MAX_UNIFORM_BLOCK_SIZE, &uboSize);
		glGetIntegerv(GL_MAX_VERTEX_UNIFORM_BLOCKS, &uboVertex);
		glGetIntegerv(GL_MAX_FRAGMENT_UNIFORM_BLOCKS, &uboFragment);
		glGetIntegerv(GL_MAX_GEOMETRY_UNIFORM_BLOCKS, &uboGeometry);
		std::cerr << "  Max uniform block bindings: " << uboBindings << std::endl;
		std::cerr << "      Max uniform block size: " << uboSize << std::endl;
		std::cerr << "    Max uniform block vertex: " << uboVertex << std::endl;
		std::cerr << "  Max uniform block fragment: " << uboFragment << std::endl;
		std::cerr << "  Max uniform block geometry: " << uboGeometry << std::endl;
		std::cerr << "----------------------------------------------------------------" << std::endl;
		std::cerr << ">Texture specifications:" << std::endl;
		pxInt32 texUnits;
		glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &texUnits);
		std::cerr << "     Max texture image units: " << texUnits << std::endl;
		std::cerr << "----------------------------------------------------------------" << std::endl;
		UNSTACK_TRACE;
	}

	//----------------------------------------------------------------------------------------------------------------------------------------//
	///
	//----------------------------------------------------------------------------------------------------------------------------------------//
	void CImageStreamDoubleDisplay::_checkSDLError() {
		const char *error = SDL_GetError();
		if (*error != '\0') {
			ERROR_CONTINUE("SDL Error: " + pxString(error));
			SDL_ClearError();
		}
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	////  ================================================================================================================================  ////
	////    ---- Constructor/Destructor -----                                                                                               ////
	////  ================================================================================================================================  ////
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	//----------------------------------------------------------------------------------------------------------------------------------------//
	///
	//----------------------------------------------------------------------------------------------------------------------------------------//
	CImageStreamDoubleDisplay::CImageStreamDoubleDisplay(
			pxString a_Windows1Caption, pxUInt a_Windows1Width,	pxUInt a_Windows1Height,
			pxString a_Windows2Caption, pxUInt a_Windows2Width, pxUInt a_Windows2Height
	) :
		m_Windows1Caption(a_Windows1Caption),
		m_Windows1Width(a_Windows1Width),
		m_Windows1Height(a_Windows1Height),
		m_Windows2Caption(a_Windows2Caption),
		m_Windows2Width(a_Windows2Width),
		m_Windows2Height(a_Windows2Height)
	{
		_configureContext();

		pxString vertexShaderCode =
			"#version 330\n\n"
			"layout(location = 0) in vec3 in_vertex_position;\n"
			"void main(void) {\n"
			"    gl_Position = vec4(in_vertex_position, 1.0);\n"
			"}\n";

		pxString fragmentShaderCode =
			"#version 330\n\n"
			"uniform sampler2D  streammed_image;\n"
			"uniform int  screen_width;\n"
			"uniform int  screen_height;\n"
			"layout(location = 0) out vec4 outColor;\n"
			"void main() {\n"
			"    ivec2 windowSize = ivec2(screen_width, screen_height);\n"
			"    vec2 texCoord = gl_FragCoord.xy / windowSize;\n"
			"    //texCoord.y = -texCoord.y;\n"
			"    outColor = texture(streammed_image, texCoord);\n"
			"}\n";

		m_upShader = createUniquePtr<LowLevelWrapper::CShader>("VertexShader", vertexShaderCode, "FragmentShader", fragmentShaderCode);
		m_upMainRender = createUniquePtr<PostProcess::CPostProcessPass>(m_upShader.get());
		m_upTexture = createUniquePtr<LowLevelWrapper::CTexture>(m_StreamWidth, m_StreamHeight, LowLevelWrapper::CTexture::ETextureType::NORMAL, false);

		m_upMainRender->bindTexture(m_upTexture.get(), "streammed_image", 0);
		m_upMainRender->bindVariableName("screen_width");
		m_upMainRender->bindVariableName("screen_height");
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	////  ================================================================================================================================  ////
	////    ---- Core -----                                                                                                                 ////
	////  ================================================================================================================================  ////
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	//----------------------------------------------------------------------------------------------------------------------------------------//
	///
	//----------------------------------------------------------------------------------------------------------------------------------------//
	void CImageStreamDoubleDisplay::displayImage(void* a_RawImageData) {

		// ---- Common ----
		m_RenderProcessTimer.reset();					// Timer reset >> 0
		m_upTexture.get()->setData(a_RawImageData);		// Send image data from RAM to VRAM !! (CPU -> GPU) Same for the 2 displays

		// ---- First display -----
		SDL_GL_MakeCurrent(m_upSDLWindow1, m_GLContext);
		m_upMainRender.get()->sendVariable("screen_width", m_Windows1Width);
		m_upMainRender.get()->sendVariable("screen_height", m_Windows1Height);
		m_upMainRender.get()->process();
		SDL_GL_SwapWindow(m_upSDLWindow1);

		// ---- Second display -----
		SDL_GL_MakeCurrent(m_upSDLWindow2, m_GLContext);
		m_upMainRender.get()->sendVariable("screen_width", m_Windows2Width);
		m_upMainRender.get()->sendVariable("screen_height", m_Windows2Height);
		m_upMainRender.get()->process();
		SDL_GL_SwapWindow(m_upSDLWindow2);


		// ---- FPS update -----
#ifdef DEBUG //  ============================================================================= [ DEBUG ] =======================================  //
		if (m_RefreshFPSTimer.getElapsedMiliseconds() > m_RefreshFPSPeriod) {
			pxFloat elapsedMillisecondsPerFrame = (pxFloat)m_RenderProcessTimer.getElapsedMiliseconds();
			pxString fpsCount = pxString("    FPS: ") + std::to_string((pxUInt)(1000.0 / elapsedMillisecondsPerFrame)) +
					pxString("    so you got ") + std::to_string((pxUInt)(40 - elapsedMillisecondsPerFrame)) + pxString(" ms");
			pxString windowCaption1 = m_Windows1Caption + fpsCount;
			pxString windowCaption2 = m_Windows2Caption + fpsCount;
			SDL_SetWindowTitle(m_upSDLWindow1, windowCaption1.c_str());
			SDL_SetWindowTitle(m_upSDLWindow2, windowCaption2.c_str());
			m_RefreshFPSTimer.reset();
		}
#endif //  =============================================================================== [ END DEBUG ] =======================================  //
	}
}
