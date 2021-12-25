#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

#include "RenderWindow.hpp"
#include "Entity.hpp"
#include "Board.hpp"
#include "Text.hpp"
#include "AI.hpp"

using namespace std;

int cursorColumn(SDL_MouseButtonEvent b);                                                                 // returns board column from mouse position
void createPiece(Entity *pieces[], SDL_Texture *tex, int row, int column, int &num_pieces, int turns);    // adds new piece at top of board
void createPieceAbs(Entity *pieces[], SDL_Texture *tex, int row, int column, int &num_pieces, int turns); // adds new piece at given row
void renderPieces(RenderWindow window, Entity *pieces[], int num_pieces, Mix_Chunk *fall_sound);          // renders pieces from object list
void highlightWinningPieces(RenderWindow window, Board board, Mix_Chunk *fall_sound);                     // changes colour of winning pieces to green
void winScreen(RenderWindow window, int winner);                                                          // displays winner and exits to main menu
bool inRect(SDL_Rect rect, int x, int y);                                                                 // checks if x and y coordinates lie inside given rectangle
void highlightColumn(RenderWindow Window, Board board, int x, int y);                                     // highlights column where mouse hovers
void mainMenuLoop(RenderWindow window);                                                                   // game loop for main menu
bool singlePlayerLoop(RenderWindow window);                                                               // game loop for single player mode
bool twoPlayerLoop(RenderWindow window);                                                                  // game loop for two player mode

const int SCREEN_WIDTH = 421;
const int SCREEN_HEIGHT = 480;

const int SQUARE_SIZE = 60;
const int ROWS = 6;
const int COLUMNS = 7;

const SDL_Color White = {255, 255, 255};
const SDL_Color Red = {155, 0, 0};
const SDL_Color Blue = {0, 163, 185};

int main(int argc, char *args[])
{
    if (SDL_Init(SDL_INIT_EVERYTHING) > 0)
        cout
            << "SDL INIT FAILED" << endl;

    if (!(IMG_Init(IMG_INIT_PNG)))
        cout << "IMG INIT FAILED" << endl;

    if (TTF_Init() == -1)
        cout << "TTF INIT FAILED" << endl;

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
        cout << "Audio has failed" << endl;

    RenderWindow window("Connect 4", SCREEN_WIDTH, SCREEN_HEIGHT);

    mainMenuLoop(window);

    return 0;
}

int cursorColumn(SDL_MouseButtonEvent b)
{
    int mouse_x, mouse_y;
    SDL_GetMouseState(&mouse_x, &mouse_y);
    int column = mouse_x / SQUARE_SIZE;

    return column;
}

void createPiece(Entity *pieces[], SDL_Texture *tex, int row, int column, int &num_pieces, int turns)
{
    pieces[turns / 2] = new Entity(column * SQUARE_SIZE, 0, SQUARE_SIZE, SQUARE_SIZE, tex, ((row + 1) * SQUARE_SIZE + SQUARE_SIZE));
    num_pieces++;
}

void createPieceAbs(Entity *pieces[], SDL_Texture *tex, int row, int column, int &num_pieces)
{
    pieces[num_pieces] = new Entity(column * SQUARE_SIZE, row * SQUARE_SIZE + SQUARE_SIZE, SQUARE_SIZE, SQUARE_SIZE, tex, -1);
    num_pieces++;
}

void renderPieces(RenderWindow window, Entity *pieces[], int num_pieces, Mix_Chunk *fall_sound)
{
    for (int i = 0; i < num_pieces; i++)
    {
        window.render(*pieces[i]);
        pieces[i]->fall(fall_sound);
    }
}

void highlightWinningPieces(RenderWindow window, Board board, Mix_Chunk *fall_sound)
{
    int winner = board.checkWin();

    SDL_Texture *green_tex = window.loadTexture("res/gfx/green.png");
    Entity *green_pieces[4];
    int num_green_pieces = 0;

    if (board.direction == 0)
    {
        for (int i = 0; i < 4; i++)
        {
            createPieceAbs(green_pieces, green_tex, board.winning_row, board.winning_column + i, num_green_pieces);
        }
    }
    else if (board.direction == 1)
    {
        for (int i = 0; i < 4; i++)
        {
            createPieceAbs(green_pieces, green_tex, board.winning_row + i, board.winning_column, num_green_pieces);
        }
    }
    else if (board.direction == 2)
    {
        for (int i = 0; i < 4; i++)
        {
            createPieceAbs(green_pieces, green_tex, board.winning_row + i, board.winning_column + i, num_green_pieces);
        }
    }
    else if (board.direction == 3)
    {
        for (int i = 0; i < 4; i++)
        {
            createPieceAbs(green_pieces, green_tex, board.winning_row - i, board.winning_column - i, num_green_pieces);
        }
    }
    else if (board.direction == 4)
    {
        for (int i = 0; i < 4; i++)
        {
            createPieceAbs(green_pieces, green_tex, board.winning_row + i, board.winning_column - i, num_green_pieces);
        }
    }
    else if (board.direction == 5)
    {
        for (int i = 0; i < 4; i++)
        {
            createPieceAbs(green_pieces, green_tex, board.winning_row - i, board.winning_column + i, num_green_pieces);
        }
    }

    renderPieces(window, green_pieces, num_green_pieces, fall_sound);
}

void winScreen(RenderWindow window, int winner)
{
    Text p1wins(window, "fonts/LemonMilkLight.otf", 25, "Player 1 Wins!", White);
    Text p2wins(window, "fonts/LemonMilkLight.otf", 25, "Player 2 Wins!", White);
    Text draw_prompt(window, "fonts/LemonMilkLight.otf", 25, "It's a draw!", White);

    window.clear();
    if (winner == 1)
        p1wins.display(window, SCREEN_WIDTH / 2 - p1wins.getTextRect().w / 2, SCREEN_HEIGHT / 2 - p1wins.getTextRect().h / 2);
    else if (winner == 2)
        p2wins.display(window, SCREEN_WIDTH / 2 - p2wins.getTextRect().w / 2, SCREEN_HEIGHT / 2 - p2wins.getTextRect().h / 2);
    else
        draw_prompt.display(window, SCREEN_WIDTH / 2 - draw_prompt.getTextRect().w / 2, SCREEN_HEIGHT / 2 - draw_prompt.getTextRect().h / 2);

    window.display();
    SDL_Delay(2000);
}

bool inRect(SDL_Rect rect, int x, int y)
{
    if (x > rect.x && x < rect.x + rect.w && y > rect.y && y < rect.y + rect.h)
        return true;
    return false;
}

void highlightColumn(RenderWindow window, Board board, int x, int y)
{
    if (y > SQUARE_SIZE && y < 420)
    {
        int column = x / SQUARE_SIZE;
        int row = board.topMostRow(column);
        if (row >= 0)
            window.drawRect(x / SQUARE_SIZE * SQUARE_SIZE, SQUARE_SIZE, SQUARE_SIZE, row * SQUARE_SIZE + SQUARE_SIZE);
    }
}

bool singlePlayerLoop(RenderWindow window)
{
    Board board;
    AI ai;

    Mix_Chunk *fall_sound = Mix_LoadWAV("res/sounds/fall_sound.wav");

    int player = 1;
    int turns = 0;
    int start_ticks = 0;
    bool game_over = false;
    int winner = 0;

    // loading textures
    SDL_Texture *board_tex = window.loadTexture("res/gfx/board.png");
    SDL_Texture *red_tex = window.loadTexture("res/gfx/red.png");
    SDL_Texture *blue_tex = window.loadTexture("res/gfx/blue.png");

    Text p1turn(window, "fonts/LemonMilkLight.otf", 25, "Player 1's turn", White);
    Text p2turn(window, "fonts/LemonMilkLight.otf", 25, "Player 2's turn", White);
    Text game_over_text(window, "fonts/LemonMilkLight.otf", 25, "Game Over!", White);
    Text main_menu_text(window, "fonts/LemonMilkLight.otf", 13, "Main Menu", White);

    Entity board_png(0, SQUARE_SIZE, SCREEN_WIDTH, 360, board_tex, -1);

    Entity *red_pieces[21];
    int num_red_pieces = 0;
    Entity *blue_pieces[21];
    int num_blue_pieces = 0;

    bool run = true;
    while (run)
    {
        int x, y;
        SDL_GetMouseState(&x, &y);

        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                return false;
            }
            else if (event.type == SDL_MOUSEBUTTONDOWN)
            {
                int column = cursorColumn(event.button);
                int row = board.topMostRow(column);
                if (inRect(main_menu_text.getTextRect(), x, y))
                {
                    run = false;
                    break;
                }
                else if (board.onBoard(row, column) && !game_over)
                {
                    if (player == 1)
                    {
                        board.changePiece(row, column, player);
                        createPiece(red_pieces, red_tex, row, column, num_red_pieces, turns);

                        turns++;
                        player = turns % 2 + 1;
                        start_ticks = SDL_GetTicks();
                    }
                }
            }
            else if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)
            {
                run = false;
                break;
            }
        }

        if (!game_over)
        {
            winner = board.checkWin();
            if (winner > 0)
            {
                start_ticks = SDL_GetTicks();
                game_over = true;
            }
        }

        if (player == 2 && !game_over)
        {
            int end_ticks = SDL_GetTicks();
            if (end_ticks - start_ticks > 1000)
            {
                int column = ai.pickBestCol(board, player, turns);

                int row = board.topMostRow(column);

                if (board.onBoard(row, column))
                {
                    board.changePiece(row, column, player);
                    createPiece(blue_pieces, blue_tex, row, column, num_blue_pieces, turns);
                    turns++;
                    player = turns % 2 + 1;
                }
            }
        }

        window.clear();
        highlightColumn(window, board, x, y);
        window.render(board_png);

        renderPieces(window, red_pieces, num_red_pieces, fall_sound);
        renderPieces(window, blue_pieces, num_blue_pieces, fall_sound);

        if (player == 1 && !game_over)
            p1turn.display(window, SCREEN_WIDTH / 2 - p1turn.getTextRect().w / 2, 430);
        else if (player == 2 && !game_over)
            p2turn.display(window, SCREEN_WIDTH / 2 - p2turn.getTextRect().w / 2, 430);
        else
            game_over_text.display(window, SCREEN_WIDTH / 2 - game_over_text.getTextRect().w / 2, 430);

        if (game_over)
        {
            int end_ticks = SDL_GetTicks();
            if (end_ticks - start_ticks > 1500)
            {
                highlightWinningPieces(window, board, fall_sound);
            }
            if (end_ticks - start_ticks > 3500)
            {
                winScreen(window, winner);
                run = false;
            }
        }

        main_menu_text.display(window, 0, 0);
        if (inRect(main_menu_text.getTextRect(), x, y))
        {
            Text main_menu_text(window, "fonts/LemonMilkLight.otf", 13, "Main Menu", Red);
            main_menu_text.display(window, 0, 0);
        }

        SDL_Delay(5);

        window.display();
    }

    Mix_ResumeMusic();
    Mix_FreeChunk(fall_sound);
    return true;
}

bool twoPlayerLoop(RenderWindow window)
{
    Board board;

    Mix_Chunk *fall_sound = Mix_LoadWAV("res/sounds/fall_sound.wav");

    int player = 1;
    int turns = 0;
    int start_ticks = 0;
    bool game_over = false;
    int winner = 0;

    // loading textures
    SDL_Texture *board_tex = window.loadTexture("res/gfx/board.png");
    SDL_Texture *red_tex = window.loadTexture("res/gfx/red.png");
    SDL_Texture *blue_tex = window.loadTexture("res/gfx/blue.png");

    Text p1turn(window, "fonts/LemonMilkLight.otf", 25, "Player 1's turn", White);
    Text p2turn(window, "fonts/LemonMilkLight.otf", 25, "Player 2's turn", White);
    Text game_over_text(window, "fonts/LemonMilkLight.otf", 25, "Game Over!", White);
    Text main_menu_text(window, "fonts/LemonMilkLight.otf", 13, "Main Menu", White);

    Entity board_png(0, SQUARE_SIZE, SCREEN_WIDTH, 360, board_tex, -1);

    Entity *red_pieces[21];
    int num_red_pieces = 0;
    Entity *blue_pieces[21];
    int num_blue_pieces = 0;

    bool run = true;
    while (run)
    {
        int x, y;
        SDL_GetMouseState(&x, &y);

        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                return false;
            }
            else if (event.type == SDL_MOUSEBUTTONDOWN)
            {
                int column = cursorColumn(event.button);
                int row = board.topMostRow(column);
                if (inRect(main_menu_text.getTextRect(), x, y))
                {
                    run = false;
                    break;
                }
                else if (board.onBoard(row, column) && !(game_over))
                {
                    board.changePiece(row, column, player);

                    if (player == 1)
                        createPiece(red_pieces, red_tex, row, column, num_red_pieces, turns);
                    else
                        createPiece(blue_pieces, blue_tex, row, column, num_blue_pieces, turns);

                    turns++;
                    player = turns % 2 + 1;
                }

                winner = board.checkWin();
                if (winner > 0)
                {
                    start_ticks = SDL_GetTicks();
                    game_over = true;
                }
            }
            else if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)
            {
                run = false;
                break;
            }
        }

        window.clear();
        highlightColumn(window, board, x, y);

        window.render(board_png);

        renderPieces(window, red_pieces, num_red_pieces, fall_sound);
        renderPieces(window, blue_pieces, num_blue_pieces, fall_sound);

        if (player == 1 && !game_over)
            p1turn.display(window, SCREEN_WIDTH / 2 - p1turn.getTextRect().w / 2, 430);
        else if (player == 2 && !game_over)
            p2turn.display(window, SCREEN_WIDTH / 2 - p2turn.getTextRect().w / 2, 430);
        else
            game_over_text.display(window, SCREEN_WIDTH / 2 - game_over_text.getTextRect().w / 2, 430);

        if (game_over)
        {
            int end_ticks = SDL_GetTicks();
            if (end_ticks - start_ticks > 1500)
            {
                highlightWinningPieces(window, board, fall_sound);
            }
            if (end_ticks - start_ticks > 3500)
            {
                winScreen(window, winner);
                run = false;
            }
        }

        main_menu_text.display(window, 0, 0);
        if (inRect(main_menu_text.getTextRect(), x, y))
        {
            Text main_menu_text(window, "fonts/LemonMilkLight.otf", 13, "Main Menu", Red);
            main_menu_text.display(window, 0, 0);
        }

        SDL_Delay(5);

        window.display();
    }
    Mix_ResumeMusic();
    Mix_FreeChunk(fall_sound);
    return true;
}

void mainMenuLoop(RenderWindow window)
{
    Mix_Music *background_music = Mix_LoadMUS("res/sounds/background_music.mp3");
    bool paused = false;

    Text title(window, "fonts/LemonMilkLight.otf", 40, "Connect 4", White);
    Text single_player_text(window, "fonts/LemonMilkLight.otf", 20, "Single Player", White);
    Text two_player_text(window, "fonts/LemonMilkLight.otf", 20, "Two Player", White);
    Text mute_text(window, "fonts/LemonMilkLight.otf", 15, "Mute", White);
    Text unmute_text(window, "fonts/LemonMilkLight.otf", 15, "Unmute", White);
    Mix_PlayMusic(background_music, -1);

    bool run = true;
    while (run)
    {
        int x, y;
        SDL_GetMouseState(&x, &y);

        SDL_Rect single_player_button = single_player_text.getTextRect();
        SDL_Rect two_player_button = two_player_text.getTextRect();
        SDL_Rect mute_button = mute_text.getTextRect();
        SDL_Rect unmute_button = unmute_text.getTextRect();

        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                run = false;
                break;
            }
            else if (event.type == SDL_MOUSEBUTTONDOWN)
            {
                if (inRect(single_player_button, x, y))
                {
                    Mix_PauseMusic();
                    if (!singlePlayerLoop(window))
                    {
                        run = false;
                        break;
                    }
                    paused = false;
                }
                else if (inRect(two_player_button, x, y))
                {
                    Mix_PauseMusic();
                    if (!twoPlayerLoop(window))
                    {
                        run = false;
                        break;
                    }
                    paused = false;
                }
                else if (inRect(mute_button, x, y) && !paused)
                {
                    Mix_PauseMusic();
                    paused = true;
                }
                else if (inRect(unmute_button, x, y) && paused)
                {
                    Mix_ResumeMusic();
                    paused = false;
                }
            }
        }

        window.clear();
        title.display(window, SCREEN_WIDTH / 2 - title.getTextRect().w / 2, SCREEN_HEIGHT / 2 - SCREEN_HEIGHT / 4);
        single_player_text.display(window, SCREEN_WIDTH / 2 - single_player_button.w / 2, SCREEN_HEIGHT / 2);
        two_player_text.display(window, SCREEN_WIDTH / 2 - two_player_button.w / 2, SCREEN_HEIGHT / 2 + SCREEN_HEIGHT / 8);
        if (!paused)
            mute_text.display(window, 0, SCREEN_HEIGHT - mute_text.getTextRect().h);
        else
            unmute_text.display(window, 0, SCREEN_HEIGHT - unmute_text.getTextRect().h);

        // highlighting text

        if (inRect(single_player_button, x, y))
        {
            Text single_player_text(window, "fonts/LemonMilkLight.otf", 20, "Single Player", Red);
            single_player_text.display(window, SCREEN_WIDTH / 2 - single_player_button.w / 2, SCREEN_HEIGHT / 2);
        }

        if (inRect(two_player_button, x, y))
        {
            Text two_player_text(window, "fonts/LemonMilkLight.otf", 20, "Two Player", Blue);
            two_player_text.display(window, SCREEN_WIDTH / 2 - two_player_button.w / 2, SCREEN_HEIGHT / 2 + SCREEN_HEIGHT / 8);
        }

        if (inRect(mute_button, x, y) && !paused)
        {
            Text mute_text(window, "fonts/LemonMilkLight.otf", 15, "Mute", Red);
            mute_text.display(window, 0, SCREEN_HEIGHT - mute_button.h);
        }
        if (inRect(unmute_button, x, y) && paused)
        {
            Text unmute_text(window, "fonts/LemonMilkLight.otf", 15, "Unmute", Blue);
            unmute_text.display(window, 0, SCREEN_HEIGHT - unmute_button.h);
        }

        window.display();
    }

    Mix_FreeMusic(background_music);

    window.cleanUp();
    TTF_Quit();
    IMG_Quit();
    Mix_Quit();
    SDL_Quit();
}