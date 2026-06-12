#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define WIDTH 80
#define HEIGHT 25
#define MAX_SHAPES 100

// ================= SHAPE DATA STRUCTURES =================

typedef enum {
    SHAPE_LINE,
    SHAPE_RECTANGLE,
    SHAPE_CIRCLE,
    SHAPE_TRIANGLE
} ShapeType;

typedef struct {
    int x1, y1;
    int x2, y2;
} LineParams;

typedef struct {
    int x, y; // Top-left corner
    int width, height;
} RectParams;

typedef struct {
    int cx, cy; // Center
    int radius;
} CircleParams;

typedef struct {
    int x1, y1;
    int x2, y2;
    int x3, y3;
} TriangleParams;

typedef struct {
    int id;
    ShapeType type;
    union {
        LineParams line;
        RectParams rect;
        CircleParams circle;
        TriangleParams triangle;
    } params;
} Shape;

// Global shape list
Shape shapes[MAX_SHAPES];
int num_shapes = 0;
int next_shape_id = 1;

char canvas[HEIGHT][WIDTH];

// ================= SHAPE MANAGEMENT =================

int add_line(int x1, int y1, int x2, int y2) {
    if (num_shapes >= MAX_SHAPES) {
        return -1;
    }
    Shape s;
    s.id = next_shape_id++;
    s.type = SHAPE_LINE;
    s.params.line.x1 = x1;
    s.params.line.y1 = y1;
    s.params.line.x2 = x2;
    s.params.line.y2 = y2;
    shapes[num_shapes++] = s;
    return s.id;
}

int add_rectangle(int x, int y, int width, int height) {
    if (num_shapes >= MAX_SHAPES) {
        return -1;
    }
    Shape s;
    s.id = next_shape_id++;
    s.type = SHAPE_RECTANGLE;
    s.params.rect.x = x;
    s.params.rect.y = y;
    s.params.rect.width = width;
    s.params.rect.height = height;
    shapes[num_shapes++] = s;
    return s.id;
}

int add_circle(int cx, int cy, int radius) {
    if (num_shapes >= MAX_SHAPES) {
        return -1;
    }
    Shape s;
    s.id = next_shape_id++;
    s.type = SHAPE_CIRCLE;
    s.params.circle.cx = cx;
    s.params.circle.cy = cy;
    s.params.circle.radius = radius;
    shapes[num_shapes++] = s;
    return s.id;
}

int add_triangle(int x1, int y1, int x2, int y2, int x3, int y3) {
    if (num_shapes >= MAX_SHAPES) {
        return -1;
    }
    Shape s;
    s.id = next_shape_id++;
    s.type = SHAPE_TRIANGLE;
    s.params.triangle.x1 = x1;
    s.params.triangle.y1 = y1;
    s.params.triangle.x2 = x2;
    s.params.triangle.y2 = y2;
    s.params.triangle.x3 = x3;
    s.params.triangle.y3 = y3;
    shapes[num_shapes++] = s;
    return s.id;
}

int delete_shape(int id) {
    int index = -1;
    for (int i = 0; i < num_shapes; i++) {
        if (shapes[i].id == id) {
            index = i;
            break;
        }
    }
    if (index == -1) {
        return 0;
    }
    for (int i = index; i < num_shapes - 1; i++) {
        shapes[i] = shapes[i + 1];
    }
    num_shapes--;
    return 1;
}

Shape* find_shape(int id) {
    for (int i = 0; i < num_shapes; i++) {
        if (shapes[i].id == id) {
            return &shapes[i];
        }
    }
    return NULL;
}

// ================= SHAPE DRAWING ALGORITHMS =================

static void draw_pixel(char canvas[HEIGHT][WIDTH], int x, int y) {
    if (x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT) {
        canvas[y][x] = '*';
    }
}

void draw_line(char canvas[HEIGHT][WIDTH], int x1, int y1, int x2, int y2) {
    int dx = abs(x2 - x1);
    int dy = -abs(y2 - y1);
    int sx = x1 < x2 ? 1 : -1;
    int sy = y1 < y2 ? 1 : -1;
    int err = dx + dy;
    
    while (1) {
        draw_pixel(canvas, x1, y1);
        if (x1 == x2 && y1 == y2) break;
        int e2 = 2 * err;
        if (e2 >= dy) {
            err += dy;
            x1 += sx;
        }
        if (e2 <= dx) {
            err += dx;
            y1 += sy;
        }
    }
}

void draw_rectangle(char canvas[HEIGHT][WIDTH], int x, int y, int width, int height) {
    if (width <= 0 || height <= 0) return;

    // Top and bottom horizontal edges
    for (int i = x; i < x + width; i++) {
        draw_pixel(canvas, i, y);
        draw_pixel(canvas, i, y + height - 1);
    }
    // Left and right vertical edges
    for (int j = y; j < y + height; j++) {
        draw_pixel(canvas, x, j);
        draw_pixel(canvas, x + width - 1, j);
    }
}

void draw_circle(char canvas[HEIGHT][WIDTH], int cx, int cy, int radius) {
    if (radius < 0) return;
    int x = 0;
    int y = radius;
    int d = 3 - 2 * radius;
    
    while (y >= x) {
        draw_pixel(canvas, cx + x, cy + y);
        draw_pixel(canvas, cx - x, cy + y);
        draw_pixel(canvas, cx + x, cy - y);
        draw_pixel(canvas, cx - x, cy - y);
        draw_pixel(canvas, cx + y, cy + x);
        draw_pixel(canvas, cx - y, cy + x);
        draw_pixel(canvas, cx + y, cy - x);
        draw_pixel(canvas, cx - y, cy - x);
        
        x++;
        if (d > 0) {
            y--;
            d = d + 4 * (x - y) + 10;
        } else {
            d = d + 4 * x + 6;
        }
    }
}

void draw_triangle(char canvas[HEIGHT][WIDTH], int x1, int y1, int x2, int y2, int x3, int y3) {
    draw_line(canvas, x1, y1, x2, y2);
    draw_line(canvas, x2, y2, x3, y3);
    draw_line(canvas, x3, y3, x1, y1);
}

// ================= CANVAS ENGINE =================

void canvas_clear(char canvas[HEIGHT][WIDTH]) {
    for (int r = 0; r < HEIGHT; r++) {
        for (int c = 0; c < WIDTH; c++) {
            canvas[r][c] = '_';
        }
    }
}

void canvas_draw_shapes(char canvas[HEIGHT][WIDTH]) {
    for (int i = 0; i < num_shapes; i++) {
        Shape s = shapes[i];
        switch (s.type) {
            case SHAPE_LINE:
                draw_line(canvas, s.params.line.x1, s.params.line.y1, 
                                  s.params.line.x2, s.params.line.y2);
                break;
            case SHAPE_RECTANGLE:
                draw_rectangle(canvas, s.params.rect.x, s.params.rect.y, 
                                       s.params.rect.width, s.params.rect.height);
                break;
            case SHAPE_CIRCLE:
                draw_circle(canvas, s.params.circle.cx, s.params.circle.cy, 
                                     s.params.circle.radius);
                break;
            case SHAPE_TRIANGLE:
                draw_triangle(canvas, s.params.triangle.x1, s.params.triangle.y1, 
                                       s.params.triangle.x2, s.params.triangle.y2, 
                                       s.params.triangle.x3, s.params.triangle.y3);
                break;
        }
    }
}

void canvas_display(char canvas[HEIGHT][WIDTH]) {
    // Tens header line
    printf("     ");
    for (int col = 0; col < WIDTH; col++) {
        if (col % 10 == 0) {
            printf("%d", col / 10);
        } else {
            printf(" ");
        }
    }
    printf("\n");

    // Units header line
    printf("     ");
    for (int col = 0; col < WIDTH; col++) {
        printf("%d", col % 10);
    }
    printf("\n");

    // Top border
    printf("    +");
    for (int col = 0; col < WIDTH; col++) {
        printf("-");
    }
    printf("+\n");

    // Canvas rows
    for (int r = 0; r < HEIGHT; r++) {
        printf(" %02d |", r);
        for (int c = 0; c < WIDTH; c++) {
            printf("%c", canvas[r][c]);
        }
        printf("|\n");
    }

    // Bottom border
    printf("    +");
    for (int col = 0; col < WIDTH; col++) {
        printf("-");
    }
    printf("+\n");
}

// ================= USER INTERFACE FLOWS =================

void clear_screen() {
#ifdef _WIN32
    system("cls");
#else
    printf("\033[H\033[J");
    fflush(stdout);
#endif
}

int read_int(const char* prompt, int* val) {
    printf("%s", prompt);
    char buf[100];
    if (fgets(buf, sizeof(buf), stdin) == NULL) {
        return 0;
    }
    if (sscanf(buf, "%d", val) != 1) {
        return 0;
    }
    return 1;
}

int validate_x(int x) {
    return (x >= 0 && x < WIDTH);
}

int validate_y(int y) {
    return (y >= 0 && y < HEIGHT);
}

void list_active_shapes() {
    if (num_shapes == 0) {
        printf("\nNo active shapes in the picture.\n");
        return;
    }
    printf("\n=== Active Shapes List ===\n");
    for (int i = 0; i < num_shapes; i++) {
        Shape s = shapes[i];
        printf("ID [%d] - ", s.id);
        switch (s.type) {
            case SHAPE_LINE:
                printf("Line from (%d, %d) to (%d, %d)\n", 
                       s.params.line.x1, s.params.line.y1, 
                       s.params.line.x2, s.params.line.y2);
                break;
            case SHAPE_RECTANGLE:
                printf("Rectangle: top-left=(%d, %d), width=%d, height=%d\n", 
                       s.params.rect.x, s.params.rect.y, 
                       s.params.rect.width, s.params.rect.height);
                break;
            case SHAPE_CIRCLE:
                printf("Circle: center=(%d, %d), radius=%d\n", 
                       s.params.circle.cx, s.params.circle.cy, 
                       s.params.circle.radius);
                break;
            case SHAPE_TRIANGLE:
                printf("Triangle: vertices (%d, %d), (%d, %d), (%d, %d)\n", 
                       s.params.triangle.x1, s.params.triangle.y1, 
                       s.params.triangle.x2, s.params.triangle.y2, 
                       s.params.triangle.x3, s.params.triangle.y3);
                break;
        }
    }
    printf("==========================\n");
}

void add_shape_flow() {
    printf("\n--- Add a New Shape ---\n");
    printf("1. Line\n");
    printf("2. Rectangle\n");
    printf("3. Circle\n");
    printf("4. Triangle\n");
    printf("5. Back to Main Menu\n");
    
    int choice;
    if (!read_int("Select shape type: ", &choice)) {
        printf("Invalid input. Press Enter to continue...\n");
        char dummy[100];
        fgets(dummy, sizeof(dummy), stdin);
        return;
    }

    switch (choice) {
        case 1: {
            int x1, y1, x2, y2;
            if (!read_int("Enter X1 (0-79): ", &x1) || !validate_x(x1)) { printf("Invalid X1 coordinate.\n"); break; }
            if (!read_int("Enter Y1 (0-24): ", &y1) || !validate_y(y1)) { printf("Invalid Y1 coordinate.\n"); break; }
            if (!read_int("Enter X2 (0-79): ", &x2) || !validate_x(x2)) { printf("Invalid X2 coordinate.\n"); break; }
            if (!read_int("Enter Y2 (0-24): ", &y2) || !validate_y(y2)) { printf("Invalid Y2 coordinate.\n"); break; }
            
            int id = add_line(x1, y1, x2, y2);
            if (id != -1) printf("Successfully added Line with ID %d!\n", id);
            else printf("Error: Maximum shapes capacity reached.\n");
            break;
        }
        case 2: {
            int x, y, w, h;
            if (!read_int("Enter top-left X (0-79): ", &x) || !validate_x(x)) { printf("Invalid X coordinate.\n"); break; }
            if (!read_int("Enter top-left Y (0-24): ", &y) || !validate_y(y)) { printf("Invalid Y coordinate.\n"); break; }
            if (!read_int("Enter width (positive): ", &w) || w <= 0) { printf("Invalid width (must be > 0).\n"); break; }
            if (!read_int("Enter height (positive): ", &h) || h <= 0) { printf("Invalid height (must be > 0).\n"); break; }
            
            int id = add_rectangle(x, y, w, h);
            if (id != -1) printf("Successfully added Rectangle with ID %d!\n", id);
            else printf("Error: Maximum shapes capacity reached.\n");
            break;
        }
        case 3: {
            int cx, cy, r;
            if (!read_int("Enter center X (0-79): ", &cx) || !validate_x(cx)) { printf("Invalid center X coordinate.\n"); break; }
            if (!read_int("Enter center Y (0-24): ", &cy) || !validate_y(cy)) { printf("Invalid center Y coordinate.\n"); break; }
            if (!read_int("Enter radius (positive): ", &r) || r < 0) { printf("Invalid radius (must be >= 0).\n"); break; }
            
            int id = add_circle(cx, cy, r);
            if (id != -1) printf("Successfully added Circle with ID %d!\n", id);
            else printf("Error: Maximum shapes capacity reached.\n");
            break;
        }
        case 4: {
            int x1, y1, x2, y2, x3, y3;
            if (!read_int("Enter X1 (0-79): ", &x1) || !validate_x(x1)) { printf("Invalid X1 coordinate.\n"); break; }
            if (!read_int("Enter Y1 (0-24): ", &y1) || !validate_y(y1)) { printf("Invalid Y1 coordinate.\n"); break; }
            if (!read_int("Enter X2 (0-79): ", &x2) || !validate_x(x2)) { printf("Invalid X2 coordinate.\n"); break; }
            if (!read_int("Enter Y2 (0-24): ", &y2) || !validate_y(y2)) { printf("Invalid Y2 coordinate.\n"); break; }
            if (!read_int("Enter X3 (0-79): ", &x3) || !validate_x(x3)) { printf("Invalid X3 coordinate.\n"); break; }
            if (!read_int("Enter Y3 (0-24): ", &y3) || !validate_y(y3)) { printf("Invalid Y3 coordinate.\n"); break; }
            
            int id = add_triangle(x1, y1, x2, y2, x3, y3);
            if (id != -1) printf("Successfully added Triangle with ID %d!\n", id);
            else printf("Error: Maximum shapes capacity reached.\n");
            break;
        }
        case 5:
            return;
        default:
            printf("Invalid choice.\n");
            break;
    }
    printf("Press Enter to continue...\n");
    char dummy[100];
    fgets(dummy, sizeof(dummy), stdin);
}

void modify_shape_flow() {
    list_active_shapes();
    if (num_shapes == 0) {
        printf("Press Enter to continue...\n");
        char dummy[100];
        fgets(dummy, sizeof(dummy), stdin);
        return;
    }
    
    int id;
    if (!read_int("Enter the ID of the shape to modify: ", &id)) {
        printf("Invalid input. Press Enter to continue...\n");
        char dummy[100];
        fgets(dummy, sizeof(dummy), stdin);
        return;
    }
    Shape* s = find_shape(id);
    if (!s) {
        printf("Shape with ID %d not found. Press Enter to continue...\n", id);
        char dummy[100];
        fgets(dummy, sizeof(dummy), stdin);
        return;
    }
    
    printf("\nModifying shape ID %d.\n", id);
    switch (s->type) {
        case SHAPE_LINE: {
            int x1, y1, x2, y2;
            printf("Current: Line from (%d, %d) to (%d, %d)\n", 
                   s->params.line.x1, s->params.line.y1, 
                   s->params.line.x2, s->params.line.y2);
            if (!read_int("Enter new X1 (0-79): ", &x1) || !validate_x(x1)) { printf("Invalid X1.\n"); break; }
            if (!read_int("Enter new Y1 (0-24): ", &y1) || !validate_y(y1)) { printf("Invalid Y1.\n"); break; }
            if (!read_int("Enter new X2 (0-79): ", &x2) || !validate_x(x2)) { printf("Invalid X2.\n"); break; }
            if (!read_int("Enter new Y2 (0-24): ", &y2) || !validate_y(y2)) { printf("Invalid Y2.\n"); break; }
            s->params.line.x1 = x1;
            s->params.line.y1 = y1;
            s->params.line.x2 = x2;
            s->params.line.y2 = y2;
            printf("Line modified successfully!\n");
            break;
        }
        case SHAPE_RECTANGLE: {
            int x, y, w, h;
            printf("Current: Rectangle: top-left=(%d, %d), width=%d, height=%d\n", 
                   s->params.rect.x, s->params.rect.y, 
                   s->params.rect.width, s->params.rect.height);
            if (!read_int("Enter new top-left X (0-79): ", &x) || !validate_x(x)) { printf("Invalid X.\n"); break; }
            if (!read_int("Enter new top-left Y (0-24): ", &y) || !validate_y(y)) { printf("Invalid Y.\n"); break; }
            if (!read_int("Enter new width (positive): ", &w) || w <= 0) { printf("Invalid width.\n"); break; }
            if (!read_int("Enter new height (positive): ", &h) || h <= 0) { printf("Invalid height.\n"); break; }
            s->params.rect.x = x;
            s->params.rect.y = y;
            s->params.rect.width = w;
            s->params.rect.height = h;
            printf("Rectangle modified successfully!\n");
            break;
        }
        case SHAPE_CIRCLE: {
            int cx, cy, r;
            printf("Current: Circle: center=(%d, %d), radius=%d\n", 
                   s->params.circle.cx, s->params.circle.cy, 
                   s->params.circle.radius);
            if (!read_int("Enter new center X (0-79): ", &cx) || !validate_x(cx)) { printf("Invalid X.\n"); break; }
            if (!read_int("Enter new center Y (0-24): ", &cy) || !validate_y(cy)) { printf("Invalid Y.\n"); break; }
            if (!read_int("Enter new radius (positive): ", &r) || r < 0) { printf("Invalid radius.\n"); break; }
            s->params.circle.cx = cx;
            s->params.circle.cy = cy;
            s->params.circle.radius = r;
            printf("Circle modified successfully!\n");
            break;
        }
        case SHAPE_TRIANGLE: {
            int x1, y1, x2, y2, x3, y3;
            printf("Current: Triangle: vertices (%d, %d), (%d, %d), (%d, %d)\n", 
                   s->params.triangle.x1, s->params.triangle.y1, 
                   s->params.triangle.x2, s->params.triangle.y2, 
                   s->params.triangle.x3, s->params.triangle.y3);
            if (!read_int("Enter new X1 (0-79): ", &x1) || !validate_x(x1)) { printf("Invalid X1.\n"); break; }
            if (!read_int("Enter new Y1 (0-24): ", &y1) || !validate_y(y1)) { printf("Invalid Y1.\n"); break; }
            if (!read_int("Enter new X2 (0-79): ", &x2) || !validate_x(x2)) { printf("Invalid X2.\n"); break; }
            if (!read_int("Enter new Y2 (0-24): ", &y2) || !validate_y(y2)) { printf("Invalid Y2.\n"); break; }
            if (!read_int("Enter new X3 (0-79): ", &x3) || !validate_x(x3)) { printf("Invalid X3.\n"); break; }
            if (!read_int("Enter new Y3 (0-24): ", &y3) || !validate_y(y3)) { printf("Invalid Y3.\n"); break; }
            s->params.triangle.x1 = x1;
            s->params.triangle.y1 = y1;
            s->params.triangle.x2 = x2;
            s->params.triangle.y2 = y2;
            s->params.triangle.x3 = x3;
            s->params.triangle.y3 = y3;
            printf("Triangle modified successfully!\n");
            break;
        }
    }
    printf("Press Enter to continue...\n");
    char dummy[100];
    fgets(dummy, sizeof(dummy), stdin);
}

void delete_shape_flow() {
    list_active_shapes();
    if (num_shapes == 0) {
        printf("Press Enter to continue...\n");
        char dummy[100];
        fgets(dummy, sizeof(dummy), stdin);
        return;
    }
    
    int id;
    if (!read_int("Enter the ID of the shape to delete: ", &id)) {
        printf("Invalid input. Press Enter to continue...\n");
        char dummy[100];
        fgets(dummy, sizeof(dummy), stdin);
        return;
    }
    
    if (delete_shape(id)) {
        printf("Shape ID %d deleted successfully!\n", id);
    } else {
        printf("Shape with ID %d not found.\n", id);
    }
    printf("Press Enter to continue...\n");
    char dummy[100];
    fgets(dummy, sizeof(dummy), stdin);
}

int main() {
    int choice;
    while (1) {
        clear_screen();
        
        canvas_clear(canvas);
        canvas_draw_shapes(canvas);
        canvas_display(canvas);
        
        printf("\n====== 2D Graphics Editor Menu ======\n");
        printf("1. Add a Shape (Line, Rect, Circle, Triangle)\n");
        printf("2. Modify a Shape\n");
        printf("3. Delete a Shape\n");
        printf("4. Clear All Shapes\n");
        printf("5. Exit\n");
        printf("=====================================\n");
        
        if (!read_int("Select an option: ", &choice)) {
            printf("Invalid input. Press Enter to try again...\n");
            char dummy[100];
            fgets(dummy, sizeof(dummy), stdin);
            continue;
        }
        
        if (choice == 5) {
            printf("Exiting... Goodbye!\n");
            break;
        }
        
        switch (choice) {
            case 1:
                add_shape_flow();
                break;
            case 2:
                modify_shape_flow();
                break;
            case 3:
                delete_shape_flow();
                break;
            case 4:
                num_shapes = 0;
                printf("All shapes cleared. Press Enter to continue...\n");
                char dummy[100];
                fgets(dummy, sizeof(dummy), stdin);
                break;
            default:
                printf("Invalid choice. Press Enter to try again...\n");
                char dummy2[100];
                fgets(dummy2, sizeof(dummy2), stdin);
                break;
        }
    }
    return 0;
}
