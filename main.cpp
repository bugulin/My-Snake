/*
 * This is my c++ Snake
 * - by petriczech
 */

#include <SDL2/SDL.h>
#include <iostream>
#include <ctime>

const int WIDTH = 25;
const int HEIGHT = 15;
const int SIZE = 60;

const int SCREEN_WIDTH = 1 + WIDTH * (SIZE+1);
const int SCREEN_HEIGHT = 1 +HEIGHT * (SIZE+1);

int n = 3;
int max_length = WIDTH * HEIGHT - 5;
int eaten = 0;
int direction = 1;
bool paused = false;
SDL_Rect food = {0, 0, SIZE, SIZE};

SDL_Window* Window = NULL;
SDL_Renderer* Renderer = NULL;

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
		
		SDL_Rect snake[max_length];

		for( int i = 0; i < n; i++ )
		{
			snake[i].w = SIZE;
			snake[i].h = SIZE;
			snake[i].x = 1 + (SIZE+1)*(n-i);
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
				
				if( eaten > 0 ) {
					eaten--;

					snake[n].x = snake[n-1].x;
					snake[n].y = snake[n-1].y;
					snake[n].w = SIZE;
					snake[n].h = SIZE;
					for( int i = n-1; i > 0; i-- )
					{
						snake[i].x = snake[i-1].x;
						snake[i].y = snake[i-1].y;
					}
					n++;
				}
				else
				{
					for ( int i = n-1; i > 0; i-- )
					{
						snake[i].x = snake[i-1].x;
						snake[i].y = snake[i-1].y;
					}
				}

				switch( direction )
				{
					case 1:
						if( snake[0].x + SIZE+1 == SCREEN_WIDTH)
						{
							snake[0].x = 1;
						}
						else
						{
							snake[0].x += SIZE+1;
						}
						break;
					case 2:
						if( snake[0].y == 1 )
						{
							snake[0].y = SCREEN_HEIGHT - SIZE - 1;
						}
						else
						{
							snake[0].y -= SIZE+1;
						}
						break;
					case 3:
						if( snake[0].x == 1 )
						{
							snake[0].x = SCREEN_WIDTH - SIZE - 1;
						}
						else
						{
							snake[0].x -= SIZE+1;
						}
						break;
					case 4:
						if( snake[0].y + SIZE+1 == SCREEN_HEIGHT )
						{
							snake[0].y = 1;
						}
						else
						{
							snake[0].y += SIZE+1;
						}
						break;
				}

				if( snake[0].x == food.x && snake[0].y == food.y )
				{
					getFood();
					eaten += 8;
				}

				SDL_SetRenderDrawColor( Renderer, 153, 255, 51, 255 );
				SDL_RenderFillRect( Renderer, &food );

				SDL_SetRenderDrawColor( Renderer, 0, 102, 153, 255 );
				for( int i = 1; i < n; i++ )
				{
					SDL_RenderFillRect( Renderer, &snake[i] );
				}
				
				SDL_SetRenderDrawColor( Renderer, 200, 0, 0, 255 );
				SDL_RenderFillRect( Renderer, &snake[0] );

				SDL_RenderPresent( Renderer );
				SDL_Delay(80);
			}
		}
	}

	close();
	return 0;
}
