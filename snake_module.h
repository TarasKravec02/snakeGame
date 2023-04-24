#ifndef SNAKE_MODULE_H
#define SNAKE_MODULE_H

struct snake_node{
	int x, y;
	struct snake_node *next_snake_node;
};

struct snake_struct{
	struct snake_node *head, *tail;
	int dx, dy;
};

void init_snake(struct snake_struct *s, int row, int col);
void hide_snake(struct snake_struct s);
void show_snake(struct snake_struct s);
void move_snake(struct snake_struct s);
void grow_snake(struct snake_struct *s);
void set_snake_direction(struct snake_struct *s, int x, int y);

#endif