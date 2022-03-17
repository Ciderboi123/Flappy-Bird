#include "SceneBase.hpp"
#include "GameScene.hpp"

class MainMenu : public Scene {
  sf::Texture background;
  sf::Sprite backgroundSprite;
  sf::Font button_font;
  sf::Font Title_font;
  sf::Text text;

  Button* play_button;
  Button* exit_button;

  sf::SoundBuffer buttonSoundBuffer;
  sf::Sound buttonSound;
  bool soundPlayed = false;

  bool hoverd_text = false;

public:
  MainMenu(sf::RenderWindow* window, std::stack<Scene*>* scenes) : Scene(window, scenes) {
    this->button_font.loadFromFile("content/Fonts/PixellettersFull.ttf");
    this->Title_font.loadFromFile("content/Fonts/FlappyBirdy.ttf");

    this->buttonSoundBuffer.loadFromFile("content/Audio/click.wav");
    this->buttonSound.setBuffer(buttonSoundBuffer);

    this->name = "menu";

    this->text.setFont(this->Title_font);
    this->text.setString("Flappy Bird");
    this->text.setStyle(this->text.Bold);
    this->text.setFillColor(sf::Color::Black);
    this->text.setCharacterSize(100);
    this->text.setPosition(sf::Vector2f(
      (this->window->getSize().x / 2.f) - this->text.getGlobalBounds().width / 2.f,
      (this->window->getSize().y / 2.f) - this->text.getGlobalBounds().height / 2.f - 200
    ));

    if (!background.loadFromFile("content/Textures/bg.jpeg")) {}
    background.setRepeated(true);
    backgroundSprite.setTexture(background);
    backgroundSprite.setScale(1.f, 1.2f);
    backgroundSprite.setTextureRect({ 0, 0, 800 + 284, 600 });

    this->play_button = new Button(
      this->window->getSize().x / 2.f, this->window->getSize().y / 2.f + 50, 200, 50,
    &this->button_font, "Play Game",
    sf::Color(70, 70, 70, 200),
    sf::Color(150, 150, 150, 255),
    sf::Color(20, 20, 20, 200));

    this->exit_button = new Button(
      this->window->getSize().x / 2.f, this->window->getSize().y / 2.f + 150, 150, 50,
    &this->button_font, "Exit",
    sf::Color(70, 70, 70, 200),
    sf::Color(150, 150, 150, 255),
    sf::Color(20, 20, 20, 200));

  }

  virtual ~MainMenu() {
    delete this->play_button;
    delete this->exit_button;
  }

  void exitScene() {

  }

  void update(const float& dt) {
    if (dt > 1.f) {}
    this->checkForQuit();
    this->updateMousePosition();

    this->play_button->update(this->mousePosView);
    this->exit_button->update(this->mousePosView);

    if (this->play_button->isPressed() && !soundPlayed) {
      this->buttonSound.play();
      soundPlayed = true;
    }
    else soundPlayed = false;

    if (this->play_button->isPressed()) {
      this->scenes->push(new GameScene(this->window, this->scenes));
    }

    if (this->exit_button->isPressed()) {
      this->exit = true;
    }

    if (this->backgroundSprite.getPosition().x < -283) {
      this->backgroundSprite.setPosition(0, 0);
    }
    else {
      this->backgroundSprite.setPosition(this->backgroundSprite.getPosition().x - 1.f, 0);
    }
  }

  void render(sf::RenderTarget* target = NULL) {
    if (!target) target = this->window;

    target->draw(this->backgroundSprite);
    target->draw(this->text);
    this->play_button->render(target);
    this->exit_button->render(target);
  }
};
