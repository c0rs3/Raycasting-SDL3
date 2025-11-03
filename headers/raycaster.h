#pragma once
#ifndef RAYCASTER
#define RAYCASTER

#define BRED    0b11111111'00000000'00000000'11111111
#define BBLUE   0b00000000'11111111'00000000'11111111
#define BGREEN  0b00000000'00000000'11111111'11111111
#define BWHITE  0b11111111'11111111'11111111'11111111
#define BBLACK  0b11111111'11111111'11111111'00000000
#define BORANGE 0b11111111'10110000'00000000'11111111 
#define BPURPLE 0b10011111'00110011'11110000'11111111 
#define BTEAL   0b00110011'11001100'11001100'11111111 
#define BOLIVE  0b10111111'10111111'00110000'11111111 
#define BMAROON 0b10000000'00000000'00110000'11111111 
#define BNAVY   0b00000000'00000000'10000011'11111111 
#define BSILVER 0b11001100'11001100'11001100'11111111 
#define BGRAY   0b10000000'10000000'10000000'11111111 
#define BTRANS  0b00000000'00000000'00000000'00000000

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
#include <cstring>
#include <sys/stat.h>
#include <thread>
#include <memory>

// SDL 
#include <SDL3/SDL.h>
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_scancode.h>
#include <SDL3/SDL_timer.h>
#include <SDL3/SDL_video.h>
#include <SDL3/SDL_keyboard.h>
#include <SDL3/SDL_opengl.h>
#include <SDL3/SDL_mouse.h>

#endif

