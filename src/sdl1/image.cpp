#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_gfxPrimitives.h>
#include <SDL/SDL_ttf.h>

#include <map>
#include "../logger.h"
#include "image.h"
#include "../engine/image.h"

namespace sdl1{
namespace image{

    SDL_Surface* screenSurface;
    std::map<std::string,SDL_Surface*> spriteMap;

    void Initiate(){
        
        // Initialize SDL
        if (SDL_InitSubSystem(SDL_INIT_VIDEO) < 0) {
            log(ERROR)<<"SDL initialization failed: "<< SDL_GetError();
        }
        if (TTF_Init() < 0) {
            log(ERROR)<<"TTF initialization failed: "<< SDL_GetError();
        }

        // Create the SDL window
        screenSurface = SDL_SetVideoMode(640, 480, 32, SDL_SWSURFACE);
        if (screenSurface == NULL) {
            log(ERROR)<<"Failed to create SDL window: "<< SDL_GetError();
        }

        SDL_ShowCursor(false);
        
        engine::image::driverDrawToScreen(&DrawToScreen);
        engine::image::driverLoadSprite(&LoadSprite);
        engine::image::driverCreateText(&CreateText);
        engine::image::driverCreateBox(&CreateBox);
        engine::image::driverStitchSprite(&StitchSprite);
        engine::image::driverNewSpriteColor(&NewSpriteColor);
        engine::image::driverNewSpriteRotation(&NewSpriteRotation);
        engine::image::driverNewSpriteScale(&NewSpriteScale);
        engine::image::driverClearSprite(&ClearSprite);
        engine::image::driverUpdateScreen(&UpdateScreen);

        log(INFO) << "SDL image initialized successfully.";
    }

    void DrawToScreen(const std::string& original, int x, int y) {
        SDL_Surface* surface=spriteMap[original];
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

    void LoadSprite(const std::string& filename, const std::string& label) {
        SDL_Surface* loadedSurface = NULL;
        SDL_Surface* optimizedSurface = NULL;

        // Load the image using SDL_image
        loadedSurface = IMG_Load(filename.c_str());
        if (loadedSurface == NULL) {
            log(ERROR)<<"Failed to load image " << filename << " " << IMG_GetError();
            return;
        }

        // Optimize the surface
        optimizedSurface = SDL_DisplayFormatAlpha(loadedSurface);
        SDL_FreeSurface(loadedSurface); // Free the loaded surface

        if (optimizedSurface == NULL) {
            log(ERROR)<<"Failed to optimize image " << filename << " " << IMG_GetError();
            return;
        }

        if(spriteMap[label])
            SDL_FreeSurface(spriteMap[label]);
        spriteMap[label]=optimizedSurface;
        return;
    }   

    void CreateText(const std::string& label, const std::string& text, const std::string& fontFile, int fontSize, int r, int g, int b)
    {

        // Open the font
        TTF_Font* font = TTF_OpenFont(fontFile.c_str(), fontSize);
        if (font == nullptr)
        {
            // Error handling if the font failed to load
            log(ERROR)<<"Failed to load font: "<<TTF_GetError();
            return;
        }

        // Render the text
        SDL_Color textColor = { r, g, b };
        SDL_Surface* textSurface = TTF_RenderText_Blended(font, text.c_str(), textColor);
        if (textSurface == nullptr)
        {
            // Error handling if the text failed to render
            log(ERROR)<<"Failed to render text: "<<TTF_GetError();
            TTF_CloseFont(font);
            return;
        }

        // Clean up the font
        TTF_CloseFont(font);
        if(spriteMap[label])
            SDL_FreeSurface(spriteMap[label]);
        spriteMap[label]=textSurface;
        return;
    }
    void CreateBox(const std::string& label, int width, int height, int r, int g, int b){
        
        // Create the surface
        SDL_Surface* surface = SDL_CreateRGBSurface(SDL_SWSURFACE | SDL_SRCALPHA, width, height, 32, 0, 0, 0, 0);
        if (surface == nullptr)
        {
            // Error handling if the surface failed to create
            log(ERROR)<<"Failed to create surface: "<<SDL_GetError();
            return;
        }
        SDL_Rect rect={0,0,surface->w,surface->h};
        FillRectWithAlpha(surface,rect,r,g,b,255);
        SDL_Surface* blendedSurface = SDL_DisplayFormatAlpha(surface);
        SDL_FreeSurface(surface);
        if(spriteMap[label])
            SDL_FreeSurface(spriteMap[label]);
        spriteMap[label]=blendedSurface;
    }
    void StitchSprite(const std::string& sprite, const std::string& destination, const std::string& label, int x ,int y)
    {
        SDL_Surface* surfaceA=spriteMap[sprite];
        SDL_Surface* surfaceB=spriteMap[destination];
        // Create the new surface
        SDL_Surface* newSurface = SDL_CreateRGBSurface(surfaceB->flags, surfaceB->w, surfaceB->h,
                                                    surfaceB->format->BitsPerPixel,
                                                    surfaceB->format->Rmask, surfaceB->format->Gmask,
                                                    surfaceB->format->Bmask, surfaceB->format->Amask);
        if (newSurface == nullptr)
        {
            // Error handling if the new surface failed to create
            log(ERROR)<<"Failed to create new surface: "<<SDL_GetError();
            return;
        }

        // Blit surfaceB onto the new surface
        SDL_SetAlpha(surfaceB, 0, 0);  // Disable blending on the original surface
        SDL_SetAlpha(newSurface, 0, 0);  // Disable blending on the cloned surface

        SDL_BlitSurface(surfaceB, nullptr, newSurface, nullptr);

        SDL_SetAlpha(surfaceB, SDL_SRCALPHA, 255);  // Restore the original surface's alpha blending
        SDL_SetAlpha(newSurface, SDL_SRCALPHA, 255);  // Restore the cloned surface's alpha blending

        // Blit surfaceA onto the new surface at the specified position
        SDL_Rect destRect;
        destRect.x = x;
        destRect.y = y;
        SDL_BlitSurface(surfaceA, nullptr, newSurface, &destRect);

        if(spriteMap[label])
            SDL_FreeSurface(spriteMap[label]);
        spriteMap[label]=newSurface;
        return;
    }
    
    void NewSpriteScale(const std::string& original, const std::string& label, int newWidth, int newHeight) {
        SDL_Surface* surface=spriteMap[original];
        if (surface == NULL) {
            log(ERROR)<<"Invalid surface passed to scaleSurface function.";
            return;
        }

        // Create a new surface with the desired width and height
        SDL_Surface* newSurface = SDL_CreateRGBSurface(surface->flags, newWidth, newHeight,
                                                    surface->format->BitsPerPixel,
                                                    surface->format->Rmask, surface->format->Gmask,
                                                    surface->format->Bmask, surface->format->Amask);
        if (newSurface == NULL) {
            log(ERROR)<<"Failed to create scaled surface. "<<SDL_GetError();
            return;
        }

        // Use SDL_gfx to perform the surface scaling
        if (SDL_SoftStretch(surface, NULL, newSurface, NULL) != 0) {
            log(ERROR)<<"Failed to scale surface. "<<SDL_GetError();
            SDL_FreeSurface(newSurface);
            return;
        }
        
        if(spriteMap[label])
            SDL_FreeSurface(spriteMap[label]);
        spriteMap[label]=newSurface;
        return;
    }

    void NewSpriteColor(const std::string& original, const std::string& label, int red, int green, int blue, int alpha) {
        SDL_Surface* surface=spriteMap[original];
        if (surface == NULL) {
            log(ERROR)<<"Invalid surface passed to modifySurfaceColor function";
            return;
        }

        // Create a new surface with the same format and size
        SDL_Surface* newSurface = SDL_CreateRGBSurface(surface->flags, surface->w, surface->h,
                                                    surface->format->BitsPerPixel,
                                                    surface->format->Rmask, surface->format->Gmask,
                                                    surface->format->Bmask, surface->format->Amask);
        if (newSurface == NULL) {
            log(ERROR)<<"Failed to create modified surface: "<<SDL_GetError();
            return;
        }

        // Lock the surfaces to access the pixels
        if (SDL_LockSurface(surface) != 0 || SDL_LockSurface(newSurface) != 0) {
            log(ERROR)<<"Failed to lock surfaces: "<<SDL_GetError();
            SDL_FreeSurface(newSurface);
            return;
        }

        // Modify the color and alpha values of each pixel
        for (int y = 0; y < surface->h; ++y) {
            for (int x = 0; x < surface->w; ++x) {
                Uint32 pixel = *((Uint32*)((Uint8*)surface->pixels + y * surface->pitch + x * sizeof(Uint32)));

                Uint8 originalR, originalG, originalB, originalA;
                SDL_GetRGBA(pixel, surface->format, &originalR, &originalG, &originalB, &originalA);

                Uint8 newR = (originalR * red) / 255;
                Uint8 newG = (originalG * blue) / 255;
                Uint8 newB = (originalB * green) / 255;
                Uint8 newA = (originalA * alpha) / 255;

                Uint32 newPixel = SDL_MapRGBA(newSurface->format, newR, newG, newB, newA);
                *((Uint32*)((Uint8*)newSurface->pixels + y * newSurface->pitch + x * sizeof(Uint32))) = newPixel;
            }
        }

        // Unlock the surfaces
        SDL_UnlockSurface(surface);
        SDL_UnlockSurface(newSurface);

        if(spriteMap[label])
            SDL_FreeSurface(spriteMap[label]);
        spriteMap[label]=newSurface;
        return;
    }

    void NewSpriteRotation(const std::string& original, const std::string& label, double angle) {
        SDL_Surface* surface=spriteMap[original];
        if (surface == NULL) {
            log(ERROR)<<"Invalid surface passed to rotateSurface function";
            return;
        }

        // Create a new surface with the same dimensions
        SDL_Surface* newSurface = SDL_CreateRGBSurface(surface->flags, surface->w, surface->h,
                                                    surface->format->BitsPerPixel,
                                                    surface->format->Rmask, surface->format->Gmask,
                                                    surface->format->Bmask, surface->format->Amask);
        if (newSurface == NULL) {
            log(ERROR)<<"Failed to create rotated surface: "<<SDL_GetError();
            return;
        }

        // Lock the surfaces
        if (SDL_LockSurface(surface) != 0 || SDL_LockSurface(newSurface) != 0) {
            log(ERROR)<<"Failed to lock surfaces: "<<SDL_GetError();
            SDL_FreeSurface(newSurface);
            return;
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

        if(spriteMap[label])
            SDL_FreeSurface(spriteMap[label]);
        spriteMap[label]=newSurface;
        return;
    }

    void ClearSprite(const std::string& label){
        SDL_FreeSurface(spriteMap[label]);
    }

    void UpdateScreen(){
        SDL_Flip(screenSurface);
        // Create a SDL_Rect representing the entire surface
        SDL_Rect rect = { 0, 0, screenSurface->w, screenSurface->h };

        // Fill the surface with the specified color
        Uint32 color = SDL_MapRGB(screenSurface->format, 0, 0, 0);
        SDL_FillRect(screenSurface, &rect, color);
    }

    void FillRectWithAlpha(SDL_Surface* surface, SDL_Rect rect, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
    {
        // Convert the color to the format of the surface
        Uint32 color = SDL_MapRGBA(surface->format, r, g, b, a);

        // Lock the surface to access its pixels
        if (SDL_MUSTLOCK(surface))
            SDL_LockSurface(surface);

        // Draw the filled rectangle pixel by pixel
        Uint32* pixels = static_cast<Uint32*>(surface->pixels);
        int pitch = surface->pitch / sizeof(Uint32);
        for (int y = rect.y; y < rect.y + rect.h; ++y)
        {
            for (int x = rect.x; x < rect.x + rect.w; ++x)
            {
                pixels[y * pitch + x] = color;
            }
        }

        // Unlock the surface if necessary
        if (SDL_MUSTLOCK(surface))
            SDL_UnlockSurface(surface);
    }

}
}