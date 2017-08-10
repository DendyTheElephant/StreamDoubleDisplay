/* Standard library includes */
/***/

/* External dependencies */
#include <SDL2/SDL.h>

/* Internal headers includes */
#include "UErrorHandler.h"
#include "CImageStreamDoubleDisplay.h"
#include "DBasicTypes.h"
#include "CInputHandler.h"
#include "CTimer.h"

/* Entry point */
int main(int argc, char *argv[]) {

// If in debug mode, print cerr in console, else in log.txt
#ifndef DEBUG //  ======================================================================== [ not DEBUG ] =======================================  //
	std::ofstream logStream("PixPhetamine2D.log");
	std::streambuf* originErrorStream = std::cerr.rdbuf();
	std::cerr.rdbuf(logStream.rdbuf());
#endif //  =========================================================================== [ END not DEBUG ] =======================================  //

	Utility::UErrorHandler* ErrorLog = &Utility::UErrorHandler::getInstance();
	ErrorLog->setOutputFile("error.txt");



	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	////  ================================================================================================================================  ////
	////    ---- Image Stream Double Display creation ----		                                                                            ////
	////																																	////
	PixPhetamine2D::CImageStreamDoubleDisplay* displayer;

	pxString windowTitle1("Phantom pain remedy (MD)");
	pxString windowTitle2("Phantom pain remedy (Patent)");

	displayer = new PixPhetamine2D::CImageStreamDoubleDisplay(windowTitle1, 1280, 720, windowTitle2, 1600, 900);
	////																																	////
	////    ---- Image Stream Double Display creation ----		                                                                            ////
	////  ================================================================================================================================  ////
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	PadPhetamine::CInputHandler* inputHandler = new PadPhetamine::CInputHandler(displayer->getWindow1());

	// -- Load a texture in RAM
	SDL_Surface *image = IMG_Load("../Dendy2.png");
	
	if (image == nullptr) {
		ERROR("Can't load the image ../Dendy2.png!");
	}
		
	
	int x = 0, y = 0;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	////  ================================================================================================================================  ////
	////    ---- MAIN LOOP ----		                                                                                                        ////
	////																																	////
	do {

		inputHandler->update();

		// *((unsigned int *)image->pixels + 640 * x++ + y++) = 0;

		displayer->displayImage(image->pixels);

	} while (inputHandler->isQuit() == false);
	////																																	////
	////    ---- MAIN LOOP ----		                                                                                                        ////
	////  ================================================================================================================================  ////
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	SDL_FreeSurface(image);

	return EXIT_SUCCESS;
}
