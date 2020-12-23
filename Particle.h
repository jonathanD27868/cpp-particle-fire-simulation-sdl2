#ifndef __PARTICLE_H__
#define __PARTICLE_H__


namespace caveofprogramming {

struct Particle {
  double m_x;
  double m_y;

private:
  double m_speed;
  double m_direction;

private:
  void init();

public:
  Particle();
  virtual ~Particle();
  void update(int interval);
};

}

#endif
