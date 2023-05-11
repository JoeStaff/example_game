#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_gfxPrimitives.h>

#include <map>
#include "../logger.h"

namespace sdl1{
namespace image{

    SDL_Surface* screenSurface;

    void Initiate(){
        
        // Initialize SDL
        if (SDL_InitSubSystem(SDL_INIT_VIDEO) < 0) {
            log(ERROR)<<"SDL initialization failed: "<< SDL_GetError();
        }

        // Create the SDL window
        screenSurface = SDL_SetVideoMode(640, 480, 32, SDL_SWSURFACE);
        if (screenSurface == NULL) {
            log(ERROR)<<"Failed to create SDL window: "<< SDL_GetError();
        }
        log(INFO) << "SDL image initialized successfully.";
    }

    void DrawToScreen(SDL_Surface* surface, int x, int y) {
        if (surface == NULL || screenSurface == NULL) {
            log(ERROR)<<"Invalid surface passed to DrawToScreen function.";
            return;
        }

        // Create a temporary rectangle to hold the blit position
        SDL_Rect destRect;
        destRect.x = x;
        destRect.y = y;
        destRect.w = surface->w;
        destRect.h = surface->h;

        // Blit the surface to the screen surface
        if (SDL_BlitSurface(surface, NULL, screenSurface, &destRect) != 0) {
            log(ERROR)<<"Failed to blit surface to screen: "<<SDL_GetError();
        }
    }

    SDL_Surface* LoadSprite(const std::string& filename) {
        SDL_Surface* loadedSurface = NULL;
        SDL_Surface* optimizedSurface = NULL;

        // Load the image using SDL_image
        loadedSurface = IMG_Load(filename.c_str());
        if (loadedSurface == NULL) {
            log(ERROR)<<"Failed to load image " << filename << " " << IMG_GetError();
            return NULL;
        }

        // Optimize the surface
        optimizedSurface = SDL_DisplayFormatAlpha(loadedSurface);
        SDL_FreeSurface(loadedSurface); // Free the loaded surface

        if (optimizedSurface == NULL) {
            log(ERROR)<<"Failed to optimize image " << filename << " " << IMG_GetError();
            return NULL;
        }

        return optimizedSurface;
    }   

    
    SDL_Surface* NewSpriteScale(SDL_Surface* surface, int newWidth, int newHeight) {
        if (surface == NULL) {
            log(ERROR)<<"Invalid surface passed to scaleSurface function.";
            return NULL;
        }

        // Create a new surface with the desired width and height
        SDL_Surface* newSurface = SDL_CreateRGBSurface(surface->flags, newWidth, newHeight,
                                                    surface->format->BitsPerPixel,
                                                    surface->format->Rmask, surface->format->Gmask,
                                                    surface->format->Bmask, surface->format->Amask);
        if (newSurface == NULL) {
            log(ERROR)<<"Failed to create scaled surface. "<<SDL_GetError();
            return NULL;
        }

        // Use SDL_gfx to perform the surface scaling
        if (SDL_SoftStretch(surface, NULL, newSurface, NULL) != 0) {
            log(ERROR)<<"Failed to scale surface. "<<SDL_GetError();
            SDL_FreeSurface(newSurface);
            return NULL;
        }

        return newSurface;
    }

    SDL_Surface* NewSpriteColor(SDL_Surface* surface, Uint8 red, Uint8 green, Uint8 blue, Uint8 alpha) {
        if (surface == NULL) {
            printf("Invalid surface passed to modifySurfaceColor function\n");
            return NULL;
        }

        // Create a new surface with the same format and size
        SDL_Surface* newSurface = SDL_CreateRGBSurface(surface->flags, surface->w, surface->h,
                                                    surface->format->BitsPerPixel,
                                                    surface->format->Rmask, surface->format->Gmask,
                                                    surface->format->Bmask, surface->format->Amask);
        if (newSurface == NULL) {
            printf("Failed to create modified surface: %s\n", SDL_GetError());
            return NULL;
        }

        // Lock the surfaces to access the pixels
        if (SDL_LockSurface(surface) != 0 || SDL_LockSurface(newSurface) != 0) {
            printf("Failed to lock surfaces: %s\n", SDL_GetError());
            SDL_FreeSurface(newSurface);
            return NULL;
        }

        // Modify the color and alpha values of each pixel
        for (int y = 0; y < surface->h; ++y) {
            for (int x = 0; x < surface->w; ++x) {
                Uint32 pixel = *((Uint32*)((Uint8*)surface->pixels + y * surface->pitch + x * sizeof(Uint32)));

                Uint8 originalR, originalG, originalB, originalA;
                SDL_GetRGBA(pixel, surface->format, &originalR, &originalG, &originalB, &originalA);

                Uint8 newR = (originalR * red) / 255;
                Uint8 newG = (originalG * green) / 255;
                Uint8 newB = (originalB * blue) / 255;
                Uint8 newA = (originalA * alpha) / 255;

                Uint32 newPixel = SDL_MapRGBA(newSurface->format, newR, newG, newB, newA);
                *((Uint32*)((Uint8*)newSurface->pixels + y * newSurface->pitch + x * sizeof(Uint32))) = newPixel;
            }
        }

        // Unlock the surfaces
        SDL_UnlockSurface(surface);
        SDL_UnlockSurface(newSurface);

        return newSurface;
    }

    SDL_Surface* NewSpriteRotation(SDL_Surface* surface, double angle) {
        if (surface == NULL) {
            printf("Invalid surface passed to rotateSurface function\n");
            return NULL;
        }

        // Create a new surface with the same dimensions
        SDL_Surface* newSurface = SDL_CreateRGBSurface(surface->flags, surface->w, surface->h,
                                                    surface->format->BitsPerPixel,
                                                    surface->format->Rmask, surface->format->Gmask,
                                                    surface->format->Bmask, surface->format->Amask);
        if (newSurface == NULL) {
            printf("Failed to create rotated surface: %s\n", SDL_GetError());
            return NULL;
        }

        // Lock the surfaces
        if (SDL_LockSurface(surface) != 0 || SDL_LockSurface(newSurface) != 0) {
            printf("Failed to lock surfaces: %s\n", SDL_GetError());
            SDL_FreeSurface(newSurface);
            return NULL;
        }

        // Calculate the rotation center
        int centerX = surface->w / 2;
        int centerY = surface->h / 2;

        // Perform the surface rotation
        for (int y = 0; y < surface->h; y++) {
            for (int x = 0; x < surface->w; x++) {
                // Calculate the coordinates relative to the center
                int xOffset = x - centerX;
                int yOffset = y - centerY;

                // Apply the rotation transformation
                double newX = xOffset * cos(angle) - yOffset * sin(angle);
                double newY = xOffset * sin(angle) + yOffset * cos(angle);

                // Calculate the original coordinates
                int srcX = (int)(newX + centerX);
                int srcY = (int)(newY + centerY);

                // Check if the original coordinates are within bounds
                if (srcX >= 0 && srcX < surface->w && srcY >= 0 && srcY < surface->h) {
                    // Copy the pixel to the rotated surface
                    Uint32 pixel = *((Uint32*)surface->pixels + srcY * surface->pitch / sizeof(Uint32) + srcX);
                    *((Uint32*)newSurface->pixels + y * newSurface->pitch / sizeof(Uint32) + x) = pixel;
                }
            }
        }

        // Unlock the surfaces
        SDL_UnlockSurface(surface);
        SDL_UnlockSurface(newSurface);

        return newSurface;
    }

    void UpdateScreen(){
        SDL_Flip(screenSurface);
        // Create a SDL_Rect representing the entire surface
        SDL_Rect rect = { 0, 0, screenSurface->w, screenSurface->h };

        // Fill the surface with the specified color
        Uint32 color = SDL_MapRGB(screenSurface->format, 0, 0, 0);
        SDL_FillRect(screenSurface, &rect, color);
    }

}
}