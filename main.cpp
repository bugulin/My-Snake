/*
 * This is my c++ Snake
 * - by petriczech
 */

#include <SDL2/SDL.h>
#include <iostream>
#include <ctime>

const int WIDTH = 35;//25;
const int HEIGHT = 25;//15;
const int SIZE = 30;
const int max_length = WIDTH * HEIGHT / 3 * 2;

const int SCREEN_WIDTH = 1 + WIDTH * (SIZE+1);
const int SCREEN_HEIGHT = 1 + HEIGHT * (SIZE+1);

int n;
int eaten = 0;
int direction = 0;
bool paused = false;
SDL_Rect food = {0, 0, SIZE, SIZE};

SDL_Rect snake[max_length];

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
int collide( int x, int y, SDL_Rect rect )
{
	if( x == rect.x && y == rect.y )
	{
		return 1;
	}
	return 0;
}

void getFood()
{
	bool OK = false;
	int x;
	int y;

	do {
		x = 1 + (rand()%WIDTH) * (SIZE+1);
		y = 1 + (rand()%HEIGHT) * (SIZE+1);
		
		OK = true;

		for( int i = 0; i < n; i++ )
		{
			if( collide(x, y, snake[i]) )
			{
				OK = false;
				break;
			}
		}
	} while( !OK );

	food.x = x;
	food.y = y;
}

void newGame()
{
		n = 3;
		eaten = 0;
		for( int i = 0; i < n; i++ )
		{
			snake[i].w = SIZE;
			snake[i].h = SIZE;
			snake[i].x = 1 + (SIZE+1)*(n-i);
			snake[i].y = 1;
		}
		
		direction = 1;
		getFood();
}

int main( int, char**)
{
	if( !init() )
	{
		std::cout << "Failed to initialize!" << std::endl;
	}
	else {
		srand(time(NULL));
		
		
		newGame();

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
				std::cout << n << " / " << max_length << std::endl;
				SDL_SetRenderDrawColor( Renderer, 50, 50, 50, 255 );
				SDL_RenderClear( Renderer );
				
				drawLandMarks();
				
				if( eaten > 0 ) {
					eaten--;

					if( n >= max_length )
					{
						newGame();
					}
					else {
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
					eaten += 2;
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
				SDL_Delay(150);
			}
		}
	}

	close();
	return 0;
}
