#ifndef __SCRENN_H__
#define __SCRENN_H__

#include <SDL2/SDL.h>
#include <stdio.h>

using namespace std;

namespace caveofprogramming{

class Screen {
public:
  const static int SCREEN_WIDTH = 800;
  const static int SCREEN_HEIGHT = 600;

private:
  SDL_Window *m_window;
  SDL_Renderer *m_renderer;
  SDL_Texture *m_texture;
  Uint32 *m_buffer1;
  Uint32 *m_buffer2;

public:
  Screen();
  bool init();
  void update();
  void setPixel(int x, int y, Uint8 red, Uint8 green, Uint8 blue);
  bool processEvents();
  void close();
  void boxBlur();
};

} /* namespace caveofprogramming */

#endif
