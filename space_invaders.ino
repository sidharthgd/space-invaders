#include <gamma.h>
#include <RGBmatrixPanel.h>
#include <Adafruit_GFX.h>




// define the wiring of the LED screen
const uint8_t CLK  = 8;
const uint8_t LAT = A3;
const uint8_t OE = 9;
const uint8_t A = A0;
const uint8_t B = A1;
const uint8_t C = A2;




// define the wiring of the inputs
const int POTENTIOMETER_PIN_NUMBER = 5;
const int BUTTON_PIN_NUMBER = 10;
const int UPDATE_TIME = 2000;


const int POTENTIOMETER_VALUE_INCREMENTATION = 32;




// global constant for the number of Invaders in the game
const int NUM_ENEMIES = 16;




// a global variable that represents the LED screen
RGBmatrixPanel matrix(A, B, C, CLK, LAT, OE, false);
bool optionHasBeenSelected = false;
bool yesPowerUps = false;
bool noPowerUps = false;




// the following functions are for printing messages
void print_level(int level);
void print_lives(int lives);
void game_over();
void print_boss_incoming();




class Color {
public:
   int red;
   int green;
   int blue;
   Color() {
     red = 0;
     green = 0;
     blue = 0;
   }
   Color(int r, int g, int b) {
     red = r;
     green = g;
     blue = b;
   }
   uint16_t to_333() const {
     return matrix.Color333(red, green, blue);
   }
};




const Color BLACK(0, 0, 0);
const Color RED(4, 0, 0);
const Color ORANGE(6, 1, 0);
const Color YELLOW(4, 4, 0);
const Color GREEN(0, 4, 0);
const Color BLUE(0, 0, 4);
const Color PURPLE(1, 0, 2);
const Color WHITE(4, 4, 4);
const Color LIME(2, 4, 0);
const Color AQUA(0, 4, 4);




class Invader {
public:
  // Constructors
  Invader() {
    x = 0;
    y = 0;
    strength = 0;
    heart = false;
    ice = false;
    cannon = false;
    isBoss1 = false;
    isBoss2 = false;
  }
  // sets values for private data members x and y
  Invader(int x_arg, int y_arg) {
    x = x_arg;
    y = y_arg;
  }
  // sets values for private data members
  Invader(int x_arg, int y_arg, int strength_arg) {
    x = x_arg;
    y = y_arg;
    strength = strength_arg;
  }
  // sets values for private data members
  void initialize(int x_arg, int y_arg, int strength_arg) {
    x = x_arg;
    y = y_arg;
    strength = strength_arg;
    heart = false;
    ice = false;
    cannon = false;
  }

  // getters
  int get_x() const {
    return x;
  }
  int get_y() const {
    return y;
  }
  int get_strength() const {
    return strength;
  }

  bool getBoss1() {
    return isBoss1;
  }

  bool getBoss2() {
    return isBoss2;
  }

  int set_strength(int s) {
   strength = s;
  }
  void setHeart(bool h){
     heart = h;
  }

  void setBoss1(bool b){
    isBoss1 = b;
  }

  void setBoss2(bool b) {
    isBoss2 = b;
  }
  
  void setIce(bool h){
     ice = h;
  }
  void setCannon(bool h){
     cannon = h;


  }
   bool isHeart() {
     return heart;
 }
   bool isIce() {
     return ice;
   }
   bool isCannon() {
     return cannon;
   }


  // Moves the Invader down the screen by one row
  // Modifies: y
  void move() {
    y = y - 1;
  }
  // draws the Invader if its strength is greater than 0
  // calls: draw_with_rgb
  void draw() {
    if (strength <= 0) {
      return;
    }
    if (isBoss1) {
      drawBossOne();
      return;
    }
    if (isBoss2) {
      drawBossTwo(YELLOW);
      return;
    }
    if (strength == 1 && !heart && !ice && !cannon){
      draw_with_rgb(RED, BLUE);
    }
    if (strength == 2){
      draw_with_rgb(ORANGE, BLUE);
    }
    if (strength == 3){
      draw_with_rgb(YELLOW, BLUE);
    }
    if (strength == 4){
      draw_with_rgb(GREEN, BLUE);
    }
    if (strength == 5){
      draw_with_rgb(BLUE, BLUE);
    }
    if (strength == 6){
      draw_with_rgb(PURPLE, BLUE);
    }
    if (strength == 7){
      draw_with_rgb(WHITE, BLUE);
    }
    if (strength == 1 && heart){
       draw_heart(RED);
    }
    if (strength == 1 && ice){
       draw_ice(AQUA);
    }
    if (strength == 1 && cannon){
       draw_cannon(RED, YELLOW);
    }
  }
  // draws black where the Invader used to be
  // calls: draw_with_rgb
  void erase() {
    if (isBoss1) {
      bossOneErase();
    }
    else if (isBoss2) {
      drawBossTwo(BLACK);
    }
    else {
      draw_with_rgb(BLACK, BLACK);
    }
  }  
  void heart_erase() {
    draw_heart(BLACK);
 }
  void ice_erase() {
    draw_ice(BLACK);
  }
  void cannon_erase(){
    draw_cannon(BLACK, BLACK);
  }


  // Invader is hit by a Cannonball.
  // Modifies: strength
  // calls: draw, erase
  void hit() {
    strength = strength - 1;
    if (strength <= 0){
      if (isBoss1){
        bossOneErase();
      }
      if (isBoss2) {
        drawBossTwo(BLACK);
      }
      else {
        erase();
      }
    }
    else {
      draw();
    }
  }

  void drawBossOne() {
    for (int i = 0; i < 10; i++) {
      matrix.drawPixel(x + i, y, PURPLE.to_333());
    }
    for (int i = -1; i < 11; i++) {
      matrix.drawPixel(x + i, y-1, PURPLE.to_333());
    }
    for (int i = -2; i < 12; i++) {
      matrix.drawPixel(x + i, y-2, PURPLE.to_333());
    }
    for (int i = -3; i < 13; i++) {
      matrix.drawPixel(x + i, y-3, PURPLE.to_333());
    }
  }

  void bossOneErase() {
    for (int i = 0; i < 10; i++) {
      matrix.drawPixel(x + i, y, BLACK.to_333());
    }
    for (int i = -1; i < 12; i++) {
      matrix.drawPixel(x + i, y-1, BLACK.to_333());
    }
    for (int i = -2; i < 14; i++) {
      matrix.drawPixel(x + i, y-2, BLACK.to_333());
    }
    for (int i = -3; i < 16; i++) {
      matrix.drawPixel(x + i, y-3, BLACK.to_333());
    }
  }

  void drawBossTwo(Color col) {
    for (int i = 0; i < 5; i++) {
      matrix.drawPixel(x + i, y, col.to_333());
    }
    for (int i = -1; i < 6; i++) {
      matrix.drawPixel(x + i, y-1, col.to_333());
    }
    for (int i = -2; i < 7; i++) {
      matrix.drawPixel(x + i, y-2, col.to_333());
    }
    for (int i = -3; i < 8; i++) {
      matrix.drawPixel(x + i, y-3, col.to_333());
    }
  }



private:
  int x;
  int y;
  int strength;
  bool heart;
  bool ice;
  bool cannon;
  bool isBoss1;
  bool isBoss2;
  // draws the Invader
  void draw_with_rgb(Color body_color, Color eye_color) {
    //1st row
    matrix.drawPixel(x, y, body_color.to_333());
    matrix.drawPixel(x+1, y, body_color.to_333());
    //2nd row
    matrix.drawPixel(x-1, y-1, body_color.to_333());
    matrix.drawPixel(x, y-1, eye_color.to_333());
    matrix.drawPixel(x+1, y-1, eye_color.to_333());
    matrix.drawPixel(x+2, y-1, body_color.to_333());
    //3rd row
    matrix.drawPixel(x-1, y-2, body_color.to_333());
    matrix.drawPixel(x, y-2, body_color.to_333());
    matrix.drawPixel(x+1, y-2, body_color.to_333());
    matrix.drawPixel(x+2, y-2, body_color.to_333());
    //4th row
    matrix.drawPixel(x-1, y-3, body_color.to_333());
    matrix.drawPixel(x+2, y-3, body_color.to_333());
  }

  void draw_heart(Color body_color) {
   //1st row
   //matrix.drawPixel(x-1, y, body_color.to_333());
  //matrix.drawPixel(x+2, y, body_color.to_333());
   //2nd row
  // matrix.drawPixel(x-1, y-1, body_color.to_333());
   matrix.drawPixel(x, y-1, body_color.to_333());
   matrix.drawPixel(x+1, y-1, body_color.to_333());
  // matrix.drawPixel(x+2, y-1, body_color.to_333());
   //3rd row
  // matrix.drawPixel(x-1, y-2, body_color.to_333());
   matrix.drawPixel(x, y-2, body_color.to_333());
   matrix.drawPixel(x+1, y-2, body_color.to_333());
 //  matrix.drawPixel(x+2, y-2, body_color.to_333());
   //4th row
  // matrix.drawPixel(x, y-3, body_color.to_333());
  // matrix.drawPixel(x+1, y-3, body_color.to_333());
 }
 void draw_ice(Color body_color) {
   //1st row
  // matrix.drawPixel(x-1, y, body_color.to_333());
   //matrix.drawPixel(x+2, y, body_color.to_333());
  // matrix.drawPixel(x+1, y, body_color.to_333());
  // matrix.drawPixel(x, y, body_color.to_333());
   //2nd row
 //  matrix.drawPixel(x-1, y-1, body_color.to_333());
   matrix.drawPixel(x, y-1, body_color.to_333());
   matrix.drawPixel(x+1, y-1, body_color.to_333());
 //  matrix.drawPixel(x+2, y-1, body_color.to_333());
   //3rd row
  // matrix.drawPixel(x-1, y-2, body_color.to_333());
   matrix.drawPixel(x, y-2, body_color.to_333());
   matrix.drawPixel(x+1, y-2, body_color.to_333());
  // matrix.drawPixel(x+2, y-2, body_color.to_333());
   //4th row
 //  matrix.drawPixel(x, y-3, body_color.to_333());
  // matrix.drawPixel(x+1, y-3, body_color.to_333());
  // matrix.drawPixel(x-1, y-3, body_color.to_333());
 //  matrix.drawPixel(x+2, y-3, body_color.to_333());
 }
 void draw_cannon(Color body_color, Color body_color2) {
   //1st row
   //matrix.drawPixel(x-1, y, body_color.to_333());
  // matrix.drawPixel(x+2, y, body_color.to_333());
  // matrix.drawPixel(x-1, y, body_color.to_333());
  // matrix.drawPixel(x+2, y, body_color.to_333());
   //2nd row
  // matrix.drawPixel(x-1, y-1, body_color.to_333());
   matrix.drawPixel(x, y-1, body_color.to_333());
   matrix.drawPixel(x+1, y-1, body_color2.to_333());
  // matrix.drawPixel(x+2, y-1, body_color.to_333());
   //3rd row
   //matrix.drawPixel(x-1, y-2, body_color.to_333());
   matrix.drawPixel(x, y-2, body_color2.to_333());
   matrix.drawPixel(x+1, y-2, body_color.to_333());
  // matrix.drawPixel(x+2, y-2, body_color.to_333());
   //4th row
  /// matrix.drawPixel(x, y-3, body_color.to_333());
  // matrix.drawPixel(x+1, y-3, body_color.to_333());
  // matrix.drawPixel(x-1, y, body_color.to_333());
   //matrix.drawPixel(x+2, y, body_color.to_333());
 }
};

void draw_powerups(Color body_color);
void yes_box(Color body_color);
void no_box(Color body_color);


class Cannonball {
public:
  Cannonball() {
    x = 0;
    y = 0;
    fired = false;
    fireTime = 0;
    moveTime = 0;
    cannonTime = 0;
  }
  // resets private data members to initial values
  void reset() {
    x = 0;
    y = 0;
    fired = false;
  }
  // getters
  int get_x() const {
    return x;
  }
  int get_y() const {
    return y;
  }
 
  bool has_been_fired() const {
    return fired;
  }
  // sets private data members
  void set_cannon_time(int n) {
   cannonTime = n;
  }


  void fire(int x_arg, int y_arg, unsigned long time, bool a) {
   int t = 0;
   if (time - cannonTime < 2500 ){t = 100;}
   else {t = 500;}
   if (time >= (fireTime + t)){ //+100 for powerup
     x = x_arg;
     y = y_arg;
     fired = true;
     fireTime = time;
   }
  }
  // moves the Cannonball and detects if it goes off the screen
  // Modifies: y, fired
  void move(unsigned long time, bool a) {
   int t = 0;
   if (time - cannonTime < 2500 ){t = 10;}
   else {t = 50;}
    if (time >= (moveTime + t)){ // +10 for powerup
     y += 1;
     fired = true;
     moveTime = time;
    }
  }
  // resets private data members to initial values
  void hit() {
    erase();
    reset();
  }
  // draws the Cannonball, if it is fired
  void draw() {
    matrix.drawPixel(x, y, ORANGE.to_333());
    matrix.drawPixel(x, y+1, ORANGE.to_333());
  }
  // draws black where the Cannonball used to be
  void erase() {
    matrix.drawPixel(x, y, BLACK.to_333());
    matrix.drawPixel(x, y+1, BLACK.to_333());
  }
private:
  int x;
  int y;
  bool fired;
  unsigned long fireTime;
  unsigned long moveTime;
  unsigned long cannonTime;
};




class Player {
public:
  Player() {
    x = 0;
    y = 0;
    lives = 3;
  }
   // getters
  int get_x() const {
    return x;
  }
  int get_y() const {
    return y;
  }
  int get_lives() const {
    return lives;
  }


  // setter
  void set_x(int x_arg) {
    x = x_arg;
  }


   // Modifies: lives
  void die() {
    lives -= 1;
  }
  void addLife(){
   lives += 1;
  }
  // draws the Player
  // calls: draw_with_rgb
  void draw() {
    draw_with_rgb(AQUA);
  }
  // draws black where the Player used to be
  // calls: draw_with_rgb
  void erase() {
    draw_with_rgb(BLACK);
  }




private:
  int x;
  int y;
  int lives;
  // sets private data members x and y to initial values
  void initialize(int x_arg, int y_arg) {
    x = x_arg;
    y = y_arg;
  }
  // draws the player
  void draw_with_rgb(Color color) {
    matrix.drawPixel(x, 0, color.to_333());
    matrix.drawPixel(x-1, 0, color.to_333());
    matrix.drawPixel(x+1, 0, color.to_333());
    matrix.drawPixel(x, 1, color.to_333());
  }
};




class Game {
public:
  Game() {
   level = 0;
   time = 0;
   enemiesTime = UPDATE_TIME;
   freezeTime = 0;
   cannonTime = 0;
   cannonMode = false;
   bossLevel = 1;
   buttonPress = false;
   for (int i = 0; i < NUM_ENEMIES; i++) {
     enemies[i] = Invader(0, 0, 0);
    }
  }

  int getLevel() {
    return level;
  }

  bool setButtonPress(bool b) {
    buttonPress = b;
  }
  // advances the game simulation one step and renders the graphics
  // see spec for details of game
  // Modifies: global variable matrix
  void update(int potentiometer_value, bool button_pressed) {
   
//bool optionHasBeenSelected = false;
//bool yesPowerUps = false;
//bool noPowerUps = false;
   buttonPress = button_pressed;

   if (!optionHasBeenSelected){
    draw_powerups(WHITE);
    if (potentiometer_value < 512){
      yes_box(RED);
      no_box(BLACK);
      if (button_pressed){
        yesPowerUps = true;
        optionHasBeenSelected = true;
        
      }
    }
    else {
      no_box(RED);
      yes_box(BLACK);
      if (button_pressed){
        noPowerUps = true;
        optionHasBeenSelected = true;
        
      }
    }
   }
   
   else { 
    //update time
    time = millis();
    //move invaders down
   if (level_cleared()){
    if (!(enemies[0].getBoss1())){
     level++;
    }
    if (enemies[0].getBoss1()) {
      enemies[0].setBoss1(false);
      bossLevel = 2;
    }
    else {
      bossLevel = 1;
    }
    reset_level();
    enemiesTime = millis() + UPDATE_TIME;
   }
   if (time >= enemiesTime) {
     updateEnemies();
     enemiesTime += UPDATE_TIME;
   }
    //draw player
    player.erase();
    player.set_x(31 - (potentiometer_value / POTENTIOMETER_VALUE_INCREMENTATION));
    player.draw();
    //draw CannonBall
    if (ball.has_been_fired()) {
      //ball hits top of screen
      if (ball.get_y() > 15) {
        ball.erase();
        ball.reset();
      }
      else {
        ball.erase();
        ball.move(time, cannonMode);
        ball.draw();
      }
    }
    else {
      if (buttonPress){
        ball.fire(player.get_x(), 2, time, cannonMode);
      }
    }

    for (int i = 0; i < NUM_ENEMIES; i++) {
      //check if cannonball hits boss2
      if (enemies[i].getBoss2()) {
        if ((ball.get_x() >= enemies[i].get_x() - 3) && (ball.get_x() <= enemies[i].get_x() + 7)) {
          if (ball.get_y() == enemies[i].get_y() - 4) {
            ball.hit();
            enemies[i].hit();
            if (enemies[i].get_strength() == 0) {
              enemies[i].setBoss2(false);
              splitIntoEnemies(i);
            }
          }
      }
    }
    }

    if (enemies[0].getBoss1()) {
      //check if cannonball hits boss1
      for (int i = 0; i < NUM_ENEMIES; i++) {
        if ((ball.get_x() >= 8 && ball.get_x() <= 23) && (ball.get_y() == enemies[i].get_y() - 4)) {
          ball.hit();
          enemies[i].hit();
        }
      }
    }

    else {
      //check if cannonball hits enemy
      for (int i = 0; i < NUM_ENEMIES; i++){
      if (!enemies[i].isHeart() && !enemies[i].isIce() && !enemies[i].isCannon()){
        if (enemies[i].get_strength() != 0){
          if ((ball.get_x() == enemies[i].get_x() + 2 &&
              ball.get_y() == enemies[i].get_y() - 3) ||
              (ball.get_x() == enemies[i].get_x() + 1 &&
              ball.get_y() == enemies[i].get_y() - 2) ||
              (ball.get_x() == enemies[i].get_x() &&
              ball.get_y() == enemies[i].get_y() - 2) ||
              (ball.get_x() == enemies[i].get_x() - 1 &&
              ball.get_y() == enemies[i].get_y() - 3)) {
                ball.hit();
                enemies[i].hit();
          }
        }
      }
      else {
        if (enemies[i].get_strength() != 0 && enemies[i].isHeart()){
          if ((ball.get_x() == enemies[i].get_x() + 1 &&
              ball.get_y() == enemies[i].get_y() - 2) ||
              (ball.get_x() == enemies[i].get_x() &&
              ball.get_y() == enemies[i].get_y() - 2)) {
                ball.hit();
                enemies[i].hit();
                player.addLife();
          }
        }
        if (enemies[i].get_strength() != 0 && enemies[i].isIce()){
          if ((ball.get_x() == enemies[i].get_x() + 1 &&
              ball.get_y() == enemies[i].get_y() - 2) ||
              (ball.get_x() == enemies[i].get_x() &&
              ball.get_y() == enemies[i].get_y() - 2)) {
                ball.hit();
                enemies[i].hit();
                freezeTime = time;
              
          }
        }
      
        if (enemies[i].get_strength() != 0 && enemies[i].isCannon()){
          if ((ball.get_x() == enemies[i].get_x() + 1 &&
              ball.get_y() == enemies[i].get_y() - 2) ||
              (ball.get_x() == enemies[i].get_x() &&
              ball.get_y() == enemies[i].get_y() - 2)) {
                ball.hit();
                enemies[i].hit();
                ball.set_cannon_time(time);
              
          }
        }
      }
      }
    }
      
       
  for (int i = 0; i < NUM_ENEMIES; i++){
    //check if boss2 hits player
    if (enemies[i].getBoss2()) {
      if ((player.get_x() >= enemies[i].get_x() - 3) && player.get_x() <= enemies[i].get_x() + 7) {
        if (player.get_y() == enemies[i].get_y() - 4) {
          if (level % 6 == 0) {
            game_over();
          }
          player.die();
          if (player.get_lives() == 0) {
            game_over();
          }
          bossLevel = 1;
          enemies[i].setBoss2(false);
          reset_level();
          enemiesTime = millis() + UPDATE_TIME;
        }
      }
    }
  }

  if (enemies[0].getBoss1()) {
    //check if boss1 hits player
    if ((player.get_x() >= 8 && player.get_x() <= 23) && player.get_y() == enemies[0].get_y() - 4) {
      if (level % 6 == 0) {
        game_over();
      }
      player.die();
      if (player.get_lives() == 0) {
        game_over();
      }
      reset_level();
      enemiesTime = millis() + UPDATE_TIME;
    }
  }
  

  else {
 //check if cannon is hit by invader
    for (int i = 0; i < NUM_ENEMIES; i++) {
      if (!enemies[i].isCannon() && !enemies[i].isIce() && !enemies[i].isHeart()){
      if (enemies[i].get_strength() > 0) {
        if ((((player.get_x() - 1 == enemies[i].get_x() + 2) ||
              (player.get_x() - 1 == enemies[i].get_x() - 1)) &&
              (enemies[i].get_y() == 3)) ||
            (((player.get_x() + 1 == enemies[i].get_x() + 2) ||
              (player.get_x() + 1 == enemies[i].get_x() - 1)) &&
              (enemies[i].get_y() == 3)) ||
            (((player.get_x() == enemies[i].get_x() + 2) ||
              (player.get_x() == enemies[i].get_x() - 1)) &&
              (enemies[i].get_y() == 4))) {
              if (level % 6 == 0) {
                game_over();
              }
              player.die();
              if (player.get_lives() == 0) {
                game_over();
              }
              reset_level();
              enemiesTime = millis() + UPDATE_TIME;
        }
      }
    }
    else {
      if (enemies[i].get_strength() > 0) {
        if ((((player.get_x() - 1 == enemies[i].get_x() + 1)) &&
              (enemies[i].get_y() == 2)) ||
            (((player.get_x() + 1 == enemies[i].get_x()) ||
              (player.get_x() + 1 == enemies[i].get_x() - 1)) &&
              (enemies[i].get_y() == 2)) ||
            (((player.get_x() == enemies[i].get_x()) ||
              (player.get_x() == enemies[i].get_x() + 1)) &&
              (enemies[i].get_y() == 3))) {
              enemies[i].hit();
              
              if (enemies[i].isCannon()){
                ball.set_cannon_time(time);
              }
              if (enemies[i].isHeart()){
                player.addLife();
              }
              if (enemies[i].isIce()){
                freezeTime = time;
              }
              
        }
      }
    }
    }
  }
  
    //check if invaders are at the bottom
   for (int i = 0; i < NUM_ENEMIES; i++) {
    if (!enemies[i].isCannon() && !enemies[i].isHeart() && !enemies[i].isIce()){
     if (enemies[i].get_y() == 3) {
      //Serial.println("die");
      if (level % 6 == 0) {
        game_over();
      }
      player.die();
      if (player.get_lives() == 0) {
        game_over();
      }
      reset_level();
      enemiesTime = millis() + UPDATE_TIME;
     }
   }
 }
}
} //added parenthesis



 private:
  int level;
  unsigned long time;
  unsigned long enemiesTime;
  unsigned long freezeTime;
  unsigned long cannonTime;
  bool cannonMode;
  bool buttonPress;
  int bossStage;
  int bossLevel;
  Player player;
  Cannonball ball;
  Invader enemies[NUM_ENEMIES];


  // check if Player defeated all Invaders in current level
  bool level_cleared() {
    for (int i = 0; i < NUM_ENEMIES; i++) {
    if (!enemies[i].isHeart() && !enemies[i].isIce() && !enemies[i].isCannon()){ 
      if (enemies[i].get_strength() > 0) {
        return false;
      }
    }
  }
    return true;
  }

  void splitIntoEnemies(int enemy) {
    int currY = enemies[0].get_y();
    if (enemy == 0) {
      enemies[0].setBoss2(false);
      for (int i = 0; i < 4; i++) {
        enemies[i].initialize((4*i) + 1, currY, 2);
        enemies[i].draw();
      }
    }
    else {
      enemies[6].setBoss2(false);
      for (int i = 4; i < 8; i++) {
        enemies[i].initialize((4*i) + 1, currY, 2);
        enemies[i].draw();
      }
    }
  }

  // set up a level
  void reset_level() {
    //Serial.println("resetting");
    if (!(bossLevel > 1)) {
      print_level(level);
      print_lives(player.get_lives());
    }
   int random2 = 0;
   if (level == 1) {
    buttonPress = false;
     for (int i = 0; i < 8; i++) {
       enemies[i] = Invader(((i * 4) + 1), 15, 1);
       randomSeed(analogRead(12));
       int randInt = random(1, 12);
       if (noPowerUps){
        randInt = 0;
       }
       if (randInt == 1) {
         int randPower = random(0, 3);
         switch(randPower) {
           case 0:
             enemies[i].setHeart(true);
             break;
          
           case 1:
             enemies[i].setIce(true);
             break;

          
         }
       }
       enemies[i].draw();
     }
   }


   else if (level == 2) {
     for (int i = 0; i < NUM_ENEMIES; i++) {
       if (i > 7) {
         enemies[i].initialize(((i * 4) + 1) % 32, 15, ((i + 1) % 2) + 1);
         random2 = random(1,12);
         if (noPowerUps){
        random2 = 0;
       }
       if (random2 == 3){
         enemies[i].set_strength(1);
         random2 = random(1,4);
         if (random2 == 1){
           enemies[i].setHeart(true);
         }
         if (random2 == 2){
           enemies[i].setIce(true);
         }
         if (random2 == 31){
           enemies[i].setCannon(true);
         }
       }
         enemies[i].draw();
       }
       else {
         enemies[i].initialize(((i * 4) + 1) % 32, 11, (i % 2) + 1);
         random2 = random(1,12);
         if (noPowerUps){
        random2 = 0;
       }
       if (random2 == 3){
         enemies[i].set_strength(1);
         random2 = random(1,4);
         if (random2 == 1){
           enemies[i].setHeart(true);
         }
         if (random2 == 2){
           enemies[i].setIce(true);
         }
         if (random2 == 31){
           enemies[i].setCannon(true);
         }
       }
         enemies[i].draw();


       }
     }
   }


   else if (level == 3) {
     for (int i = 0; i < NUM_ENEMIES; i++) {
       if (i > 7) {
         enemies[i].initialize(29 - ((i-8) * 4), 15, ((i - 8) % 5) + 1);
         random2 = random(1,12);
         if (noPowerUps){
        random2 = 0;
       }
       if (random2 == 3){
         enemies[i].set_strength(1);
         random2 = random(1,4);
         if (random2 == 1){
           enemies[i].setHeart(true);
         }
         if (random2 == 2){
           enemies[i].setIce(true);
         }
         if (random2 == 31){
           enemies[i].setCannon(true);
         }
       }
         enemies[i].draw();
       }
       else {
         enemies[i].initialize(29 - (4 * i), 11, ((i+8) % 5) + 1);
         random2 = random(1,12);
         if (noPowerUps){
        random2 = 0;
       }
         enemies[i].set_strength(1);
       if (random2 == 3){
         random2 = random(1,4);
         if (random2 == 1){
           enemies[i].setHeart(true);
         }
         if (random2 == 2){
           enemies[i].setIce(true);
         }
         if (random2 == 10){
           enemies[i].setCannon(true);
         }
       }
         enemies[i].draw();
       }
     }
   }


   else if (level == 4) {
     for (int i = 0; i < NUM_ENEMIES; i++) {
       if (i > 7) {
         enemies[i].initialize(((i * 4) + 1) % 32, 15, (4 + (i) % 2));
         random2 = random(1,12);
         if (noPowerUps){
        random2 = 0;
       }
       if (random2 == 3){
         enemies[i].set_strength(1);
         random2 = random(1,4);
         if (random2 == 1){
           enemies[i].setHeart(true);
         }
         if (random2 == 2){
           enemies[i].setIce(true);
         }
         if (random2 == 10){
           enemies[i].setCannon(true);
         }
       }
         enemies[i].draw();


       }
       else {
         enemies[i].initialize(((i * 4) + 1) % 32, 11, (2 + ((i + 1) % 2)));
          random2 = random(1,12);
          if (noPowerUps){
        random2 = 0;
       }
       if (random2 == 3){
         enemies[i].set_strength(1);
         random2 = random(1,4);
         if (random2 == 1){
           enemies[i].setHeart(true);
         }
         if (random2 == 2){
           enemies[i].setIce(true);
         }
         if (random2 == 10){
           enemies[i].setCannon(true);
         }
       }
         enemies[i].draw();
       }
     }
   }

  else if (level % 6 == 0) {
    if (bossLevel == 1) {
      print_boss_incoming();
      enemies[0].initialize(11, 15, 10);
      enemies[0].setBoss1(true);
      enemies[0].draw();
      for (int i = 1; i < 16; i++) {
        enemies[i].initialize(0, 0, 0);
      } 
    }
    if (bossLevel == 2) {
      int currentY = enemies[0].get_y();
      enemies[0].initialize(6, currentY, 7);
      enemies[0].setBoss1(false);
      enemies[0].setBoss2(true);
      enemies[6].initialize(21, currentY, 7);
      enemies[6].setBoss2(true);
      enemies[0].draw();
      enemies[6].draw();
      for (int i = 2; i < 16; i++) {
        if (i == 6) {
          continue;
        }
        enemies[i].initialize(0, 0, 0);
      } 
    }
  }

  else {
     for (int i = 0; i < NUM_ENEMIES; i++) {
       if (i > 7) {
         enemies[i].initialize(((i * 4) + 1) % 32, 15, random(1,8));
          random2 = random(1,12);
          if (noPowerUps){
        random2 = 0;
       }
       if (random2 == 3){
         enemies[i].set_strength(1);
         random2 = random(1,4);
         if (random2 == 1){
           enemies[i].setHeart(true);
         }
         if (random2 == 2){
           enemies[i].setIce(true);
         }
         if (random2 == 31){
           enemies[i].setCannon(true);
         }
       }
         enemies[i].draw();
       }
       else {
         enemies[i].initialize(((i * 4) + 1) % 32, 11, random(1,8));
          random2 = random(1,12);
          if (noPowerUps){
        random2 = 0;
       }
       if (random2 == 3){
         enemies[i].set_strength(1);
         random2 = random(1,4);
         if (random2 == 1){
           enemies[i].setHeart(true);
         }
         if (random2 == 2){
           enemies[i].setIce(true);
         }
         if (random2 == 31){
           enemies[i].setCannon(true);
         }
       }
         enemies[i].draw();
      
       }
     }
   }
 }

 //move invaders down
 void updateEnemies() {
   if (level == 1) {
     for (int i = 0; i < 8; i++) {
       if (enemies[i].isHeart()){
          enemies[i].heart_erase();
        }
       else if (enemies[i].isIce()){
         enemies[i].ice_erase();
       }
        else {
      enemies[i].erase();
        }
      if (time - 5000 > freezeTime) enemies[i].move();
      enemies[i].draw();
     }
   }
   else {
     for (int i = 0; i < NUM_ENEMIES; i++){
       enemies[i].draw();
     }
     if(!firstRowCleared()){
       for (int i = 0; i < 8; i++) {
         enemies[i].erase();
         if (time - 5000 > freezeTime) enemies[i].move();
         enemies[i].draw();
       }
     }
     else {
       for (int i = 8; i < NUM_ENEMIES; i++) {
         enemies[i].erase();
         if (time - 5000 > freezeTime) enemies[i].move();
         enemies[i].draw();
       }
     }
   }
 }




 bool firstRowCleared() {
   for (int i = 0; i < 8; i++) {
     if (enemies[i].get_strength() != 0) {
       return false;
     }
   }
   return true;
 }
};




// a global variable that represents the game Space Invaders
Game game;




// see https://www.arduino.cc/reference/en/language/structure/sketch/setup/
void setup() {
Serial.begin(9600);
pinMode(BUTTON_PIN_NUMBER, INPUT);
matrix.begin();
draw_welcome(AQUA);
delay(2000);
matrix.fillScreen(BLACK.to_333());

//while (!optionHasBeenSelected){
  //draw_powerups(AQUA);
  //yes_box(RED);
  //no_box(RED);
  //if (potentiometer_value < 512){
   // Serial.print(potentiometer_value);
  //}


}



//}




// see https://www.arduino.cc/reference/en/language/structure/sketch/loop/
void loop() {
int potentiometer_value = analogRead(POTENTIOMETER_PIN_NUMBER);
bool button_pressed = (digitalRead(BUTTON_PIN_NUMBER) == HIGH);
game.update(potentiometer_value, button_pressed);
}




// displays Level
void print_level(int level) {
 matrix.setCursor(0,0);
 matrix.setRotation(2);
 matrix.fillScreen(BLACK.to_333());
 delay(1000);
 matrix.print("Level");
 matrix.print(level);
 delay(2000);
 matrix.fillScreen(BLACK.to_333());
 matrix.setRotation(0);


}


void print_boss_incoming() {
 matrix.setCursor(0,0);
 matrix.setRotation(2);
 matrix.fillScreen(BLACK.to_333());
 delay(1000);
 matrix.print("Boss Level");
 delay(2000);
 matrix.fillScreen(BLACK.to_333());
 delay(1000);
 matrix.setCursor(0,0);
 matrix.print("Sudden");
 delay(1000);
 matrix.fillScreen(BLACK.to_333());
 matrix.setCursor(0,0);
 matrix.print("Death");
 delay(2000);
 matrix.fillScreen(BLACK.to_333());
 delay(1000);
 matrix.setRotation(0);
}




// displays number of lives
void print_lives(int lives) {
 matrix.setCursor(0,0);
 matrix.setRotation(2);
 matrix.fillScreen(BLACK.to_333());
 delay(1000);
 matrix.print("Lives");
 matrix.print(lives);
 delay(2000);
 matrix.fillScreen(BLACK.to_333());
 matrix.setRotation(0);


}




// displays "game over"
void game_over() {
 matrix.setCursor(0,0);
 matrix.fillScreen(BLACK.to_333());
 matrix.setRotation(2);
 matrix.print("Game Over");
 delay(99999999999);
}
void draw_welcome(Color body_color){
  matrix.drawPixel(31, 12, body_color.to_333());
  matrix.drawPixel(31, 11, body_color.to_333());
  matrix.drawPixel(31, 10, body_color.to_333());
  matrix.drawPixel(31, 9, body_color.to_333());
  matrix.drawPixel(31, 8, body_color.to_333());
  matrix.drawPixel(31, 7, body_color.to_333());
  matrix.drawPixel(31, 6, body_color.to_333());
  matrix.drawPixel(31, 5, body_color.to_333());
  matrix.drawPixel(31, 4, body_color.to_333());
  matrix.drawPixel(30, 4, body_color.to_333());
  matrix.drawPixel(29, 4, body_color.to_333());
  matrix.drawPixel(29, 4, body_color.to_333());
  matrix.drawPixel(29, 5, body_color.to_333());
  matrix.drawPixel(29, 6, body_color.to_333());
  matrix.drawPixel(29, 7, body_color.to_333());
  matrix.drawPixel(28, 4, body_color.to_333());
  matrix.drawPixel(27, 4, body_color.to_333());
  matrix.drawPixel(27, 5, body_color.to_333());
  matrix.drawPixel(27, 6, body_color.to_333());
  matrix.drawPixel(27, 7, body_color.to_333());
  matrix.drawPixel(27, 8, body_color.to_333());
  matrix.drawPixel(27, 9, body_color.to_333());
  matrix.drawPixel(27, 10, body_color.to_333());
  matrix.drawPixel(27, 11, body_color.to_333());
  matrix.drawPixel(27, 12, body_color.to_333());
  matrix.drawPixel(25, 12, body_color.to_333());
  matrix.drawPixel(24, 12, body_color.to_333());
  matrix.drawPixel(23, 12, body_color.to_333());
  matrix.drawPixel(25, 11, body_color.to_333());
  matrix.drawPixel(25, 10, body_color.to_333());
  matrix.drawPixel(25, 9, body_color.to_333());
  matrix.drawPixel(25, 8, body_color.to_333());
  matrix.drawPixel(25, 7, body_color.to_333());
  matrix.drawPixel(25, 6, body_color.to_333());
  matrix.drawPixel(25, 5, body_color.to_333());
  matrix.drawPixel(25, 4, body_color.to_333());
  matrix.drawPixel(24, 8, body_color.to_333());
  matrix.drawPixel(23, 8, body_color.to_333());
  matrix.drawPixel(24, 4, body_color.to_333());
  matrix.drawPixel(23, 4, body_color.to_333());
  matrix.drawPixel(21, 12, body_color.to_333());
  matrix.drawPixel(21, 11, body_color.to_333());
  matrix.drawPixel(21, 10, body_color.to_333());
  matrix.drawPixel(21, 9, body_color.to_333());
  matrix.drawPixel(21, 8, body_color.to_333());
  matrix.drawPixel(21, 7, body_color.to_333());
  matrix.drawPixel(21, 6, body_color.to_333());
  matrix.drawPixel(21, 5, body_color.to_333());
  matrix.drawPixel(21, 4, body_color.to_333());
  matrix.drawPixel(20, 4, body_color.to_333());
  matrix.drawPixel(19, 4, body_color.to_333());
  matrix.drawPixel(17, 12, body_color.to_333());
  matrix.drawPixel(17, 11, body_color.to_333());
  matrix.drawPixel(17, 10, body_color.to_333());
  matrix.drawPixel(17, 9, body_color.to_333());
  matrix.drawPixel(17, 8, body_color.to_333());
  matrix.drawPixel(17, 7, body_color.to_333());
  matrix.drawPixel(17, 6, body_color.to_333());
  matrix.drawPixel(17, 5, body_color.to_333());
  matrix.drawPixel(17, 4, body_color.to_333());
  matrix.drawPixel(16, 12, body_color.to_333());
  matrix.drawPixel(15, 12, body_color.to_333());
  matrix.drawPixel(16, 4, body_color.to_333());
  matrix.drawPixel(15, 4, body_color.to_333());
  matrix.drawPixel(16, 12, body_color.to_333());
  matrix.drawPixel(13, 12, body_color.to_333());
  matrix.drawPixel(13, 11, body_color.to_333());
  matrix.drawPixel(13, 10, body_color.to_333());
  matrix.drawPixel(13, 9, body_color.to_333());
  matrix.drawPixel(13, 8, body_color.to_333());
  matrix.drawPixel(13, 7, body_color.to_333());
  matrix.drawPixel(13, 6, body_color.to_333());
  matrix.drawPixel(13, 5, body_color.to_333());
  matrix.drawPixel(13, 4, body_color.to_333());
  matrix.drawPixel(12, 12, body_color.to_333());
  matrix.drawPixel(11, 12, body_color.to_333());
  matrix.drawPixel(12, 4, body_color.to_333());
  matrix.drawPixel(11, 4, body_color.to_333());
  matrix.drawPixel(10, 12, body_color.to_333());
  matrix.drawPixel(10, 11, body_color.to_333());
  matrix.drawPixel(10, 10, body_color.to_333());
  matrix.drawPixel(10, 9, body_color.to_333());
  matrix.drawPixel(10, 8, body_color.to_333());
  matrix.drawPixel(10, 7, body_color.to_333());
  matrix.drawPixel(10, 6, body_color.to_333());
  matrix.drawPixel(10, 5, body_color.to_333());
  matrix.drawPixel(10, 4, body_color.to_333());
  matrix.drawPixel(8, 12, body_color.to_333());
  matrix.drawPixel(8, 11, body_color.to_333());
  matrix.drawPixel(8, 10, body_color.to_333());
  matrix.drawPixel(8, 9, body_color.to_333());
  matrix.drawPixel(8, 8, body_color.to_333());
  matrix.drawPixel(8, 7, body_color.to_333());
  matrix.drawPixel(8, 6, body_color.to_333());
  matrix.drawPixel(8, 5, body_color.to_333());
  matrix.drawPixel(8, 4, body_color.to_333());
  matrix.drawPixel(7, 11, body_color.to_333());
  matrix.drawPixel(6, 10, body_color.to_333());
  matrix.drawPixel(5, 11, body_color.to_333());
  matrix.drawPixel(4, 12, body_color.to_333());
  matrix.drawPixel(4, 11, body_color.to_333());
  matrix.drawPixel(4, 10, body_color.to_333());
  matrix.drawPixel(4, 9, body_color.to_333());
  matrix.drawPixel(4, 8, body_color.to_333());
  matrix.drawPixel(4, 7, body_color.to_333());
  matrix.drawPixel(4, 6, body_color.to_333());
  matrix.drawPixel(4, 5, body_color.to_333());
  matrix.drawPixel(4, 4, body_color.to_333());
  matrix.drawPixel(2, 12, body_color.to_333());
  matrix.drawPixel(2, 11, body_color.to_333());
  matrix.drawPixel(2, 10, body_color.to_333());
  matrix.drawPixel(2, 9, body_color.to_333());
  matrix.drawPixel(2, 8, body_color.to_333());
  matrix.drawPixel(2, 7, body_color.to_333());
  matrix.drawPixel(2, 6, body_color.to_333());
  matrix.drawPixel(2, 5, body_color.to_333());
  matrix.drawPixel(2, 4, body_color.to_333());
  matrix.drawPixel(1, 12, body_color.to_333());
  matrix.drawPixel(0, 12, body_color.to_333());
  matrix.drawPixel(1, 8, body_color.to_333());
  matrix.drawPixel(0, 8, body_color.to_333());
  matrix.drawPixel(1, 4, body_color.to_333());
  matrix.drawPixel(0, 4, body_color.to_333());
}

void draw_powerups(Color body_color){
//Letter P
matrix.drawPixel(31, 15, body_color.to_333());
matrix.drawPixel(31, 14, body_color.to_333());
matrix.drawPixel(31, 13, body_color.to_333());
matrix.drawPixel(31, 12, body_color.to_333());
matrix.drawPixel(31, 11, body_color.to_333());
matrix.drawPixel(30, 15, body_color.to_333());
matrix.drawPixel(29, 15, body_color.to_333());
matrix.drawPixel(30, 13, body_color.to_333());
matrix.drawPixel(29, 15, body_color.to_333());
matrix.drawPixel(29, 15, body_color.to_333());
matrix.drawPixel(29, 14, body_color.to_333());
matrix.drawPixel(29, 13, body_color.to_333());


//Letter O
matrix.drawPixel(27, 15, body_color.to_333());
matrix.drawPixel(27, 14, body_color.to_333());
matrix.drawPixel(27, 13, body_color.to_333());
matrix.drawPixel(27, 12, body_color.to_333());
matrix.drawPixel(27, 11, body_color.to_333());


matrix.drawPixel(25, 15, body_color.to_333());
matrix.drawPixel(25, 14, body_color.to_333());
matrix.drawPixel(25, 13, body_color.to_333());
matrix.drawPixel(25, 12, body_color.to_333());
matrix.drawPixel(25, 11, body_color.to_333());


matrix.drawPixel(26, 15, body_color.to_333());
matrix.drawPixel(26, 11, body_color.to_333());


//Letter W
matrix.drawPixel(23, 15, body_color.to_333());
matrix.drawPixel(23, 14, body_color.to_333());
matrix.drawPixel(23, 13, body_color.to_333());
matrix.drawPixel(23, 12, body_color.to_333());
matrix.drawPixel(23, 11, body_color.to_333());


matrix.drawPixel(21, 15, body_color.to_333());
matrix.drawPixel(21, 14, body_color.to_333());
matrix.drawPixel(21, 13, body_color.to_333());
matrix.drawPixel(21, 12, body_color.to_333());
matrix.drawPixel(21, 11, body_color.to_333());


matrix.drawPixel(19, 15, body_color.to_333());
matrix.drawPixel(19, 14, body_color.to_333());
matrix.drawPixel(19, 13, body_color.to_333());
matrix.drawPixel(19, 12, body_color.to_333());
matrix.drawPixel(19, 11, body_color.to_333());


matrix.drawPixel(22, 12, body_color.to_333());
matrix.drawPixel(22, 11, body_color.to_333());


matrix.drawPixel(20, 12, body_color.to_333());
matrix.drawPixel(20, 11, body_color.to_333());


//Letter E
matrix.drawPixel(17, 15, body_color.to_333());
matrix.drawPixel(17, 14, body_color.to_333());
matrix.drawPixel(17, 13, body_color.to_333());
matrix.drawPixel(17, 12, body_color.to_333());
matrix.drawPixel(17, 11, body_color.to_333());


matrix.drawPixel(16, 15, body_color.to_333());
matrix.drawPixel(16, 13, body_color.to_333());
matrix.drawPixel(16, 11, body_color.to_333());


//Letter R
matrix.drawPixel(14, 15, body_color.to_333());
matrix.drawPixel(14, 14, body_color.to_333());
matrix.drawPixel(14, 13, body_color.to_333());
matrix.drawPixel(14, 12, body_color.to_333());
matrix.drawPixel(14, 11, body_color.to_333());


matrix.drawPixel(13, 15, body_color.to_333());
matrix.drawPixel(13, 13, body_color.to_333());
matrix.drawPixel(13, 12, body_color.to_333());


matrix.drawPixel(12, 15, body_color.to_333());
matrix.drawPixel(12, 14, body_color.to_333());
matrix.drawPixel(12, 13, body_color.to_333());
matrix.drawPixel(12, 11, body_color.to_333());


//Letter U
matrix.drawPixel(10, 15, body_color.to_333());
matrix.drawPixel(10, 14, body_color.to_333());
matrix.drawPixel(10, 13, body_color.to_333());
matrix.drawPixel(10, 12, body_color.to_333());
matrix.drawPixel(10, 11, body_color.to_333());


matrix.drawPixel(9, 11, body_color.to_333());


matrix.drawPixel(8, 15, body_color.to_333());
matrix.drawPixel(8, 14, body_color.to_333());
matrix.drawPixel(8, 13, body_color.to_333());
matrix.drawPixel(8, 12, body_color.to_333());
matrix.drawPixel(8, 11, body_color.to_333());


//Letter P
matrix.drawPixel(6, 15, body_color.to_333());
matrix.drawPixel(6, 14, body_color.to_333());
matrix.drawPixel(6, 13, body_color.to_333());
matrix.drawPixel(6, 12, body_color.to_333());
matrix.drawPixel(6, 11, body_color.to_333());


matrix.drawPixel(4, 15, body_color.to_333());
matrix.drawPixel(4, 14, body_color.to_333());
matrix.drawPixel(4, 13, body_color.to_333());


matrix.drawPixel(5, 15, body_color.to_333());
matrix.drawPixel(5, 13, body_color.to_333());


//Letter S
matrix.drawPixel(2, 15, body_color.to_333());
matrix.drawPixel(2, 14, body_color.to_333());
matrix.drawPixel(2, 13, body_color.to_333());
matrix.drawPixel(2, 11, body_color.to_333());


matrix.drawPixel(0, 15, body_color.to_333());
matrix.drawPixel(0, 13, body_color.to_333());
matrix.drawPixel(0, 12, body_color.to_333());
matrix.drawPixel(0, 11, body_color.to_333());


matrix.drawPixel(1, 15, body_color.to_333());
matrix.drawPixel(1, 13, body_color.to_333());
matrix.drawPixel(1, 11, body_color.to_333());


//Yes and no
matrix.drawPixel(22, 8, body_color.to_333());
matrix.drawPixel(22, 7, body_color.to_333());
matrix.drawPixel(22, 6, body_color.to_333());
matrix.drawPixel(21, 6, body_color.to_333());
matrix.drawPixel(21, 5, body_color.to_333());
matrix.drawPixel(21, 4, body_color.to_333());
matrix.drawPixel(21, 3, body_color.to_333());
matrix.drawPixel(20, 8, body_color.to_333());
matrix.drawPixel(20, 7, body_color.to_333());
matrix.drawPixel(20, 6, body_color.to_333());
matrix.drawPixel(18, 3, body_color.to_333());
matrix.drawPixel(17, 4, body_color.to_333());
matrix.drawPixel(16, 5, body_color.to_333());
matrix.drawPixel(15, 6, body_color.to_333());
matrix.drawPixel(14, 7, body_color.to_333());
matrix.drawPixel(13, 8, body_color.to_333());
matrix.drawPixel(11, 8, body_color.to_333());
matrix.drawPixel(11, 7, body_color.to_333());
matrix.drawPixel(11, 6, body_color.to_333());
matrix.drawPixel(11, 5, body_color.to_333());
matrix.drawPixel(11, 4, body_color.to_333());
matrix.drawPixel(11, 3, body_color.to_333());
matrix.drawPixel(10, 7, body_color.to_333());
matrix.drawPixel(10, 6, body_color.to_333());
matrix.drawPixel(9, 6, body_color.to_333());
matrix.drawPixel(9, 5, body_color.to_333());
matrix.drawPixel(8, 8, body_color.to_333());
matrix.drawPixel(8, 7, body_color.to_333());
matrix.drawPixel(8, 6, body_color.to_333());
matrix.drawPixel(8, 5, body_color.to_333());
matrix.drawPixel(8, 4, body_color.to_333());
matrix.drawPixel(8, 3, body_color.to_333());
}



void yes_box(Color body_color) {
matrix.drawPixel(23, 9, body_color.to_333());
matrix.drawPixel(23, 8, body_color.to_333());
matrix.drawPixel(23, 7, body_color.to_333());
matrix.drawPixel(23, 6, body_color.to_333());
matrix.drawPixel(23, 5, body_color.to_333());
matrix.drawPixel(23, 4, body_color.to_333());
matrix.drawPixel(23, 3, body_color.to_333());
matrix.drawPixel(23, 2, body_color.to_333());
matrix.drawPixel(22, 9, body_color.to_333());
matrix.drawPixel(21, 9, body_color.to_333());
matrix.drawPixel(20, 9, body_color.to_333());
matrix.drawPixel(19, 9, body_color.to_333());
matrix.drawPixel(19, 8, body_color.to_333());
matrix.drawPixel(19, 7, body_color.to_333());
matrix.drawPixel(19, 6, body_color.to_333());
matrix.drawPixel(19, 5, body_color.to_333());
matrix.drawPixel(19, 4, body_color.to_333());
matrix.drawPixel(19, 3, body_color.to_333());
matrix.drawPixel(19, 2, body_color.to_333());
matrix.drawPixel(20, 2, body_color.to_333());
matrix.drawPixel(21, 2, body_color.to_333());
matrix.drawPixel(22, 2, body_color.to_333());
}




void no_box(Color body_color){
matrix.drawPixel(12, 9, body_color.to_333());
matrix.drawPixel(12, 8, body_color.to_333());
matrix.drawPixel(12, 7, body_color.to_333());
matrix.drawPixel(12, 6, body_color.to_333());
matrix.drawPixel(12, 5, body_color.to_333());
matrix.drawPixel(12, 4, body_color.to_333());
matrix.drawPixel(12, 3, body_color.to_333());
matrix.drawPixel(12, 2, body_color.to_333());
matrix.drawPixel(11, 9, body_color.to_333());
matrix.drawPixel(10, 9, body_color.to_333());
matrix.drawPixel(9, 9, body_color.to_333());
matrix.drawPixel(8, 9, body_color.to_333());
matrix.drawPixel(7, 9, body_color.to_333());
matrix.drawPixel(7, 8, body_color.to_333());
matrix.drawPixel(7, 7, body_color.to_333());
matrix.drawPixel(7, 6, body_color.to_333());
matrix.drawPixel(7, 5, body_color.to_333());
matrix.drawPixel(7, 4, body_color.to_333());
matrix.drawPixel(7, 3, body_color.to_333());
matrix.drawPixel(7, 2, body_color.to_333());
matrix.drawPixel(8, 2, body_color.to_333());
matrix.drawPixel(9, 2, body_color.to_333());
matrix.drawPixel(10, 2, body_color.to_333());
matrix.drawPixel(11, 2, body_color.to_333());
}






