#ifndef __SWARM_H__
#define __SWARM_H__

#include "Particle.h"

namespace caveofprogramming {

class Swarm {
public:
  const static int NPARTICLES = 5000;

private:
  Particle *m_particles;
  int lastTime;

public:
  Swarm();
  virtual ~Swarm();
  void update(int elapsed);
  const Particle *const getParticles() { return m_particles; };
};

}

#endif
