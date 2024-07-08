#include <iostream>
//#include <raylib.h>


using namespace std;

Color Green = Color{28, 185, 154, 255};
Color Dark_Green = Color{20, 160, 133, 255};
Color Light_Green = Color{129, 204, 184, 255};
Color Yellow = Color{243, 213, 91, 255};

int player_score = 0, cpu_score = 0;

class Ball // ball object
{
    public:
    // attributes   
    float x, y;
    int speed_x, speed_y;
    int radius;

    void Draw()
    {
        DrawCircle (x, y, radius, ORANGE); // the ball in the middle
    }

    // to make the ball move
    void Update ()
    {
        x += speed_x;
        y += speed_y;       // increases the x and y coordinates of the ball by 7 pixels everytime the game loop is run

        if (y + radius >= GetScreenHeight() || y - radius <= 0) //GetScreenHeight() is Raylib library function that gets the height of the window. by doing this we do not need to make the screen_height variable global
        {
            speed_y *= -1;
        }
        if (x + radius >= GetScreenWidth() ) // Cpu wins
        {
            cpu_score ++;
            ResetBall();
        }
        
        if (x - radius  <= 0) // player wins
        {
            player_score ++;
            ResetBall();
        }

    }

    void ResetBall()
    {
        x = GetScreenWidth() / 2;
        y = GetScreenHeight() / 2;

        int speed_choices[2] = {1, -1};
        speed_x *= speed_choices[GetRandomValue(0, 1)];
        speed_y *= speed_choices[GetRandomValue(0, 1)];
    }
};

class Paddle 
{
    protected: // no other part of the program will know about the code enclosed here
    // made a method
    void LimitMovement()
    {
        if (y <= 0)
        {
            y = 0;
        }

        if (y + height >= GetScreenHeight())
        {
            y = GetScreenHeight() - height;
        }
    }

    public:
    float x, y;
    float width, height;
    int speed;

    void Draw ()
    {
        DrawRectangleRounded(Rectangle{x, y, width, height}, 0.8, 0, LIGHTGRAY); // right paddle
    }

    void Update ()
    {
        if (IsKeyDown(KEY_UP))
        {
            y = y - speed;      // as we want to move the paddle upwards. and since the origin on the point is at the top-left, we are subtracting (if we want to move upwards, we need to decrease the y-coordonates)

        }
        if (IsKeyDown(KEY_DOWN))
        {
            y = y + speed;
        }

        LimitMovement();
    }
};

class CpuPaddle: public Paddle // this means the CpuPaddle class is the child of the Paddle class. it'll inherit all the attributes of the Paddle class
{
    public:
    void Update(int ball_y)
    {
        /*if the center y-position of the ball is above the center y-position of the paddle,
        the paddle will move upwards, otherwise it'll move down*/
        if (y + height / 2 > ball_y)
        {
            y = y - speed;
        }
        if (y + height / 2 <= ball_y)
        {
            y = y + speed;
        }
        LimitMovement();
    }
};

Ball ball; // in instance of ball
Paddle player; // object of Paddle
CpuPaddle cpu; // instance/object of the CpuPaddle class

int main()
{
    cout << "Starting the game" << endl;
    const int screen_width = 1280, screen_height = 600; // height was initially set as 800

    InitWindow (screen_width, screen_height, "My Pong game :)");         // initializes the window
    SetTargetFPS (60);  //how fast the game should run. if not defined, the game will run as fast as it can. The followingg gameloop will run 60 times per second.

    // initializing the ball instance
    ball.radius = 20;
    ball.x = screen_width / 2;
    ball.y = screen_height / 2;
    ball.speed_x = 7;
    ball.speed_y = 7;

    // initializing the paddle object
    player.width = 25;
    player.height = 120;
    player.x = screen_width - player.width - 10;
    player.y = screen_height / 2 - player.height / 2;
    player.speed = 6;

    // initializing the cpuPaddle object
    cpu.height = 120;
    cpu.width = 25;
    cpu.x = 10;
    cpu.y = screen_height / 2 - cpu.height / 2;
    cpu.speed = 6;

    // game loop
    while (WindowShouldClose() == false)        //WindowShouldClose function checks if the `esc` key is pressed or the `window close` icon is pressed. when it turns true, the game ends and the window closes.
    {
        BeginDrawing (); // creates a blank canvas for us to draw the game objects

        //Update (updating the objects or the position of the objects)
        ball.Update();
        player.Update();
        cpu.Update(ball.y);

        //Checking for collisions
        if (CheckCollisionCircleRec(Vector2{ball.x, ball.y}, ball.radius, Rectangle{player.x, player.y, player.width, player.height}))
        {
            ball.speed_x *= -1;
        }

        if (CheckCollisionCircleRec(Vector2{ball.x, ball.y}, ball.radius, Rectangle{cpu.x, cpu.y, cpu.width, cpu.height}))
        {
            ball.speed_x *= -1;
        }

        //Drawing 
        ClearBackground(Dark_Green);         //to clear the traces
        DrawRectangle(screen_width / 2, 0, screen_width/2, screen_height, Green); // the paddles 
        DrawCircle (screen_width / 2, screen_height / 2, 150, Light_Green); // the circle (ball) in the screen
        DrawLine (screen_width / 2, 0, screen_width / 2, screen_height, WHITE); // the line in the middle
        
        ball.Draw ();
        cpu.Draw();
        //DrawRectangle (10, screen_height / 2 - 60, 25, 120, WHITE); // left paddle    increase in y-axis goes downwards
        player.Draw();
        DrawText(TextFormat("%i", cpu_score), screen_width / 4 - 20, 20, 80, WHITE);
        DrawText(TextFormat("%i", player_score), 3 * screen_width / 4 - 20, 20, 80, WHITE);
        
        EndDrawing (); // ends the canvas drawing
    }

    CloseWindow();
    return 0;
}