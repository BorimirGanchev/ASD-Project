#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <unistd.h>  // For usleep

#include "./constants.h"

#define NUM_CIRCLES 50

int game_is_running = false;
int last_frame_time = 0;
SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;

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
    //first line
    add_circle(circle_map, 3, 110, 50, 15, 255, 0, 0, 255);
    add_circle(circle_map, 6, 250, 50, 15, 255, 0, 0, 255);
    add_circle(circle_map, 10, 450, 50, 15, 255, 0, 0, 255);
    add_circle(circle_map, 12, 800, 50, 15, 255, 0, 0, 255);
    add_circle(circle_map, 16, 1250, 50, 15, 255, 0, 0, 255);
    //line 2
    add_circle(circle_map, 2, 50, 110, 15, 255, 0, 0, 255);
    add_circle(circle_map, 4, 180, 110, 15, 255, 0, 0, 255);
    add_circle(circle_map, 8, 320, 110, 15, 255, 0, 0, 255);
    add_circle(circle_map, 14, 990, 110, 15, 255, 0, 0, 255);
    //line 2.5
    add_circle(circle_map, 11, 450, 210, 15, 255, 0, 0, 255);
    add_circle(circle_map, 13, 800, 210, 15, 255, 0, 0, 255);
    add_circle(circle_map, 17, 1460, 210, 15, 255, 0, 0, 255);
    //line 3
    add_circle(circle_map, 1, 50, 300, 15, 255, 0, 0, 255);
    add_circle(circle_map, 5, 180, 300, 15, 255, 0, 0, 255);
    add_circle(circle_map, 9, 320, 300, 15, 255, 0, 0, 255);
    add_circle(circle_map, 15, 990, 300, 15, 255, 0, 0, 255);
    //line 4
    add_circle(circle_map, 19, 110, 380, 15, 255, 0, 0, 255);
    add_circle(circle_map, 7, 250, 380, 15, 255, 0, 0, 255);
    // add_circle(circle_map, 38, 300, 480, 15, 255, 0, 0, 255);
    add_circle(circle_map, 20, 450, 380, 15, 255, 0, 0, 255);
    add_circle(circle_map, 21, 800, 380, 15, 255, 0, 0, 255);
    // add_circle(circle_map, 35, 1000, 480, 15, 255, 0, 0, 255);
    add_circle(circle_map, 18, 1150, 380, 15, 255, 0, 0, 255);
    // add_circle(circle_map, 18, 1250, 480, 15, 255, 0, 0, 255);
    // add_circle(circle_map, 20, 1400, 480, 15, 255, 0, 0, 255);
    // //line 4.5
    // add_circle(circle_map, 46, 50, 550, 15, 255, 0, 0, 255);
    // add_circle(circle_map, 44, 180, 550, 15, 255, 0, 0, 255);
    // //line 5
    // add_circle(circle_map, 40, 450, 650, 15, 255, 0, 0, 255);
    // add_circle(circle_map, 34, 900, 650, 15, 255, 0, 0, 255);
    // add_circle(circle_map, 21, 1190, 650, 15, 255, 0, 0, 255);
    // add_circle(circle_map, 22, 1320, 650, 15, 255, 0, 0, 255);
    // add_circle(circle_map, 23, 1460, 650, 15, 255, 0, 0, 255);
    // //line 5.5
    // add_circle(circle_map, 47, 50, 750, 15, 255, 0, 0, 255);
    // //line 6
    // add_circle(circle_map, 43, 110, 820, 15, 255, 0, 0, 255);
    // //add_circle(circle_map, 39, 410, 820, 15, 255, 0, 0, 255);
    // add_circle(circle_map, 32, 680, 820, 15, 255, 0, 0, 255);
    // add_circle(circle_map, 33, 1000, 820, 15, 255, 0, 0, 255);
    // add_circle(circle_map, 24, 1250, 820, 15, 255, 0, 0, 255);
    // //add_circle(circle_map, 26, 1320, 820, 15, 255, 0, 0, 255);
    // add_circle(circle_map, 25, 1390, 820, 15, 255, 0, 0, 255);
    // //line 6.5
    // add_circle(circle_map, 42, 50, 890, 15, 255, 0, 0, 255);
    // add_circle(circle_map, 29, 1190, 890, 15, 255, 0, 0, 255);
    // add_circle(circle_map, 27, 1460, 890, 15, 255, 0, 0, 255);
    // //line 7
    // add_circle(circle_map, 41, 110, 950, 15, 255, 0, 0, 255);
    // add_circle(circle_map, 31, 610, 950, 15, 255, 0, 0, 255);
    // add_circle(circle_map, 30, 1050, 950, 15, 255, 0, 0, 255);
    // add_circle(circle_map, 28, 1320, 950, 15, 255, 0, 0, 255);
}

void render(CircleMap *circle_map) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    SDL_Rect shapes[NUM_SHAPES] = {
        //Frame
        {20, 20, 5, 860},
        {20, 20, 1460, 5}, 
        {20, 980, 1460, -5}, 
        {1480, 20, -5, 860},
        //First block
        {60, 60, 100, 300},
        {200, 60, 100, 300},
        {340, 60, 630, 130},
        {340, 230, 630, 130},
        {340, 60, 630, 130},
        {1010, 60, 430, 300},
        //Second block
        // {60, 500, 100, 300},
        // {160, 670, 490, 130},
        // {200, 500, 960, 130},
        // {690, 670, 470, 130},
        // {1200, 500, 100, 300},
        // {1340, 500, 100, 300},
        // {60, 840, 1100, 100},
        // {1200, 840, 240, 100},
    };

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    for (int i = 0; i < NUM_SHAPES; ++i) {
        SDL_RenderFillRect(renderer, &shapes[i]);
    }

    for (int i = 0; i < circle_map->size; i++) {
        FillCircle(renderer, &circle_map->circles[i]);
    }

    SDL_RenderPresent(renderer);
}

void destroy_window(void) {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void update_circle_colors(CircleMap *circle_map, int path[], int path_length) {
    for (int i = 0; i < path_length; ++i) {
        int current_id = path[i];
        int prev_id = (i == 0) ? -1 : path[i - 1];

        Circle* current_circle = get_circle(circle_map, current_id);
        if (!current_circle) {
            fprintf(stderr, "Error: Circle with ID %d not found.\n", current_id);
            continue; // Skip this iteration if the circle is not found
        }

        Circle* prev_circle = (prev_id == -1) ? NULL : get_circle(circle_map, prev_id);
        if (prev_id != -1 && !prev_circle) {
            fprintf(stderr, "Error: Previous Circle with ID %d not found.\n", prev_id);
            continue; // Skip this iteration if the previous circle is not found
        }

        if (prev_circle) {
            prev_circle->r = 0;
            prev_circle->g = 255;
            prev_circle->b = 0;
            prev_circle->a = 255;
        }

        current_circle->r = 255;
        current_circle->g = 255;
        current_circle->b = 0;
        current_circle->a = 255;

        render(circle_map);
        usleep(500000); // Sleep for 500 milliseconds
    }

    Circle* last_circle = get_circle(circle_map, path[path_length - 1]);
    if (!last_circle) {
        fprintf(stderr, "Error: Last Circle with ID %d not found.\n", path[path_length - 1]);
        return;
    }

    last_circle->r = 0;
    last_circle->g = 255;
    last_circle->b = 0;
    last_circle->a = 255;
    render(circle_map);

    // Stop the game loop after finishing the iteration
    game_is_running = false;
}



int main(int argc, char* args[]) {
    CircleMap circle_map = { .size = 0 };

    // Define the path array
    int path[] = {0, 1, 2, 3, 6, 10, 12, 14, 15, 18, 17, 16, 13, 11, 8, 4, 5, 7, 20, 21, 13, 11, 9, 19, 0 };
    int path_length = sizeof(path) / sizeof(path[0]);

    game_is_running = initialize_window();
    setup(&circle_map);

    // Call update_circle_colors to start the path traversal
    update_circle_colors(&circle_map, path, path_length);

    // Main game loop
    while (game_is_running) {
        process_input();
        // Note: The update function is not needed here since we handle updates in update_circle_colors
        render(&circle_map);
    }

    destroy_window();

    return 0;
}

