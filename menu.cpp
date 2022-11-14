#include "menu.hpp"

bool MouseFunctions::mouseOver(int x, int y, int width, int height) {
	int mouseX, mouseY;
	SDL_GetMouseState( &mouseX, &mouseY );

	// will tell you if the mouse position is within the bounds of the texture
	if (mouseX >= x - (width/2) && mouseX <= x + (width/2) && mouseY >= y - (height/4) && mouseY <= y + (height/4)) {
		Mix_PlayChannel( -1, gSound[1] , 0 );
		return true;
	}
	else return false;
}
