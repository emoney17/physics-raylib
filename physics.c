#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <raylib.h>

typedef struct { // Object struct
    Vector2 center;
    Vector2 velocity;
    int radius;
    bool used;
} Object;

const int maxRadius = 20;
const int minRadius = 2;

const int screenWidth = 800;
const int screenHeight = 600;

static int collectionSize = 1;
static int index = 0;

Object *objCol; // Collection of created objects

Object *createObject();
float distance(Vector2 obj1, Vector2 obj2);

void resolveOutOfBounds();
void resolveCollision();
void resolveMovement();
void update();

// Object constructor
Object *createObject() {
    int randomRadius = (rand() % (maxRadius - minRadius + 1)) + minRadius;
    printf("Random radius %d\n", randomRadius);
    Object *object = malloc(sizeof(Object));
    object->center = GetMousePosition(); // Inital position/center is always the current mouse position
    object->velocity.x = 3.0f; // Move speed
    object->velocity.y = 2.0f; // Move speed
    object->radius = randomRadius;
    object->used = true;
    return object;
}

// Distance calculator
float distance(Vector2 obj1, Vector2 obj2) {
    return sqrtf(powf(obj2.x - obj1.x, 2.0f) + powf(obj2.y - obj1.y, 2.0f));
}

void resolveOutOfBounds() {
    for (int i = 0; i < collectionSize - 1; i++) { // Disable drawing object if it is off screen
        if (objCol[i].used && 
        objCol[i].center.x - objCol[i].radius > screenWidth || 
        objCol[i].center.x + objCol[i].radius < 0 || 
        objCol[i].center.y - objCol[i].radius > screenHeight || 
        objCol[i].center.y + objCol[i].radius < 0) { // Set object use to false and set coordinates constant val
            objCol[i].used = false;
            printf("Obj %d out of bounds\n", i);
            objCol[i].center.x = 0;
            objCol[i].center.y = 0;
        }
    }
}

void resolveCollision() {

}

void resolveMovement() {
    for (int i = 0; i < collectionSize - 1; i++) {
        if (objCol[i].used) { // If the Objects are being used, move them
            objCol[i].center.x += objCol[i].velocity.x;
            objCol[i].center.y += objCol[i].velocity.y;
        }
    }
}

void update() {
    resolveOutOfBounds();
    resolveMovement();
    resolveCollision();
}

int main(void) {
    srand(time(NULL));
    InitWindow(screenWidth, screenHeight, "Physics Testing");

    objCol = malloc(sizeof(Object));
    
    SetTargetFPS(60);
    while (!WindowShouldClose()) {

        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        // if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
            objCol[index] = *createObject();
            printf("Obj %d x%f, y%f\n", index, objCol[index].center.x, objCol[index].center.y);

            index++;
            collectionSize++;

            // Reallocate collection size safely
            Object *newObj;
            newObj = realloc(objCol, sizeof(Object) * collectionSize);
            if (newObj == NULL) {
                printf("Failed to reallocate memory\n");
            }
            objCol = newObj;
        }

        // Detect collision
        // for (int i = 0; i < bufferSize - 1; i++) {
        //     for (int j = 0; j < bufferSize - 2; j++) {
        //         if (i != j) {
        //             if (distance(objCol[j].center, objCol[i].center) < globalRadius * 2) {
        //                 // Move the objects back a certain ammount
        //             }
        //         }
        //     } 
        // }

        update();

        BeginDrawing();

        ClearBackground(BLACK);

        // Draw all of the objects in collection
        for (int i = 0; i < collectionSize - 1; i++) {
            if (objCol[i].used) {
                DrawCircleV(objCol[i].center, objCol[i].radius, WHITE);
            }
        }

        EndDrawing();
    }

    // Free allocated object collection
    free(objCol);
    printf("Object collection size: %d, freed\n", collectionSize);

    CloseWindow();
    return 0;
}
