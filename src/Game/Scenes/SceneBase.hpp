#pragma once

#include "../Helper.hpp"

class Scene {
protected:
  std::stack<Scene*>* scenes;

  sf::RenderWindow* window;
  std::vector<sf::Texture*> textures;

  bool exit;

  sf::Vector2i mousePosScreen;
  sf::Vector2i mousePosWindow;
  sf::Vector2f mousePosView;

public:
  Scene(sf::RenderWindow* window, std::stack<Scene*>* scenes) {
    this->window = window;
    this->scenes = scenes;
    this->exit = false;
  }
  virtual ~Scene() {}

  const bool& getExit() const {
    return this->exit;
  }

  void updateMousePosition() {
    this->mousePosScreen = sf::Mouse::getPosition();
    this->mousePosWindow = sf::Mouse::getPosition(*this->window);
    this->mousePosView = this->window->mapPixelToCoords(sf::Mouse::getPosition(*this->window));
  }

  virtual void checkForQuit() {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
      this->exit = true;
    }
  }

  virtual void exitScene() = 0;
  virtual void update(const float& delta) = 0;
  virtual void render(sf::RenderTarget* target = NULL) = 0;
};
