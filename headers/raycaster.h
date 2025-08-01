#pragma once
// Flags
// #define USE_MAP_EDITOR

// STL
#include <cmath>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <stdint.h>
#include <filesystem>
#include <string>
#include <sys/stat.h>

// SDL 
#include <SDL3/SDL.h>
#include "SDL3/SDL_events.h"
#include "SDL3/SDL_init.h"
#include "SDL3/SDL_render.h"
#include "SDL3/SDL_scancode.h"
#include "SDL3/SDL_timer.h"
#include "SDL3/SDL_video.h"
#include "SDL3/SDL_keyboard.h"
#include "SDL3/SDL_opengl.h"

#ifdef USE_MAP_EDITOR
#include <imgui.h>
#include <imgui_impl_sdl3.h>
#include <imgui_impl_sdlrenderer3.h>
#endif
