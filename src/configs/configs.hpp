namespace configs {
class Position {
public:
  float x;
  float y;
  Position(float x, float y) : x(x), y(y) {};
};
class GameConfig {
public:
  /**
      Return Mario's position.
  */
  virtual Position MarioPos() = 0;
  /**
      Return Mario's position relative to the screen itself.
  */
  virtual Position MarioScreenPos() = 0;
  /**
    Return whether Bear5 can attack.
  */
  virtual bool Bear5CanAttack() = 0;
  /**
    Return whether the level is ending (or anything else that should reset Bear
    5's position).
   */
  virtual bool LevelEnding() = 0;
  /**
    Return whether Bear5 should attack regardless of if Mario touches him (warp
    zone, etc).
  */
  virtual bool Bear5AttackOverride() = 0;
  /**
    Return what level we're on.
  */
  virtual int Level() = 0;
  /**
    Return the speed that Bear 5 should move in this game.
    */
  virtual float Speed() = 0;

  /**
    Return whether to play overworld music or not.
  */
  virtual bool Music() = 0;

  virtual ~GameConfig() {};
};

class SMB1Config : public GameConfig {
public:
  Position MarioPos() override;
  Position MarioScreenPos() override;
  bool Bear5CanAttack() override;
  bool LevelEnding() override;
  bool Bear5AttackOverride() override;
  int Level() override;
  float Speed() override;
  bool Music() override;
};
} // namespace configs