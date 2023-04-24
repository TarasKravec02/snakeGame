#include <stdlib.h>
#include <ncurses.h>

struct snake_node{
	int x, y;
	struct snake_node *next_snake_node;
};

struct snake_struct{
	struct snake_node *head, *tail;
	int dx, dy;
};

void show_snake(struct snake_struct s)
{
	struct snake_node *tmp;
	tmp = s.tail;
	while(tmp){
		move(tmp->y, tmp->x);
		addch('*');
		tmp = tmp->next_snake_node;
	}
	refresh();
}

void init_snake(struct snake_struct *s, int row, int col)
{
	s->dx = 0;
	s->dy = 0;
	s->tail = malloc(sizeof(struct snake_node));
	s->tail->x = col / 2;
	s->tail->y = row / 2;
	s->tail->next_snake_node = NULL;
	s->head = s->tail;
	show_snake(*s);
}

void hide_snake(struct snake_struct s)
{
	struct snake_node *tmp;
	tmp = s.tail;
	while(tmp){
		move(tmp->y, tmp->x);
		addch(' ');
		tmp = tmp->next_snake_node;
	}
}

void move_snake(struct snake_struct s)
{	
	if(s.dx == 0 && s.dy == 0)
		return;
	struct snake_node *tmp;
	hide_snake(s);
	tmp = s.tail;
	while(tmp->next_snake_node){
		tmp->x = tmp->next_snake_node->x;
		tmp->y = tmp->next_snake_node->y;
		tmp = tmp->next_snake_node;
	}
	s.head->x += s.dx;
	s.head->y += s.dy;
	show_snake(s);
}

void grow_snake(struct snake_struct *s)
{
	struct snake_node *tmp;
	tmp = malloc(sizeof(struct snake_node));
	tmp->next_snake_node = s->tail;
	tmp->x = s->tail->x;
	tmp->y = s->tail->y;
	s->tail = tmp;
}

void set_snake_direction(struct snake_struct *s, int x, int y)
{
	s->dx = x;
	s->dy = y;
}