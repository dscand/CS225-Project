#ifndef BUTTON_H
#define BUTTON_H

#include "SDL.h"
#include <string>
#include <vector>
#include "Input.h"
#include "GameObject.h"
#include "TectureMhr.h"

using TextureIdList = std::vector<std::string>;

class Button : GameObject {
  
  public:
  
    button(int x, int y, void (*callback)(), TextureIdList textrIDs) : GameObject(new Transform()){
      m_Tf-> X = x;
      m_Tf-> Y = y;
      m_Callback = callback;
      m_TextrIDs = textrIDs;
    }
    virtual ~Button();
  
    void update(Mouse& mouse);
    void draw();
};

#endif // BUTTON_H

