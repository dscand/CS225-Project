namespace MouseFunctions {
	bool mouseOver(int x, int y, int width, int height) {
		int mouseX, mouseY;
		SDL_GetMouseState( &mouseX, &mouseY );
		lowLimitX = x - (width/2);
		upLimitX = x + (width/2);
		lowLimitY = y - (height/2);
		upLimitY = y + (height/2);
		
		if (mouseX >= lowLimitX & mouseX <= upLimitX & mouseY >= lowLimitY & mouseY <= upLimitY){
			return true;
		}
		else{
			return false;
		}

	}
}
