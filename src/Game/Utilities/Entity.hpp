#include "../Helper.hpp"

class Entity {
protected:
  sf::RectangleShape shape;
  sf::RenderWindow* window;

  float speed;

public:
  Entity(sf::RenderWindow* window) {
    this->window = window;
  }

  virtual ~Entity() {

  }

  virtual void update(const float& dt) = 0;
  virtual void render(sf::RenderTarget* target) = 0;
};

class Moveable {
public:
  virtual void move(const float& dt, const float dir_x, const float dir_y) = 0;
};
