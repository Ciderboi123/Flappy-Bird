#include "SceneBase.hpp"
#include "GameScene.hpp"

class MainMenu : public Scene {
  sf::RectangleShape background;
  sf::Font font;
  sf::Text text;

  Button* button;

public:
  MainMenu(sf::RenderWindow* window, std::stack<Scene*>* scenes) : Scene(window, scenes) {
    this->font.loadFromFile("content/Fonts/Dosis-Light.ttf");

    this->background.setSize(sf::Vector2f(window->getSize().x, window->getSize().y));
    this->background.setFillColor(sf::Color::Magenta);

    this->button = new Button(100, 100, 150, 50,
    &this->font, "New Game",
    sf::Color(70, 70, 70, 200), sf::Color(150, 150, 150, 255), sf::Color(20, 20, 20, 200));

  }

  virtual ~MainMenu() {
    delete this->button;
  }

  void exitScene() {

  }

  void update(const float& dt) {
    if (dt > 1.f) {}
    this->checkForQuit();
    this->updateMousePosition();

    // for (auto it : this->buttons) it.second->update(this->mousePosView);
    this->button->update(this->mousePosView);

    if (this->button->isPressed()) {
      // std::cout << "Pressed" << std::endl;
      // this->exit = true;
      this->scenes->push(new GameScene(this->window, this->scenes));
    }
  }

  void render(sf::RenderTarget* target = NULL) {
    if (!target) target = this->window;

    // for (auto it : this->buttons) it.second->render(target);
    target->draw(this->background);
    this->button->render(target);
  }
};
