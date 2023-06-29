#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <raylib.h>

#define MAX_ITEMS 10

typedef struct {
    Vector2 pos;
    Color color;
    float time;
} Item;

const int width = 800;
const int height = 600;

const float radius = 20;
const int bottom = height - radius;

const float speed = 1.0;
const float gravity = 15.0;

static int index = 0;
static int frames = 0;

static Item item_col[MAX_ITEMS];

Item *create_item()
{
    Item *item = malloc(sizeof(Item));
    item->pos = GetMousePosition();
    item->color = BLUE;
    item->time = 0;
    return item;
}

void process_click()
{
    // Create new ball at mouse click
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        if (index < MAX_ITEMS) {
            item_col[index] = *create_item();
            printf("Index %d x%f, y%f\n", index, item_col[index].pos.x, item_col[index].pos.y);
            index++;
        }
    }
}

void update_items()
{
    for (int i = 0; i < index; i++) {
        // Increase the items time since creation
        if ((frames / 60 % 1) == 0) {
            item_col[i].time += 0.1f;
        }
        // Increase speed based on gravity formula
        if (item_col[i].pos.y <= bottom) {
            if ((frames/60 % 1) == 0) { // Every second (60 frames)
                item_col[i].pos.y += (0.5f * gravity * powf(item_col[i].time, 2));
                if (item_col[i].pos.y > bottom) {
                    item_col[i].pos.y = bottom;
                }
            }
        }
    }
}

void draw_items()
{
    // Draw all items
    for (int i = 0; i < index; i++) {
        DrawCircleV(item_col[i].pos, radius, BLUE);
    }
}

int main(void)
{
    InitWindow(800, 600, "Physics");

    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        frames++;

        process_click();

        update_items();

        BeginDrawing();
        
        ClearBackground(WHITE);

        draw_items();

        EndDrawing();
    }
    return 0;
}
