#include "../Entity/Entities.hpp"
#include "../Utilities/Button.hpp"

class GameScene : public Scene {
private:
  sf::Font font;
  sf::Text text;

  sf::Texture background;
  sf::Sprite backgroundSprite;

  Player* player;
  PipeHandler* pipeHandler;
  std::map<std::string, Button*> buttons;

  int i = 0;

public:
  GameScene(sf::RenderWindow* window, std::stack<Scene*>* scenes) : Scene(window, scenes) {
    player = new Player(this->window);
    pipeHandler = new PipeHandler(this->window);

    this->name = "game";

    pipeHandler->generatePipes();

    if (!background.loadFromFile("content/Textures/bg.jpeg")) {}
    background.setRepeated(true);
    backgroundSprite.setTexture(background);
    backgroundSprite.setScale(1.f, 1.2f);
    backgroundSprite.setTextureRect({ 0, 0, 800 + 284, 600 });
  }
  virtual ~GameScene() {
    delete player;
    delete pipeHandler;
  }

  void exitScene() {}

  void update(const float& delta) {
    this->updateMousePosition();
    this->inputs(delta);
    this->player->update(delta);

    if (!this->player->isDead()) {
      this->pipeHandler->update(delta);

      if (this->backgroundSprite.getPosition().x < -283) {
        this->backgroundSprite.setPosition(0, 0);
      }
      else {
        this->backgroundSprite.setPosition(this->backgroundSprite.getPosition().x - 1.f, 0);
      }
    }

    for (auto it : this->buttons) {
      it.second->update(this->mousePosView);
    }

    if (this->player->getPlayerPos().y > this->window->getSize().y + 200 && !this->player->isDead()) {
      this->player->setDead(true);
    }

    for (auto pipeIterator : this->pipeHandler->getPipes()) {
      if ((this->player->getSprite().getGlobalBounds().intersects(pipeIterator->getSprite_PipeTop().getGlobalBounds()) ||
           this->player->getSprite().getGlobalBounds().intersects(pipeIterator->getSprite_PipeBottom().getGlobalBounds())) && !this->player->isDead()) {
        this->player->setDead(true);
      }
    }

    if (this->player->isDead() && this->player->getPlayerPos().y > this->window->getSize().y + 200) {
      this->exit = true;
    }
  }

  void inputs(const float& delta) {
    this->checkForQuit();

    if (delta != 0.f) {}
  }

  void render(sf::RenderTarget* target = NULL) {
    if (!target) target = this->window;

    // this->font.loadFromFile("content/Fonts/Dosis-Light.ttf");

    for (auto it : this->buttons) it.second->render(target);

    target->draw(backgroundSprite);
    pipeHandler->render(target);
    player->render(target);
  }
};
