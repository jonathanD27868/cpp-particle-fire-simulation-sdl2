#include "Screen.h"


namespace caveofprogramming {

  Screen::Screen() :
          m_window(NULL), m_renderer(NULL), m_texture(NULL), m_buffer1(NULL), m_buffer2(NULL) {

  }

  bool Screen::init(){
    if(SDL_Init(SDL_INIT_VIDEO) < 0) {
        return false;
    }

    // create window
    m_window = SDL_CreateWindow("Particle fire explosion", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                                SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

    // check if window creation failed
    if(m_window == NULL) {
        SDL_Quit();
        return false;
    }

    // SDL_RENDERER_PRESENTVSYNC => s'assure que le rendering est synchronisé avec le vertical 
    // refresh de l'écran
    m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_PRESENTVSYNC);
    m_texture = SDL_CreateTexture(m_renderer, SDL_PIXELFORMAT_RGBA8888, 
                                SDL_TEXTUREACCESS_STATIC, SCREEN_WIDTH, SCREEN_HEIGHT);

    if(m_renderer == NULL) {
        SDL_DestroyWindow(m_window);
        SDL_Quit();
        return false;
    }

    if(m_texture == NULL) {
        SDL_DestroyRenderer(m_renderer);
        SDL_DestroyWindow(m_window);
        SDL_Quit();
        return false;
    }

    // normallement on devrait checker si buffer à réussi l'allocation de mémoire
    // Uint32 => fourni par sdl s'assur que le type ext unsigned integer de 32 bits sur tous les pc
    m_buffer1 = new Uint32[SCREEN_WIDTH * SCREEN_HEIGHT];
    m_buffer2 = new Uint32[SCREEN_WIDTH * SCREEN_HEIGHT];

    // place 0 en décimal ou hexa dans tout le tableau buffer pour l'avoir noir
    // SCREEN_WIDTH * sizeof(Uint32) => permet d'avoir la largeur complète de la fenêtre
    // (SCREEN_WIDTH * sizeof(Uint32)) * SCREEN_HEIGHT => Number of bytes to be set to the value (m_buffer1)
    // selon la doc de memeset
    memset(m_buffer1, 0, (SCREEN_WIDTH * sizeof(Uint32)) * SCREEN_HEIGHT);
    memset(m_buffer2, 0, (SCREEN_WIDTH * sizeof(Uint32)) * SCREEN_HEIGHT);

    return true;
  }

  void Screen::boxBlur() {
    // Swap the buffers, so pixel is in m_buffer2 and we are drawing to m_buffer1
    Uint32 *temp = m_buffer1;
    m_buffer1 = m_buffer2;
    m_buffer2 = temp;

    for(int y = 0; y < SCREEN_HEIGHT; y++) {
      for(int x = 0; x < SCREEN_WIDTH; x++) {
        int redTotal = 0;
        int greenTotal = 0;
        int blueTotal = 0;

        // blur box principe:
        //    0 0 0
        //    0 1 0
        //    0 0 0

        // On considère un block de 9 pixels dans lequel le pixel concerné, lorsque l'on boucle dans le buffer
        // de pixels, est au centre. L'algo consiste à calculer la moyenne des valeurs des couleurs autour de
        // ce pixel (en divisant par 9) et de l'affecter à ce pixel central. Pour boucler sur tous les pixels autour:
        for(int row = -1; row <= 1; row++) {
          for(int col = -1; col <= 1; col++) {
            int currentX = x + col;
            int currentY = y + row;

            if(currentX >= 0 && currentX < SCREEN_WIDTH && currentY >= 0 && currentY < SCREEN_HEIGHT) {
              Uint32 color = m_buffer2[currentY * SCREEN_WIDTH + currentX];

              Uint8 red = color >> 24;
              Uint8 green = color >> 16;
              Uint8 blue = color >> 8;

              redTotal += red;
              greenTotal += green;
              blueTotal += blue; 
            }
          }
        }

        // get the average of each color
        Uint8 red = redTotal / 9;
        Uint8 green = greenTotal / 9;
        Uint8 blue = blueTotal / 9;

        setPixel(x, y, red, green, blue);
      }
    }
  }

  void Screen::setPixel(int x, int y, Uint8 red, Uint8 green, Uint8 blue) {
    if(x < 0 || x >= SCREEN_WIDTH || y < 0 || y >= SCREEN_HEIGHT) {
      return;
    }
    // cf. le dossier 66-bit-shifting-and-colors pour les explications
    Uint32 color = 0;
    color += red;
    color <<= 8;
    color += green;
    color <<= 8;
    color += blue;
    color <<= 8;
    color += 0xFF;

    m_buffer1[(y * SCREEN_WIDTH) + x] = color;
  }

  void Screen::update() {
    // update the given texture with new pixel data
    // last arg => pitch -> the number of bytes in a row of pixel data, including padding between lines
    SDL_UpdateTexture(m_texture, NULL, m_buffer1, SCREEN_WIDTH * sizeof(Uint32));

    //clear the current rendering target with the drawing color.
    SDL_RenderClear(m_renderer);
    
    // copy a portion of the texture to the current rendering target.
    // 3rd arg => source => NULL for the entire structure
    // 4th arg => destination => NULL for the entire rendering target (the texture will be
    // stretched to fill the given rectangle)
    SDL_RenderCopy(m_renderer, m_texture, NULL, NULL);
    
    // update the screen with any rendering performed since the previous call.
    SDL_RenderPresent(m_renderer);
  }

  bool Screen::processEvents(){
    // the SDL_Event structure to be filled with the next event from the queue, or NULL
    SDL_Event event;

    // check events in the queue
    while(SDL_PollEvent(&event)) {
        // SDL_QUIT is an event => https://wiki.libsdl.org/SDL_Event (click to quit program)
        if(event.type == SDL_QUIT) {
          return false;
        }
    }
    return true;
  }

  void Screen::close(){
    delete []m_buffer1;
    delete []m_buffer2;
    SDL_DestroyRenderer(m_renderer);
    SDL_DestroyTexture(m_texture);
    SDL_DestroyWindow(m_window);
    SDL_Quit();
  }


} /* namespace caveofprogramming */
