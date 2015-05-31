/*
 * This is my c++ Snake
 * - by petriczech
 */

#include <SDL2/SDL.h>
#include <iostream>
#include <ctime>

const int WIDTH = 25;
const int HEIGHT = 15;
const int SIZE = 50;

const int SCREEN_WIDTH = 1 + WIDTH * (SIZE+1);
const int SCREEN_HEIGHT = 1 +HEIGHT * (SIZE+1);

const int n = 3;
int direction = 1;
bool paused = false;
SDL_Rect food = {0, 0, SIZE, SIZE};

SDL_Window* Window = NULL;
SDL_Renderer* Renderer = NULL;

int go[n][2];

bool init()
{
	bool success = true;

	if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
		std::cout << "SDL couldn't initialize! SDL Error: " << SDL_GetError() << std::endl;
		success = false;
	}
	else
	{
		Window = SDL_CreateWindow( "Snake | by petriczech", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
		if( Window == NULL )
		{
			std::cout << "Window couldn't be created! SDL Error: " << SDL_GetError() << std::endl;
			success = false;
		}
		else
		{
			Renderer = SDL_CreateRenderer( Window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );
			if( Renderer == NULL )
			{
				std::cout << "Renderer couldn't be created! SDL Error: " << SDL_GetError() << std::endl;
				success = false;
			}
		}
	}

	return success;
}

void close()
{
	SDL_DestroyRenderer( Renderer );
	SDL_DestroyWindow( Window );

	Window = NULL;
	Renderer = NULL;

	SDL_Quit();

	std::cout << "Exited..." << std::endl;
}

void drawLandMarks()
{
	SDL_SetRenderDrawColor( Renderer, 80, 80, 80, 255 );
	for( int x = SIZE+1; x < SCREEN_WIDTH-(SIZE+1); x += SIZE+1 )
	{
		for( int y = SIZE+1; y < SCREEN_HEIGHT-(SIZE+1); y += SIZE+1 )
		{
			SDL_RenderDrawPoint( Renderer, x, y );
		}
	}
}

void getFood()
{
	food.x = 1 + (rand()%WIDTH) * (SIZE+1);
	food.y = 1 + (rand()%HEIGHT) * (SIZE+1);
}

int main( int, char**)
{
	if( !init() )
	{
		std::cout << "Failed to initialize!" << std::endl;
	}
	else {
		srand(time(NULL));
		
		SDL_Rect snake[n];

		for( int i = 0; i < n; i++ )
		{
			snake[i].w = SIZE;
			snake[i].h = SIZE;
			snake[i].x = 1 + (SIZE+1)*i;
			snake[i].y = 1;
		}

		getFood();

		SDL_Event e;
		bool quit = false;

		while ( !quit ) {
			while ( SDL_PollEvent(&e) )
			{
				if ( e.type == SDL_QUIT )
				{
					quit = true;
				}
				else if( e.type == SDL_KEYDOWN )
				{
					switch( e.key.keysym.sym )
					{
						case SDLK_ESCAPE:
							quit = true;
							break;
						case SDLK_RIGHT:
							direction = 1;
							break;
						case SDLK_UP:
							direction = 2;
							break;
						case SDLK_LEFT:
							direction = 3;
							break;
						case SDLK_DOWN:
							direction = 4;
							break;
						case SDLK_p:
							paused = !paused;
							break;
					}
				}
			}
			
			if( !paused )
			{
				SDL_SetRenderDrawColor( Renderer, 50, 50, 50, 255 );
				SDL_RenderClear( Renderer );
				
				drawLandMarks();
				
				
				for ( int i = 0; i < n-1; i++ )
				{
					snake[i].x = snake[i+1].x;
					snake[i].y = snake[i+1].y;
				}
				
				switch( direction )
				{
					case 1:
						if( snake[n-1].x + SIZE+1 == SCREEN_WIDTH)
						{
							snake[n-1].x = 1;
						}
						else
						{
							snake[n-1].x += SIZE+1;
						}
						break;
					case 2:
						if( snake[n-1].y == 1 )
						{
							snake[n-1].y = SCREEN_HEIGHT - SIZE - 1;
						}
						else
						{
							snake[n-1].y -= SIZE+1;
						}
						break;
					case 3:
						if( snake[n-1].x == 1 )
						{
							snake[n-1].x = SCREEN_WIDTH - SIZE - 1;
						}
						else
						{
							snake[n-1].x -= SIZE+1;
						}
						break;
					case 4:
						if( snake[n-1].y + SIZE+1 == SCREEN_HEIGHT )
						{
							snake[n-1].y = 1;
						}
						else
						{
							snake[n-1].y += SIZE+1;
						}
						break;
				}

				SDL_SetRenderDrawColor( Renderer, 153, 255, 51, 255 );
				SDL_RenderFillRect( Renderer, &food );

				SDL_SetRenderDrawColor( Renderer, 0, 102, 153, 255 );
				for( int i = 0; i < n-1; i++ )
				{
					SDL_RenderFillRect( Renderer, &snake[i] );
				}
				
				SDL_SetRenderDrawColor( Renderer, 200, 0, 0, 255 );
				SDL_RenderFillRect( Renderer, &snake[n-1] );

				SDL_RenderPresent( Renderer );
				SDL_Delay(120);
			}
		}
	}

	close();
	return 0;
}
