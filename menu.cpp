namespace MouseFunctions {
	bool mouseOver(int x, int y, int width, int height) {
		int mouseX, mouseY;
		SDL_GetMouseState( &mouseX, &mouseY );

		// sets bounds for the texture
		int lowLimitX = x - (width/2);
		int upLimitX = x + (width/2);
		int lowLimitY = y - (height/4.3);
		int upLimitY = y + (height/4.3);

		// will tell you if the mouse position is within the bounds of the texture
		if (mouseX >= lowLimitX && mouseX <= upLimitX && mouseY >= lowLimitY && mouseY <= upLimitY){
			return true;
		}
		else{
			return false;
		}

	}
}