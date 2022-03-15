#pragma once

#include "Helper.hpp"
#include "./Scenes/MainMenu.hpp"

class Game {
private:
  sf::RenderWindow* window;
  sf::Event event;
  sf::Clock clock;

  std::stack<Scene*> scenes;

  float delta;

  void init() {

    // std::cout << getConfig().dump(4) << std::endl;

    sf::VideoMode windowBounds(800, 600);
    std::string title = "Flappy Bird";
    unsigned framesLimit = 120;
    bool verticalSync = false;

    this->window = new sf::RenderWindow(windowBounds, title, sf::Style::Titlebar | sf::Style::Close);
    this->window->setFramerateLimit(framesLimit);
    this->window->setVerticalSyncEnabled(verticalSync);
    this->window->setKeyRepeatEnabled(true);

    sf::Image icon;
    icon.loadFromFile("content/logo.png");
    this->window->setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());

    this->scenes.push(new MainMenu(this->window, &this->scenes));
  }

  void stop() {
    window->close();

    std::cout << "Shutting Down App !" << std::endl;
  }

  void updateDelta() {
    this->delta = this->clock.restart().asSeconds();
  }

  void updateEvents() {
    while (this->window->pollEvent(event)) {
      if (event.type == sf::Event::Closed) stop();
    }
  }

public:
  Game() {
    init();
  };

  virtual ~Game() {

  };

  void update() {
    this->updateEvents();
    if (!this->scenes.empty()) {
      this->scenes.top()->update(this->delta);

      if (this->scenes.top()->getExit()) {
        this->scenes.top()->exitScene();
        delete this->scenes.top();
        this->scenes.pop();
      }
    }
    else {
      stop();
    }
  }

  void render() {
    window->clear();

    if (!this->scenes.empty()) this->scenes.top()->render(this->window);

    window->display();
  }

  void run() {
    while (this->window->isOpen()) {
      this->updateDelta();
      this->update();
      this->render();
    }
  }
};
