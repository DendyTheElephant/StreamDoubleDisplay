////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////====================================================================================================================================////
/// | \file		CImageStreamDoubleDisplay
///	| \author	Daniel Huc
/// | \date		August 2017
/// |----------------------------------------------------------------------------------------------------------------------------------
/// | \brief	Blabla
/// | \details	Blabla
////====================================================================================================================================////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once
/* Standard library includes */
#include <string>

/* External dependencies */
#include <SDL2/SDL.h>

/* Modules include */
#include "DBasicTypes.h"
#include "CTimer.h"

/* Internal headers includes */
#include "CFrameBuffer.h"
#include "CPostProcessPass.h"
#include "CTexture.h"
#include "UErrorHandler.h"

/* Specific defines */

//// ---- Namespaces ---- ////
namespace PixPhetamine2D {
	
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	////====================================================================================================================================////
	//// CImageStreamDoubleDisplay
	/// \brief		Blabla
	/// \details	Blabla
	////====================================================================================================================================////
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	class CImageStreamDoubleDisplay {
		
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//// ---- Enumerations -----                                                                                                            ////
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	public:

	
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//// ---- Members -----																													//// 
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	private:
		// -- Context relative
		SDL_Window* 				m_upSDLWindow1{ nullptr };		///< SDL's Window 1 handle
		SDL_Window*					m_upSDLWindow2{ nullptr };		///< SDL's Window 2 handle
		SDL_GLContext				m_GLContext{ nullptr };			///< OpenGL context handle, shared between two windows!
		pxString					m_Windows1Caption{ "" };
		pxInt						m_Windows1Width{ 0 };
		pxInt						m_Windows1Height{ 0 };
		pxString					m_Windows2Caption{ "" };
		pxInt						m_Windows2Width{ 0 };
		pxInt						m_Windows2Height{ 0 };
		
		pxInt						m_StreamWidth{ 1920 };
		pxInt						m_StreamHeight{ 1080 };


		// -- Rendering stuff from PixPhetamine
		pxUniquePtr< LowLevelWrapper::CShader >			m_upShader{ nullptr };			///< GPU Program
		pxUniquePtr< LowLevelWrapper::CTexture >		m_upTexture{ nullptr };			///< Texture that handles our stream
		//pxUniquePtr< SceneRendering::CRenderPass >	m_upMainRender{nullptr};	///< Rendering Drawcall function 
		pxUniquePtr< PostProcess::CPostProcessPass >	m_upMainRender{ nullptr };	///< Rendering Drawcall function 

		// -- Utility
		CommonPhetamine::CTimer m_RenderProcessTimer;		///< Used to benchmark the rendering process
		CommonPhetamine::CTimer m_RefreshFPSTimer;			///< Used to timelapse the update of the benchmarking values
		pxUInt					m_RefreshFPSPeriod{ 40 };	///< Update time in ms
		
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//// ---- Methods -----																													////
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	private:
	//// ---- Internal methods ---- ////
		void _configureContext();
		void _checkSDLError();

	public:
	//// ---- Constructor/Destructor ---- ////
		CImageStreamDoubleDisplay(pxString a_Windows1Caption, pxUInt a_Windows1Width, pxUInt a_Windows1Height, 
				pxString a_Windows2Caption, pxUInt a_Windows2Width, pxUInt a_Windows2Height);
		
	//// ---- Operator ---- ////
		
	
	//// ---- Getter ---- ////
		SDL_Window* getWindow1() const { return m_upSDLWindow1; }
		SDL_Window* getWindow2() const { return m_upSDLWindow2; }

	
	//// ---- Setter ---- ////

	
	//// ---- Core ---- ////
		void displayImage(void* a_RawImageData);

	};
}
