
#include "fractal.hpp"

#include <stdexcept>

int SCREEN_WIDTH = 640;  // Default value
int SCREEN_HEIGHT = 480; // Default value

int main(int argc, char* args[])
{
    if (argc == 3) {
        try {
            SCREEN_WIDTH = std::stoi(args[1]);
            SCREEN_HEIGHT = std::stoi(args[2]);
        } catch (const std::invalid_argument& e) {
            std::cerr << "Invalid arguments. Using default screen size." << std::endl;
        }
    }

    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    SDL_Texture* texture = nullptr;
	ComplexPlane plane(-2.0, 1.5, -1.5, 1.5, SCREEN_WIDTH, SCREEN_HEIGHT, 100);

    if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    window = SDL_CreateWindow("SDL Pixel Drawing", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (window == nullptr)
	{
        std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == nullptr)
	{
        std::cerr << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, SCREEN_WIDTH, SCREEN_HEIGHT);
    if (texture == nullptr)
	{
        std::cerr << "Texture could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    bool quit = false;
    SDL_Event e;

    while (!quit)
	{
        while (SDL_PollEvent(&e) != 0)
		{
			if ((e.type == SDL_QUIT) || (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE))
				quit = true;
			//check the + and - keys to increase the number of iterations
			if (e.type == SDL_KEYDOWN)
			{
				if (e.key.keysym.sym == SDLK_KP_PLUS || e.key.keysym.sym == SDLK_PLUS)
					plane.iterations += 100;
				if (e.key.keysym.sym == SDLK_KP_MINUS || e.key.keysym.sym == SDLK_MINUS)
					plane.iterations -= 100;
			}
        }

        uint32_t* pixels = nullptr;
        int pitch = 0;
        SDL_LockTexture(texture, nullptr, (void**)&pixels, &pitch);

        for (int y = 0; y < SCREEN_HEIGHT; ++y)
		{
            for (int x = 0; x < SCREEN_WIDTH; ++x)
			{
				std::complex<double> c = plane.pixelToComplex(x, y);
				Vector3 color = calculateFractalColor(c, plane.iterations);

				uint32_t sdl_color = SDL_MapRGBA(SDL_AllocFormat(SDL_PIXELFORMAT_RGBA8888),
					static_cast<Uint8>(std::min(255.0, std::max(0.0, color.r * 255.0))),
					static_cast<Uint8>(std::min(255.0, std::max(0.0, color.g * 255.0))),
					static_cast<Uint8>(std::min(255.0, std::max(0.0, color.b * 255.0))),
					255);
                pixels[y * (pitch / sizeof(uint32_t)) + x] = sdl_color;
            }
        }

        SDL_UnlockTexture(texture);

        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, texture, nullptr, nullptr);
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}