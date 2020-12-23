#include "Particle.h"
#include <math.h>
#include <stdlib.h>

namespace caveofprogramming {

  Particle::Particle(): m_x(0), m_y(0) {
    init();
  }

  Particle::~Particle() {}

  void Particle::init() {
    m_x = 0;
    m_y = 0;
    m_direction = (2 * M_PI * rand()) / RAND_MAX;
    m_speed = (0.04 * rand()) / RAND_MAX;
    // m_speed squared:
    m_speed *= m_speed;
  }

  void Particle::update(int interval) {
    // curl all the particles with the same angular offset
    m_direction += interval * 0.0003;

    double xSpeed = m_speed * cos(m_direction);
    double ySpeed = m_speed * sin(m_direction);

    m_x += xSpeed * interval;
    m_y += ySpeed * interval;

    // if a particle is off the screen we put it to the screen's center
    if(m_x < -1 || m_x > 1 || m_y < -1 || m_y > 1){
      init();
    }

    // additional effect => randomly put some particles to the screen's center
    // if(rand() < RAND_MAX / 100) {
    //   init();
    // }
  }
}