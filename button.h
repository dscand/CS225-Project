#ifndef BUTTON_H
#define BUTTON_H

class Button{
public:
  SDL_Texture * tex;
  SDL_Rect srect, drect;
  bool isPressed = false;
  
  button();
  virtual ~Button();
  
  void update(Mouse& mouse);
  void draw();
};

#endif // BUTTON_H

