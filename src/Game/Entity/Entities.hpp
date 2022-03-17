#include "../Utilities/Entity.hpp"

class Player : public Entity, public Moveable {
private:
  std::map<std::string, sf::Keyboard::Key> keymaps = {
    { "JUMP", sf::Keyboard::Space }
  };

  sf::Texture bird;
  sf::Sprite sprite;

  bool isJumping;
  bool dead;
  float SpeedFriction = .1f;

  float fallSpeed = 4.f;
  // float fallSpeed = 0.f;
  float fallSpeedValue;

  float jumpSpeed = 2.f;
  float jumpAngleIncrement = 1.f;
  float jumpSpeedValue;

  sf::SoundBuffer deathSoundBuffer;
  sf::Sound deathSound;
  bool soundplaying = false;

public:
  Player(sf::RenderWindow* window) : Entity(window) {
    this->speed = 120.f;

    this->deathSoundBuffer.loadFromFile("content/Audio/death.wav");
    this->deathSound.setBuffer(deathSoundBuffer);
    this->deathSound.setVolume(60);

    if (!this->bird.loadFromFile("content/Textures/bird.png")) {
      std::cout << "Error loading Bird Texture" << std::endl;
    }

    this->sprite.setTexture(this->bird);
    this->sprite.setTextureRect({ 0, 20, 125, 88 });

    this->sprite.setScale(sf::Vector2f(.4f, .4f));
    this->sprite.setPosition(
      (this->window->getSize().x / 2) - (this->sprite.getGlobalBounds().width / 2) - 100,
      (this->window->getSize().y / 2) - (this->sprite.getGlobalBounds().height / 2)
    );

    this->sprite.setOrigin(
      this->bird.getSize().x / 2,
      this->bird.getSize().y / 2
    );

    isJumping = false;
    dead = false;
  }

  virtual ~Player() {

  }

  void move(const float& dt, const float dir_x, const float dir_y) {
    this->sprite.move(dir_x * this->speed * dt, dir_y * this->speed * dt);
  }

  void update(const float& dt) {
    if (!dead) {
       // Checks for Key presses
      if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
        fallSpeedValue = -jumpSpeed;
      };

      // Moves the sprite
      this->move(dt, 0.f, fallSpeedValue);

      // Changes Jumping State
      if (fallSpeedValue >= -1) {
        isJumping = false;
      }
      else {
        isJumping = true;
      }

      // Change Rotation on the basis on either jumping or not
      if (!isJumping) {
        this->sprite.rotate(jumpAngleIncrement);
      }
      else {
        this->sprite.rotate(-2.5f);
      }

      // Puts Constraints on the maximum rotation angle, 90 and 320
      if (this->sprite.getRotation() > 90 && this->sprite.getRotation() < 180) {
        this->sprite.setRotation(90);
      }

      if (this->sprite.getRotation() > 270 && this->sprite.getRotation() < 320) {
        this->sprite.setRotation(320);
      }


      // Decreses fall Speed
      fallSpeedValue += SpeedFriction;
      if (fallSpeedValue > fallSpeed) {
        fallSpeedValue = fallSpeed;
      }
    }
    else {

      this->sprite.rotate(2.f);

      if (this->sprite.getRotation() > 90 && this->sprite.getRotation() < 180) {
        this->sprite.setRotation(90);
      }

      fallSpeedValue += SpeedFriction;
      if (fallSpeedValue > fallSpeed) {
        fallSpeedValue = fallSpeed;
      }

      this->move(dt, 0.f, fallSpeedValue);

      if (!soundplaying) {
        this->deathSound.play();
        soundplaying = true;
      }
    }
  }

  void render(sf::RenderTarget* target) {
    target->draw(this->sprite);
  }

  void setDead(bool isDead) {
    this->dead = isDead;
  }

  bool isDead() {
    return this->dead;
  }

  sf::Vector2f getPlayerPos() {
    return this->sprite.getPosition();
  }

  sf::Sprite getSprite() {
    return this->sprite;
  }
};

class Pipe : public Entity {
private:
  sf::Texture pipe;
  sf::Texture pipe2;
  sf::Sprite pipe_sprite;
  sf::Sprite pipe2_sprite;

public:
  Pipe(sf::RenderWindow* window, float topPipePos = 450, float bottomPipePos = 0, int spawnX = 800, float speed = -2.f) : Entity(window) {
    this->speed = speed;

    if (!this->pipe.loadFromFile("content/Textures/pipe.png")) {
      std::cout << "Error loading Pipe texture" << std::endl;
    }
    if (!this->pipe2.loadFromFile("content/Textures/pipe2.png")) {
      std::cout << "Error loading Pipe2 texture" << std::endl;
    }

    this->pipe_sprite.setTexture(this->pipe);
    this->pipe_sprite.setTextureRect({ 0, 0, 52, 400 });
    this->pipe_sprite.setPosition(spawnX - this->pipe.getSize().x, topPipePos);

    this->pipe2_sprite.setTexture(this->pipe2);
    this->pipe2_sprite.setTextureRect({ 0, 0, 52, 320 });
    this->pipe2_sprite.setPosition(spawnX - this->pipe.getSize().x, bottomPipePos);
  }

  virtual ~Pipe() {}

  void move() {
    this->pipe_sprite.move(sf::Vector2f(this->speed, 0));
    this->pipe2_sprite.move(sf::Vector2f(this->speed, 0));
  }

  void update(const float& dt) {
    if (dt > 1.f) {}
    this->move();
  }

  void render(sf::RenderTarget* target) {
    target->draw(pipe2_sprite);
    target->draw(pipe_sprite);
  }

  sf::Sprite getSprite_PipeTop() {
    return this->pipe2_sprite;
  }

  sf::Sprite getSprite_PipeBottom() {
    return this->pipe_sprite;
  }

  void setSpeed(float newSpeed) {
    this->speed = newSpeed;
  }

  float getSpeed() {
    return this->speed;
  }
};

class PipeHandler {
private:
  sf::RenderWindow* window;
  int maxGenLimitPerFrame;
  std::vector<Pipe*> pipes;

  int random(int max = 5) {
    return rand() % max;
  }

  // Top Range    = [lower: 250, hightest: 500, increment:  50]
  // Bottom Range = [lower: -200, hightest: 50 , increment: -50]
  int topRangeLow = 250;
  int topRangeHigh = 500;
  int topIncrement = 50;
  int sutaibleTopRange[5] = { 250, 300, 350, 400, 450 };

  int bottomRangeLow = -250;
  int bottomRangeHigh = 0;
  int bottomIncrement = -50;
  int sutaibleBottomRange[5] = { -200, -150, -100, -50, 0 };

  float speed = -2.f;
  float speedIncrement = -.1f;


public:
  PipeHandler(sf::RenderWindow* window, int maxLimitGenPerFrame = 1) {
    this->window = window;
    this->maxGenLimitPerFrame = maxLimitGenPerFrame;
  }

  virtual ~PipeHandler() {
    // for (int i = 0; i < maxGenLimitPerFrame; i++) {
    //   delete pipes.top();
    //   pipes.pop();
    // }

    for (int i = 0; i < maxGenLimitPerFrame; i++) {
      this->pipes.erase(this->pipes.begin());
    }
  }

  void generatePipes() {
    int spawnPos = 900;
    for (int i = 0; i < maxGenLimitPerFrame; i++) {
      int index = random();
      speed += speedIncrement;
      this->pipes.push_back(new Pipe(this->window, sutaibleTopRange[index], sutaibleBottomRange[index], spawnPos, speed));
      spawnPos += 300;
    }
  }

  void update(const float& dt) {
    for (auto it : pipes) {
      it->update(dt);

      if (it->getSprite_PipeTop().getPosition().x < -100 || it->getSprite_PipeBottom().getPosition().x < -100) {
        this->pipes.erase(this->pipes.begin());
        generatePipes();
      }
    }
  }

  void render(sf::RenderTarget* target) {
    for (auto it : pipes) {
      it->render(target);
    }
  }

  std::vector<Pipe*> getPipes() {
    return this->pipes;
  }
};
