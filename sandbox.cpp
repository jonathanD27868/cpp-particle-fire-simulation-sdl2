#include <iostream>
#include <stdio.h>
#include <SDL2/SDL.h>
#include <math.h>
#include "Screen.h"
#include <stdlib.h>
#include <time.h>
#include "Swarm.h"
using namespace std;
using namespace caveofprogramming;

int main() {

    srand(time(NULL));

    Screen screen;
    if(screen.init() == false) {
        cout << "Error initializing SDL." << endl;
    }

    Swarm swarm;

    
    // game loop
    while(true) {
        // update and draw particles

        // return the number of ms since the program started
        int elapsed = SDL_GetTicks();

        swarm.update(elapsed);

        unsigned char red = (1 + sin(elapsed * 0.0002)) * 128;
        unsigned char green = (1 + sin(elapsed * 0.0001)) * 128;
        unsigned char blue = (1 + sin(elapsed * 0.0003)) * 128;

        const Particle * const particles = swarm.getParticles();
        for(int i = 0; i < Swarm::NPARTICLES; i++) {
            Particle particle = particles[i];

            // particle.m_x + 1 => de 0 à 2
            int x = (particle.m_x + 1) * Screen::SCREEN_WIDTH / 2;
            int y = particle.m_y * Screen::SCREEN_WIDTH / 2 + Screen::SCREEN_HEIGHT / 2;

            screen.setPixel(x, y, red, green, blue);
        }

        // screen.setPixel(400, 300, 255, 255, 255);

        // blur effect
        screen.boxBlur();

        // Draw the screen
        screen.update();
        // check for messages/events
        if(screen.processEvents() == false){
            break;
        }
    }

    screen.close();

    return 0;
}