#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include "./constants.h"
#include "./algorithm.h"

#define NUM_CIRCLES 50

int game_is_running = false;
int last_frame_time = 0;
SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;

struct game_object {
    float x;
    float y;
    float width;
    float height;
    float vel_x;
    float vel_y;
} ball, paddle;

typedef struct {
    int id;
    int x;
    int y;
    int radius;
    Uint8 r;
    Uint8 g;
    Uint8 b;
    Uint8 a;
} Circle;

typedef struct {
    Circle circles[NUM_CIRCLES];
    int size;
} CircleMap;

int initialize_window(void) {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        fprintf(stderr, "Error initializing SDL.\n");
        return false;
    }
    window = SDL_CreateWindow(
        "A simple game loop using C & SDL",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        WINDOW_WIDTH,
        WINDOW_HEIGHT,
        0
    );
    if (!window) {
        fprintf(stderr, "Error creating SDL Window.\n");
        return false;
    }
    renderer = SDL_CreateRenderer(window, -1, 0);
    if (!renderer) {
        fprintf(stderr, "Error creating SDL Renderer.\n");
        return false;
    }
    return true;
}

void process_input(void) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                game_is_running = false;
                break;
            case SDL_KEYDOWN:
                if (event.key.keysym.sym == SDLK_ESCAPE) {
                    game_is_running = false;
                }
                break;
        }
    }
}

void add_circle(CircleMap *map, int id, int x, int y, int radius, Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
    if (map->size < NUM_CIRCLES) {
        Circle new_circle = {id, x, y, radius, r, g, b, a};
        map->circles[map->size++] = new_circle;
    }
}

Circle* get_circle(CircleMap *map, int id) {
    for (int i = 0; i < map->size; i++) {
        if (map->circles[i].id == id) {
            return &map->circles[i];
        }
    }
    return NULL;
}

void FillCircle(SDL_Renderer *renderer, Circle *circle) {
    SDL_SetRenderDrawColor(renderer, circle->r, circle->g, circle->b, circle->a);

    for (int y = -circle->radius; y <= circle->radius; y++) {
        for (int x = -circle->radius; x <= circle->radius; x++) {
            if (x * x + y * y <= circle->radius * circle->radius) {
                SDL_RenderDrawPoint(renderer, circle->x + x, circle->y + y);
            }
        }
    }
}

void setup(CircleMap *circle_map) {
    // Adding circles with unique IDs
    add_circle(circle_map, 3, 110, 50, 15, 255, 0, 0, 255);
    add_circle(circle_map, 6, 250, 50, 15, 255, 0, 0, 255);
    add_circle(circle_map, 10, 450, 50, 15, 255, 0, 0, 255);
    add_circle(circle_map, 12, 800, 50, 15, 255, 0, 0, 255);
    add_circle(circle_map, 11, 900, 50, 15, 255, 0, 0, 255);
    add_circle(circle_map, 4, 50, 150, 15, 255, 0, 0, 255);
    add_circle(circle_map, 1, 250, 150, 15, 255, 0, 0, 255);
    add_circle(circle_map, 7, 450, 150, 15, 255, 0, 0, 255);
    add_circle(circle_map, 5, 800, 150, 15, 255, 0, 0, 255);
    add_circle(circle_map, 2, 50, 370, 15, 255, 0, 0, 255);
    add_circle(circle_map, 8, 250, 400, 15, 255, 0, 0, 255);
    add_circle(circle_map, 9, 450, 450, 15, 255, 0, 0, 255);
    add_circle(circle_map, 0, 350, 450, 15, 255, 0, 0, 255);
    add_circle(circle_map, 13, 800, 450, 15, 255, 0, 0, 255);
    add_circle(circle_map, 14, 900, 450, 15, 255, 0, 0, 255);
    add_circle(circle_map, 15, 900, 150, 15, 255, 0, 0, 255);

    // Sample graph initialization
    n = 16; // number of nodes
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            graph[i][j] = INT_MAX; // No edge
        }
    }

    // Sample edges
    graph[0][1] = 10; graph[1][0] = 10;
    graph[0][2] = 20; graph[2][0] = 20;
    graph[1][2] = 30; graph[2][1] = 30;
    // Add more edges as required
}

void update(void) {
    int time_to_wait = FRAME_TARGET_TIME - (SDL_GetTicks() - last_frame_time);
    if (time_to_wait > 0 && time_to_wait <= FRAME_TARGET_TIME) {
        SDL_Delay(time_to_wait);
    }
    last_frame_time = SDL_GetTicks();
}

void render(CircleMap *circle_map, int *finalPath) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    // Draw Circles
    for (int i = 0; i < circle_map->size; i++) {
        FillCircle(renderer, &circle_map->circles[i]);
    }

    // Update colors based on finalPath
    for (int i = 0; finalPath[i] != -1; i++) {
        Circle *circle = get_circle(circle_map, finalPath[i]);
        if (circle) {
            if (i == 0) {
                circle->r = 0; circle->g = 255; circle->b = 0; // Starting node
            } else if (finalPath[i + 1] == -1) {
                circle->r = 255; circle->g = 255; circle->b = 0; // Ending node
            } else {
                circle->r = 0; circle->g = 0; circle->b = 255; // Visited node
            }
        }
    }

    SDL_RenderPresent(renderer);
}

void destroy_window(void) {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

int main(void) {
    CircleMap circle_map = {.size = 0};

    game_is_running = initialize_window();

    setup(&circle_map);

    int finalPath[MAX];
    nearestNeighbor(0, finalPath);

    while (game_is_running) {
        process_input();
        update();
        render(&circle_map, finalPath);
    }

    destroy_window();

    return 0;
}
