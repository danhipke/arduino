// Pong

#include <stdint.h>
#include <SeeedTouchScreen.h>
#include <TFTv2.h>
#include <SPI.h>

typedef struct pong_point {
  int x;
  int y;
} PongPoint;
// For better pressure precision, we need to know the resistance
// between X+ and X- Use any multimeter to read it
// The 2.8" TFT Touch shield has 300 ohms across the X plate

TouchScreen ts = TouchScreen(XP, YP, XM, YM); //init TouchScreen port pins
PongPoint player_1 = { 100, 5 };
PongPoint player_2 = { 100, 305 };
PongPoint ball = { 120, 160 };
int ball_speed = 2;
int ball_x_vel;
int ball_y_vel;
int ball_size = 5;
long player_1_score = 1;
long player_2_score = 10;
int paddle_width = 8;
int paddle_length = 40;
int game_state = -1;

void drawGame() {
  if (game_state == -1) {
    Tft.drawString("PONG", 200, 60, 8, WHITE);
  } else if (game_state == 0) {
    ball.x = 120;
    ball.y = 160;
    ball_x_vel = -(ball_speed);
    ball_y_vel = -(ball_speed);
    Tft.fillScreen();
    Tft.drawRectangle(0, 0, 240, 320, WHITE);
    Tft.drawNumber(player_1_score, 230, 130, 3, WHITE);
    Tft.drawNumber(player_2_score, 230, 169, 3, WHITE);
    Tft.fillRectangle(player_1.x, player_1.y, paddle_length, paddle_width, WHITE);
    Tft.fillRectangle(player_2.x, player_2.y, paddle_length, paddle_width, WHITE);
    Tft.fillRectangle(ball.x, ball.y, ball_size, ball_size, WHITE);
    for (int i = 0; i < 240; i+=10) {
      Tft.drawLine(i, 160, i+5, 160, WHITE); 
    }
  } else {
    
  }
}
void setup()
{
    Tft.TFTinit();  //init TFT library
    Serial.begin(115200);
    drawGame();

}


void loop() {
    // a point object holds x y and z coordinates.
    Point p = ts.getPoint();

    //map the ADC value read to into pixel co-ordinates
    p.x = map(p.x, TS_MINX, TS_MAXX, 0, 240) - (paddle_length / 2);
    p.y = map(p.y, TS_MINY, TS_MAXY, 0, 320);
    
    if (game_state == -1) {
      Tft.drawString("Touch screen to play!", 70, 38, 2, WHITE);
      delay(1000);
      Tft.drawString("Touch screen to play!", 70, 38, 2, BLACK);
      delay(500);
      if (p.z > __PRESSURE) {
        game_state = 0;
        drawGame();
      }
    } else if (game_state == 0) {
      // we have some minimum pressure we consider 'valid'
      // pressure of 0 means no pressing!
      if (p.z > __PRESURE) {
        
        // Make sure paddles don't go out of bounds   
        if (p.x <= 1) {
          p.x = 1;  
        } else if ((p.x + paddle_length) >= 239) {
          p.x = 238 - paddle_length;  
        }
        
        // Move paddle based on what side of screen, left side is p1, right side is p2
        if (p.y <= 160) {  // we are on the left side of the screen
          if (int delta_x = p.x - player_1.x) {  // does the paddle need to be redrawn
            if (delta_x > 0) {            
              Tft.fillRectangle(player_1.x, player_1.y, delta_x, paddle_width, BLACK);
            } else {
              delta_x = abs(delta_x);
              Tft.fillRectangle(player_1.x + paddle_length - delta_x, player_1.y, delta_x, paddle_width, BLACK); 
            }
            player_1.x = p.x;
            Tft.fillRectangle(player_1.x, player_1.y, paddle_length, paddle_width, WHITE);  
          }
        } else {  // we are on the right side of the screen
          if (int delta_x = p.x - player_2.x) {  // does the paddle need to be redrawn
            if (delta_x > 0) {            
              Tft.fillRectangle(player_2.x, player_2.y, delta_x, paddle_width, BLACK);
            } else {
              delta_x = abs(delta_x);
              Tft.fillRectangle(player_2.x + paddle_length - delta_x, player_2.y, delta_x, paddle_width, BLACK); 
            }
            player_2.x = p.x;
            Tft.fillRectangle(player_2.x, player_2.y, paddle_length, paddle_width, WHITE); 
          }
        }
      }
      
      // Redraw the ball
      Tft.fillRectangle(ball.x, ball.y, ball_size, ball_size, BLACK);
      ball.x = ball.x + ball_x_vel;
      ball.y = ball.y + ball_y_vel;
      Tft.fillRectangle(ball.x, ball.y, ball_size, ball_size, WHITE);
      
      // get ball edges
      int ball_left = ball.y;
      int ball_right = ball.y + ball_size;
      int ball_top = ball.x + ball_size;
      int ball_bottom = ball.x;
      
      // get player 1 paddle
      int player_1_right = player_1.y + paddle_width + 1;
      int player_1_top = player_1.x + paddle_length;
      int player_1_bottom = player_1.x - 2;
      
      // get player 2 paddle
      int player_2_left = player_2.y - 2;
      int player_2_top = player_2.x + paddle_length;
      int player_2_bottom = player_2.x - 2;
      
      // Check collisions with top and bottom
      if (ball_bottom <= 2) {
        ball_x_vel = ball_speed;
      } else if (ball_top >= 237) {
        ball_x_vel = -(ball_speed);  
      }
      
      // Check paddle collisions and goal collisions
      if ((ball_left == player_1_right) && (ball_top >= player_1_bottom) && (ball_bottom <= player_1_top)) {
        ball_y_vel = ball_speed;  // left paddle collision
      } else if ((ball_right == player_2_left) && (ball_top >= player_2_bottom) && (ball_bottom <= player_2_top)) {
        ball_y_vel = -(ball_speed);  // right paddle collision
      } else if (((ball_top == player_1_bottom) || (ball_top == player_1_bottom + 1)) && (ball_left <= player_1_right)) {
        ball_x_vel = -(ball_speed);  // left paddle bottom collision
      } else if (((ball_top == player_2_bottom) || (ball_top == player_2_bottom + 1)) && (ball_right >= player_2_left)) {
        ball_x_vel = -(ball_speed);  // right paddle bottom collision
      } else if (((ball_bottom == player_1_top) || (ball_bottom == player_1_top + 1)) && (ball_left <= player_1_right)) {
        ball_x_vel = ball_speed;  // left paddle top collision
      } else if (((ball_bottom == player_2_top) || (ball_bottom == player_2_top + 1)) && (ball_right >= player_2_left)) {
        ball_x_vel = ball_speed;  // right paddle top collision
      } else if ((ball_left) <= 0) {  // player 1 loses
        player_2_score++;
        drawGame();
        delay(1000); 
      } else if ((ball_right) >= 320) {  // player 2 loses  
        player_1_score++;
        drawGame();
        delay(1000);    
      }
      
      //Redraw parts that can get erased
      Tft.drawNumber(player_1_score, 230, 130, 3, WHITE);
      Tft.drawNumber(player_2_score, 230, 169, 3, WHITE);
      for (int i = 0; i < 240; i+=10) {
        Tft.drawLine(i, 160, i+5, 160, WHITE); 
      }
    }
      
      
}

void updatePaddles() {
  
}
