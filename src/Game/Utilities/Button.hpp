#include "../Helper.hpp"

enum ButtonState {
  IDLE = 0,
  HOVER,
  ACTIVE
};

class Button {
private:
  short unsigned state;

  sf::RectangleShape shape;
  sf::Font* font;
  sf::Text text;

  sf::Color CIdle;
  sf::Color CHover;
  sf::Color CActive;

public:
  Button(float x,
         float y,
         float width,
         float height,
         sf::Font* font,
         std::string text,
         sf::Color idleColor,
         sf::Color hoverColor,
         sf::Color activeColor) {
    this->state = IDLE;

    this->shape.setSize(sf::Vector2f(width, height));
    this->shape.setPosition(sf::Vector2f(
      this->shape.getSize().x / -2 + x,
      this->shape.getSize().y / -2 + y
    ));


    this->font = font;
    this->text.setFont(*this->font);
    this->text.setString(text);
    this->text.setFillColor(sf::Color::White);
    this->text.setCharacterSize(50);
    this->text.setPosition(
      this->shape.getPosition().x + (this->shape.getGlobalBounds().width / 2.f) - this->text.getGlobalBounds().width / 2.f,
      this->shape.getPosition().y + (this->shape.getGlobalBounds().height / 2.f) - this->text.getGlobalBounds().height / 2.f - 25
    );

    this->CIdle = idleColor;
    this->CHover = hoverColor;
    this->CActive = activeColor;

    this->shape.setFillColor(this->CIdle);
  }

  virtual ~Button() {

  }

  bool isPressed() {
    if (this->state == ACTIVE) return true;
    return false;
  }

  void update(const sf::Vector2f mousePos) {
    this->state = IDLE;

    if (this->shape.getGlobalBounds().contains(mousePos)) {
      this->state = HOVER;

      if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
        this->state = ACTIVE;
      }
    }

    switch (state) {
      case IDLE:
        this->shape.setFillColor(this->CIdle);
        break;
      case HOVER:
        this->shape.setFillColor(this->CHover);
        break;
      case ACTIVE:
        this->shape.setFillColor(this->CActive);
        break;
      default:
        this->shape.setFillColor(this->CIdle);
        break;
    }
  }

  void render(sf::RenderTarget* target) {
    target->draw(this->shape);
    target->draw(this->text);
  }
};
