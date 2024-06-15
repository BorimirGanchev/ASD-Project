#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <unistd.h> 
#include "./constants.h"

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
        "Program",
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
    add_circle(circle_map, 0, 50, 400, 15, 255, 0, 0, 255);
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
    add_circle(circle_map, 20, 450, 380, 15, 255, 0, 0, 255);
    add_circle(circle_map, 21, 800, 380, 15, 255, 0, 0, 255);
    add_circle(circle_map, 18, 1150, 380, 15, 255, 0, 0, 255);
}

void render(CircleMap *circle_map) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    SDL_Rect shapes[NUM_SHAPES] = {
        //Frame
        {20, 20, 5, 460},
        {20, 20, 1460, 5}, 
        {20, 480, 1460, -5}, 
        {1480, 20, -5, 460},
        //First block
        {60, 60, 100, 300},
        {200, 60, 100, 300},
        {340, 60, 630, 130},
        {340, 230, 630, 130},
        {340, 60, 630, 130},
        {1010, 60, 430, 300},

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
            fprintf(stderr, "Circle with ID %d not found.\n", current_id);
            continue;
        }

        Circle* prev_circle = (prev_id == -1) ? NULL : get_circle(circle_map, prev_id);
        if (prev_id != -1 && !prev_circle) {
            fprintf(stderr, "Previous Circle with ID %d not found.\n", prev_id);
            continue;
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
        usleep(500000);
    }

    Circle* last_circle = get_circle(circle_map, path[path_length - 1]);
    if (!last_circle) {
        fprintf(stderr, "Last Circle with ID %d not found.\n", path[path_length - 1]);
        return;
    }

    last_circle->r = 0;
    last_circle->g = 255;
    last_circle->b = 0;
    last_circle->a = 255;
    render(circle_map);

    game_is_running = false;
}



int main(int argc, char* args[]) {
    CircleMap circle_map = { .size = 0 };

    FILE *file;
    int numbers[100];
    int count = 0;
    char buffer[256];

    file = fopen("filename.txt", "r");
    if (file == NULL) {
        perror("Error opening file");
        return EXIT_FAILURE;
    }

    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        char *ptr = buffer;
        while (*ptr != '\0') {
            if (*ptr == ' ' || *ptr == ',') {
                ptr++;
                continue;
            }
            numbers[count] = strtol(ptr, &ptr, 10);
            count++;
            if (count >= 500) {
                fprintf(stderr, "Error: Too many numbers in the file\n");
                fclose(file);
                return EXIT_FAILURE;
            }
        }
    }

    fclose(file);
    int path_length = sizeof(numbers) / sizeof(numbers[0]);

    game_is_running = initialize_window();
    setup(&circle_map);

    update_circle_colors(&circle_map, numbers, path_length);

    while (game_is_running) {
        process_input();
        render(&circle_map);
    }

    destroy_window();

    return 0;
}