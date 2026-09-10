#define PI 3.14159265358979323846
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "../utils.h"

struct point
{
    int x;
    int y;
};
typedef struct point point_t;

struct rectangle
{
    point_t upper_left;
    point_t lower_right;
};
typedef struct rectangle rectangle_t;

struct circle
{
    point_t center;
    int radius;
};
typedef struct circle circle_t;

void translate(point_t *p1, point_t *p2)
{
    p1->x += p2->x;
    p1->y += p2->y;
}

point_t make_point(int x, int y)
{
    point_t p = {.x = x, .y = y};
    return p;
}

void print_point(point_t *p)
{
    printf("point(%d, %d)", p->x, p->y);
}

rectangle_t make_rect(int x1, int y1, int x2, int y2)
{
    point_t u_l = make_point(x1, y1);
    point_t l_r = make_point(x2, y2);
    rectangle_t rect = {.upper_left = u_l, .lower_right = l_r};
    return rect;
}

void print_rect(rectangle_t *rect)
{
    point_t *u_l = &rect->upper_left;
    point_t *l_r = &rect->lower_right;
    printf("rectangle(upper_left=");
    print_point(u_l);
    printf(", lower_right=");
    print_point(l_r);
    printf("\n");
}

int area_rect(rectangle_t *rect)
{
    int bas = rect->lower_right.x - rect->upper_left.x;
    int höjd = rect->lower_right.y -rect->upper_left.y;
    int area = bas * höjd;
    return area; 
    }

bool intersects_rect(rectangle_t *rect1, rectangle_t *rect2)
{
    int r1_x1 = rect1->upper_left.x;
    int r1_x2 = rect1->lower_right.x;
    int r1_y1 = rect1->upper_left.y;
    int r1_y2 = rect1->lower_right.y;
    int r2_x1 = rect2->upper_left.x;
    int r2_x2 = rect2->lower_right.x;
    int r2_y1 = rect2->upper_left.y;
    int r2_y2 = rect2->lower_right.y;
 
    bool x_check = !((r2_x1 > r1_x2) || (r2_x2 < r1_x1));
    bool y_check = !((r2_y1 > r1_y2) || (r2_y2 < r1_y1));
    if (x_check && y_check) 
    {
        return true;
    } else {return false;}
}

rectangle_t intersection_rect(rectangle_t *rect1, rectangle_t *rect2) 
{
    int r1_x1 = rect1->upper_left.x;
    int r1_x2 = rect1->lower_right.x;
    int r1_y1 = rect1->upper_left.y;
    int r1_y2 = rect1->lower_right.y;
    int r2_x1 = rect2->upper_left.x;
    int r2_x2 = rect2->lower_right.x;
    int r2_y1 = rect2->upper_left.y;
    int r2_y2 = rect2->lower_right.y;

    int new_x1 = r1_x1 > r2_x1 ? r1_x1 : r2_x1;
    int new_x2 = r1_x2 < r2_x2 ? r1_x2 : r2_x2;
    int new_y1 = r1_y1 > r2_y1 ? r1_y1 : r2_y1;
    int new_y2 = r1_y2 < r2_y2 ? r1_y2 : r2_y2;

    return make_rect(new_x1, new_y1, new_x2, new_y2);
}

circle_t make_circle(point_t center, int radius) 
{
    circle_t c= { .center = center, .radius = radius};
    return c;
}

void print_circle(circle_t *c) 
{
    printf("circle(center=)");
    print_point(&c->center);
    printf(", radius=%d)\n", c->radius);
}

double area_circle(circle_t *c)
{
    return PI * pow(c->radius, 2);
}
int main(void)
{
    //   point_t p = { .x = 10, .y = 7 };
    //   print_point(&p);
    //    point_t p1 = { .x = 1, .y = 2 };
    //    point_t p2 = { .x = 4, .y =  5};
    //    rectangle_t rec_test = { .upper_left = p1, .lower_right = p2};
    //    print_rect(&rec_test);¨
//rectangle_t rect = make_rect(0, 0, 10, 5);   // bas=10, höjd=5
//printf("area = %d\n", area_rect(&rect));      // förväntat: 50
//
//rectangle_t a = make_rect(0, 0, 10, 10);
//rectangle_t b = make_rect(5, 5, 15, 15);      // överlappar a
//rectangle_t c = make_rect(20, 20, 30, 30);    // överlappar INTE a
//printf("a-b intersects: %d\n", intersects_rect(&a, &b));  // förväntat: 1 (true)
//printf("a-c intersects: %d\n", intersects_rect(&a, &c));  // förväntat: 0 (false)
//rectangle_t a = make_rect(0, 0, 10, 10);
//rectangle_t b = make_rect(5, 5, 15, 15);
//
//rectangle_t snitt = intersection_rect(&a, &b);
//print_rect(&snitt);

circle_t c = make_circle(make_point(0, 0), 5);
print_circle(&c);
printf("area = %f\n", area_circle(&c));
}