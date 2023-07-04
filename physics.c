#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <raylib.h>

typedef struct {
    Vector2 center;
    Vector2 velocity;
    int radius;
    bool used;
} Object;

const int maxRadius = 20;
const int minRadius = 2;
const int screenWidth = 800;
const int screenHeight = 600;
float deltaTime;
int collectionSize = 1;
int item = 0;
Vector2 gravity = { 0.0f, 300.0f };
Object *objCol;

Object *createObject();
float distance(Vector2 obj1, Vector2 obj2);
void resolveOutOfBounds();
void resolveBoundCollision();
void resolveCollision();
void resolveMovement();
void update();

Object *createObject() {
    int randomRadius = (rand() % (maxRadius - minRadius + 1)) + minRadius;
    Object *object = malloc(sizeof(Object));
    object->center = GetMousePosition();
    object->velocity.x = 15.0f;
    object->velocity.y = 0.0f;
    object->radius = randomRadius;
    object->used = true;
    return object;
}

float distance(Vector2 obj1, Vector2 obj2) {
    return sqrtf(powf(obj2.x - obj1.x, 2.0f) + powf(obj2.y - obj1.y, 2.0f));
}

void resolveOutOfBounds() {
    for (int i = 0; i < collectionSize - 1; i++) {
        if (objCol[i].used && 
        (objCol[i].center.x - objCol[i].radius > screenWidth || 
        objCol[i].center.x + objCol[i].radius < 0 || 
        objCol[i].center.y - objCol[i].radius > screenHeight || 
        objCol[i].center.y + objCol[i].radius < 0)) {
            objCol[i].used = false;
            #ifdef DEBUG
            printf("Obj %d out of bounds\n", i);
            #endif
            objCol[i].center.x = 0;
            objCol[i].center.y = 0;
        }
    }
}

void resolveBoundCollision() {
    for (int i = 0; i < collectionSize - 1; i++) {
        if (objCol[i].used) {
            if (objCol[i].center.y + objCol[i].radius >= screenHeight) {
                // objCol[i].center.y = screenHeight - objCol[i].radius;
                objCol[i].velocity.y -= 100;
                #ifdef DEBUG
                printf("Obj %d collided with the bottom bound\n", i);
                #endif
            }

            if (objCol[i].center.x + objCol[i].radius >= screenWidth) {
                objCol[i].velocity.x -= 100;
                #ifdef DEBUG
                printf("Obj %d collided with the right bounds\n", i);
                #endif
            }

            if (objCol[i].center.x - objCol[i].radius <= 0) {
                objCol[i].velocity.x += 100;
                #ifdef DEBUG
                printf("Obj %d collided with the left bounds\n", i);
                #endif
            }
        }
    }
}

void resolveCollision() {

}

void resolveMovement() {
    for (int i = 0; i < collectionSize - 1; i++) {
        if (objCol[i].used) {
            objCol[i].center.x += objCol[i].velocity.x * deltaTime;
            objCol[i].center.y += objCol[i].velocity.y * deltaTime;

            // reduce velocity with gravity velocity
            if (objCol[i].center.y + objCol[i].radius < screenHeight - objCol[i].radius) {
            objCol[i].velocity.x += gravity.x * deltaTime;
            objCol[i].velocity.y += gravity.y * deltaTime;
            }
        }
    }
}

void update() {
    // resolveOutOfBounds();
    resolveBoundCollision();
    resolveMovement();
    resolveCollision();
}

int main(void) {
    srand(time(NULL));
    InitWindow(screenWidth, screenHeight, "Physics Testing");

    objCol = malloc(sizeof(Object));
    
    SetTargetFPS(60);
    while (!WindowShouldClose()) {
        deltaTime = GetFrameTime();

        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        // if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
            objCol[item] = *createObject();
            #ifdef DEBUG
            printf("Obj %d x%f, y%f\n", item, objCol[item].center.x, objCol[item].center.y);
            #endif

            item++;
            collectionSize++;

            Object *newObj;
            newObj = realloc(objCol, sizeof(Object) * collectionSize);
            #ifdef DEBUG
            if (newObj == NULL) {
                printf("Failed to reallocate memory\n");
            }
            #endif
            objCol = newObj;
        }

        update();

        BeginDrawing();

        ClearBackground(BLACK);

        for (int i = 0; i < collectionSize - 1; i++) {
            if (objCol[i].used) {
                DrawCircleV(objCol[i].center, objCol[i].radius, WHITE);
            }
        }

        EndDrawing();
    }

    free(objCol);
    #ifdef DEBUG
    printf("Object collection size: %d, freed\n", collectionSize);
    #endif

    CloseWindow();
    return 0;
}
