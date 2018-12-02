#include <stdio.h>
#include <stdlib.h>

#include "SDL.h"

#define SCREEN_W 640
#define SCREEN_H 480

int main(int argc, char * argv[])
{
	int ret;
	int quit = 0;
	SDL_Window * window = NULL;
	SDL_Surface * surface = NULL;
	SDL_Surface * bmpsurface = NULL;

	SDL_Event e;
	ret = SDL_Init(SDL_INIT_EVERYTHING);
	if (ret < 0)
	{
		printf("SDL init failed! error code:%s", SDL_GetError());
		return -1;
	}

	window = SDL_CreateWindow("SDL Sample01", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_W, SCREEN_H, SDL_WINDOW_SHOWN);
	if (window==NULL)
	{
		printf("SDL_CreateWindow failed! error code:%s",SDL_GetError());
		return -1;
	}
	surface = SDL_GetWindowSurface(window);
	if (surface==NULL)
	{
		printf("SDL_GetWinsowSurface failed! error code:%s", SDL_GetError());
		return -1;
	}
	//SDL_FillRect(surface,NULL,SDL_MapRGB(surface->format, 0, 0xff, 0));
	bmpsurface = SDL_LoadBMP("E:\\GitSourse\\SDL2Sample\\wdm.bmp");
	SDL_BlitSurface(bmpsurface,NULL,surface,NULL);

	SDL_UpdateWindowSurface(window);

	while (!quit)
	{
		while (SDL_PollEvent(&e)!=0)
		{
			if (e.type == SDL_QUIT)
			{
				quit = 1;
			}else if (e.type == SDL_KEYDOWN)
			{
				printf("you pressed %d\n", e.key.keysym.sym);
			} 
			else if(e.type == SDL_KEYUP)
			{
				printf("you released %d.\n",e.key.keysym.sym);
			}
		}
	}
	//SDL_Delay(5000);

	SDL_FreeSurface(bmpsurface);
	SDL_DestroyWindow(window);
	SDL_Quit();
	//getchar();
	return 0;
}