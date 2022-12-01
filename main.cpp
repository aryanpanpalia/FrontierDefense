#include <iostream>
#include <math.h>
#include "FEHLCD.h"
#include "FEHUtility.h"
#include "game.h"

// Defines the window width and height
#define WINDOW_WIDTH 320
#define WINDOW_HEIGHT 240

using namespace FEHIcon;

int highScore1 = 0, highScore2 = 0, highScore3 = 0, totalEnemiesKilled = 0, totalShots = 0, totalDeaths = 0;

/*
    Play Game Function
*/

int SelectDifficulty() {
    // Create buttons
    Icon easyButton, mediumButton, hardButton, extremeButton;
    char easyString[] = "Easy";
    char mediumString[] = "Medium";
    char hardString[] = "Hard";
    char extremeString[] = "Extreme";

    // Setting button width, height, and buffer between them as constants
    int buttonWidth = 100;
    int buttonHeight = 100;
    int buffer = 10;

    // Initial XY positions of the buttons   
    int easyStartX = (WINDOW_WIDTH - (buttonWidth * 2 + buffer)) / 2;
    int easyStartY = (WINDOW_HEIGHT - (buttonHeight * 2 + buffer)) / 2;

    int mediumStartX = easyStartX + buttonWidth + buffer;
    int mediumStartY = easyStartY;

    int hardStartX = easyStartX;
    int hardStartY = easyStartY + buttonHeight + buffer;

    int extremeStartX = easyStartX + buttonWidth + buffer;
    int extremeStartY = easyStartY + buttonHeight + buffer;

    easyButton.SetProperties(easyString, easyStartX, easyStartY, buttonWidth, buttonHeight, GREEN, WHITE);
    mediumButton.SetProperties(mediumString, mediumStartX, mediumStartY, buttonWidth, buttonHeight, YELLOW, WHITE);
    hardButton.SetProperties(hardString, hardStartX, hardStartY, buttonWidth, buttonHeight, ORANGE, WHITE);
    extremeButton.SetProperties(extremeString, extremeStartX, extremeStartY, buttonWidth, buttonHeight, RED, WHITE);

    float x, y;
    while(true) {
        // Clears the screen then draws all the buttons
        LCD.Clear();

        LCD.SetFontColor(GREEN);
        LCD.FillRectangle(easyStartX, easyStartY, buttonWidth, buttonHeight);
        easyButton.Draw();

        LCD.SetFontColor(YELLOW);
        LCD.FillRectangle(mediumStartX, mediumStartY, buttonWidth, buttonHeight);
        mediumButton.Draw();

        LCD.SetFontColor(ORANGE);
        LCD.FillRectangle(hardStartX, hardStartY, buttonWidth, buttonHeight);
        hardButton.Draw();

        LCD.SetFontColor(RED);
        LCD.FillRectangle(extremeStartX, extremeStartY, buttonWidth, buttonHeight);
        extremeButton.Draw();

        // Waits for someone to touch the screen
        while(!LCD.Touch(&x, &y));

        // Waits for someone to release their touch
        while(LCD.Touch(&x, &y));

        if(easyStartX < x && x < easyStartX + buttonWidth && easyStartY < y && y < easyStartY + buttonHeight) {
            return 0;
        } else if(mediumStartX < x && x < mediumStartX + buttonWidth && mediumStartY < y && y < mediumStartY + buttonHeight) {
            return 1;
        } else if(hardStartX < x && x < hardStartX + buttonWidth && hardStartY < y && y < hardStartY + buttonHeight) {
            return 2;
        } else if(extremeStartX < x && x < extremeStartX + buttonWidth && extremeStartY < y && y < extremeStartY + buttonHeight) {
            return 3;
        }
    }
}

void Play(){
    // Prompt for difficulty level
    int difficulty = SelectDifficulty();

    // Stores whether the screen is being pressed
    bool pressed = false;

    // Location of where the screen has been pressed
    float x, y;

    // Game object
    Game game(difficulty);

    // Alias/Reference to game's player object
    Player &player = game.player;

    // Render game
    game.render();

    // Keep running the game loop if the game has not ended
    while(!game.gameOver){
        // On click
        if(LCD.Touch(&x, &y) && !pressed) {
            pressed = true;
            if(!player.pointInPlayer(x, y)){
                Vector2D center = player.getCenter();
                float dx = x - center.x;
                float dy = center.y - y;
                float angle = atan2(dy, dx);
                
                player.shoot(angle);
            }
        } 
        // On Release
        else if(!LCD.Touch(&x, &y) && pressed) {
            pressed = false;
        }

        // Update game
        game.update();

        // Handle player out of bounds
        game.playerOutOfBounds();

        // Render game
        game.render();

        // Handle collisions
        game.handleCollisions();
        
    }

    if (game.score > highScore1) {
        highScore3 = highScore2;
        highScore2 = highScore1;
        highScore1 = game.score;
    } else if (game.score > highScore2) {
        highScore3 = highScore2;
        highScore2 = game.score;
    } else if (game.score > highScore3) {
        highScore3 = game.score;
    }

    totalEnemiesKilled += game.numEnemiesKilled;
    totalShots += game.numShots;
    totalDeaths += 1;
}

/*
    Menu Functions
*/

void Statistics() {
    Icon backButton;
    char backString[] = "Back";

    // Setting button width and height as constants
    int buttonWidth = 100;
    int buttonHeight = 40;

    // Initial XY position of back button    
    int buttonStartX = (WINDOW_WIDTH - buttonWidth) / 2;
    int buttonStartY = 175;

    backButton.SetProperties(backString, buttonStartX, buttonStartY, buttonWidth, buttonHeight, RED, WHITE);
    
    LCD.Clear();

    LCD.WriteAt("High Score 1:", 10, 10);       LCD.WriteAt(highScore1, WINDOW_WIDTH-100, 10);
    LCD.WriteAt("High Score 2:", 10, 30);       LCD.WriteAt(highScore2, WINDOW_WIDTH-100, 30);
    LCD.WriteAt("High Score 3:", 10, 50);       LCD.WriteAt(highScore3, WINDOW_WIDTH-100, 50);
    LCD.WriteAt("# Enemies Killed:", 10, 70);   LCD.WriteAt(totalEnemiesKilled, WINDOW_WIDTH-100, 70);
    LCD.WriteAt("# Shots fired:", 10, 90);      LCD.WriteAt(totalShots, WINDOW_WIDTH-100, 90);
    LCD.WriteAt("# Deaths:", 10, 110);          LCD.WriteAt(totalDeaths, WINDOW_WIDTH-100, 110);
    LCD.WriteAt("% Accuracy:", 10, 130);        LCD.WriteAt((float)totalEnemiesKilled/totalShots, WINDOW_WIDTH-100, 130);
    LCD.WriteAt("", 10, 150);

    LCD.SetFontColor(RED);
    LCD.FillRectangle(buttonStartX, buttonStartY, buttonWidth, buttonHeight);
    backButton.Draw();

    float x, y;
    while(true){
        // Waits for someone to touch the screen
        while(!LCD.Touch(&x, &y));

        // Waits for someone to release their touch
        while(LCD.Touch(&x, &y));

        if(x >= buttonStartX && x <= buttonStartX + buttonWidth && y >= buttonStartY && y <= buttonStartY + buttonHeight){
            return;
        }
    }
}

void Instructions() {
    Icon backButton;
    char backString[] = "Back";

    // Setting button width and height as constants
    int buttonWidth = 100;
    int buttonHeight = 40;

    // Initial XY position of back button    
    int buttonStartX = (WINDOW_WIDTH - buttonWidth) / 2;
    int buttonStartY = 175;

    backButton.SetProperties(backString, buttonStartX, buttonStartY, buttonWidth, buttonHeight, RED, WHITE);
    
    LCD.Clear();

    LCD.WriteAt("To play, click anywhere", 10, 10);
    LCD.WriteAt("outside the player. Shoot", 10, 30);
    LCD.WriteAt("at enemies to kill them", 10, 50);
    LCD.WriteAt("and get ammo while moving", 10, 70);
    LCD.WriteAt("yourself in the opposite", 10, 90);
    LCD.WriteAt("direction. Don't run into", 10, 110);
    LCD.WriteAt("an enemy or off the", 10, 130);
    LCD.WriteAt("screen or you'll lose!", 10, 150);

    LCD.SetFontColor(RED);
    LCD.FillRectangle(buttonStartX, buttonStartY, buttonWidth, buttonHeight);
    backButton.Draw();

    float x, y;
    while(true){
        // Waits for someone to touch the screen
        while(!LCD.Touch(&x, &y));

        // Waits for someone to release their touch
        while(LCD.Touch(&x, &y));

        if(x >= buttonStartX && x <= buttonStartX + buttonWidth && y >= buttonStartY && y <= buttonStartY + buttonHeight){
            return;
        }
    }
}

void Credits() {
    Icon backButton;
    char backString[] = "Back";

    // Setting button width and height as constants
    int buttonWidth = 100;
    int buttonHeight = 40;

    // Initial XY position of back button    
    int buttonStartX = (WINDOW_WIDTH - buttonWidth) / 2;
    int buttonStartY = 175;

    backButton.SetProperties(backString, buttonStartX, buttonStartY, buttonWidth, buttonHeight, RED, WHITE);
    
    LCD.Clear();

    LCD.WriteAt("Game Title", 10, 10);
    LCD.WriteAt("Created By:", 10, 30);
    LCD.WriteAt("Thomas Banko", 10, 50);
    LCD.WriteAt("and", 10, 70);
    LCD.WriteAt("Aryan Panpalia", 10, 90);
    LCD.WriteAt("Using:", 10, 110);
    LCD.WriteAt("Proteus Simulator", 10, 130);
    LCD.WriteAt("Libraries from OSU FEH", 10, 150);

    LCD.SetFontColor(RED);
    LCD.FillRectangle(buttonStartX, buttonStartY, buttonWidth, buttonHeight);
    backButton.Draw();

    float x, y;
    while(true){
        // Waits for someone to touch the screen
        while(!LCD.Touch(&x, &y));

        // Waits for someone to release their touch
        while(LCD.Touch(&x, &y));

        if(x >= buttonStartX && x <= buttonStartX + buttonWidth && y >= buttonStartY && y <= buttonStartY + buttonHeight){
            return;
        }
    }
}

void Menu() {
    // Icons for all the menu options
    Icon playButton;
    char playString[] = "Play Game";
    
    Icon viewStatistics;
    char statsString[] = "View Statistics";

    Icon viewInstructions;
    char instructionsString[] = "View Instructions";

    Icon viewCredits;
    char creditsString[] = "View Credits";

    // Setting button width and height as constants
    int buttonWidth = 220;
    int buttonHeight = 40;
    
    // All buttons start at the same X position
    int buttonStartX = (WINDOW_WIDTH - buttonWidth) / 2;

    // Initial Y positions for all the buttons
    int playStartY = 25, statsStartY = 75, instructionsStartY = 125, creditsStartY = 175;

    // Sets properties of all the buttons
    playButton.SetProperties(playString, buttonStartX, playStartY, buttonWidth, buttonHeight, GREEN, WHITE);
    viewStatistics.SetProperties(statsString, buttonStartX, statsStartY, buttonWidth, buttonHeight, BLUE, WHITE);
    viewInstructions.SetProperties(instructionsString, buttonStartX, instructionsStartY, buttonWidth, buttonHeight, WHITE, BLACK);
    viewCredits.SetProperties(creditsString, buttonStartX, creditsStartY, buttonWidth, buttonHeight, RED, WHITE);

    float x, y;
    while(true){
        // Clears the screen then draws all the buttons
        LCD.Clear();

        LCD.SetFontColor(GREEN);
        LCD.FillRectangle(buttonStartX, playStartY, buttonWidth, buttonHeight);
        playButton.Draw();

        LCD.SetFontColor(BLUE);
        LCD.FillRectangle(buttonStartX, statsStartY, buttonWidth, buttonHeight);
        viewStatistics.Draw();

        LCD.SetFontColor(WHITE);
        LCD.FillRectangle(buttonStartX, instructionsStartY, buttonWidth, buttonHeight);
        viewInstructions.Draw();

        LCD.SetFontColor(RED);
        LCD.FillRectangle(buttonStartX, creditsStartY, buttonWidth, buttonHeight);
        viewCredits.Draw();

        // Waits for someone to touch the screen
        while(!LCD.Touch(&x, &y));

        // Waits for someone to release their touch
        while(LCD.Touch(&x, &y));

        // Depending on where the touch was at the end, do the following actions
        if(x >= buttonStartX && x <= buttonStartX + buttonWidth){
            if(y >= playStartY && y <= playStartY + buttonHeight){
                Play();
                Sleep(1.0);
            } else if(y >= statsStartY && y <= statsStartY + buttonHeight){
                Statistics();
            } else if(y >= instructionsStartY && y <= instructionsStartY + buttonHeight){
                Instructions();
            } else if(y >= creditsStartY && y <= creditsStartY + buttonHeight){
                Credits();
            }
        }
    }
}

/* Entry point to the application */
int main() {
    // Draws the menu
    Menu();

    return 0;
}