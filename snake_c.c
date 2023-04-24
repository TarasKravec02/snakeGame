#include <ncurses.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include "snake_module.h"

enum {key_space = 32, key_escape = 27, key_save_progress = 's'};

const char msg_game_over[] = "Game Over";
const char msg_save_progress[] = "Do you want to save a progress? y\\n ";
const char msg_file_name[] = "Enter a file name: ";
const char msg_get_progress[] = "Get progress?: ";

struct fruit_struct{
	int x, y;
};

void make_new_fruit(struct fruit_struct *f, int row, int col)
{
	f->x = rand()%(col-2)+1;
	f->y = rand()%(row-2)+1;
	move(f->y, f->x);
	addch('*');
	refresh();
}

void init_field(int row, int col)
{
	int i;
	move(0, 0);
	for(i = 0; i < col; i++)
		addch('#');
	move(row -1, 0);
	for(i = 0; i < col; i++)
		addch('#');
	move(0, 0);
	for(i = 0; i < row; i++){
		move(i, 0);
		addch('#');
	}
	move(0, col-1);
	for(i = 0; i < row; i++){
		move(i, col-1);
		addch('#');
	}
	refresh();
}

int is_collision(struct snake_struct s, int row, int col)
{
    struct snake_node *s_ptr;
    s_ptr = s.tail;
    while(s_ptr->next_snake_node){
        if((s.head->x == s_ptr->x) && (s.head->y == s_ptr->y))
            return 1;
        s_ptr = s_ptr->next_snake_node;
    }
	return (s.head->x < 1) || (s.head->y < 1) ||
			(s.head->x > col-2) || (s.head->y > row-2);
}

int eating_fruit(struct snake_struct s, struct fruit_struct f)
{
	int is = (s.head->x == f.x) && (s.head->y == f.y);
	return is;
}

void game_over(int row, int col)
{
	clear();
	refresh();
	move(row/2, (col-sizeof(msg_game_over))/2);
	addstr(msg_game_over);
	refresh();
}

void save_snake_node(FILE *f, struct snake_node *s_ptr){
	if(s_ptr == NULL){
		return;
	}
	save_snake_node(f, s_ptr->next_snake_node);
	fwrite(&(s_ptr->x), sizeof(s_ptr->x), 1, f);
	fwrite(&(s_ptr->y), sizeof(s_ptr->y), 1, f);
	return;
}

void save_progress(struct snake_struct s){
	int code;
	code = getchar();
	printf("%d", code);
	switch(code){
	case 'y':
		FILE *f;
		char f_name[20];
		printf("%s", msg_file_name);
		scanf("%19s", f_name);
		f = fopen(f_name, "w");
		if(!f){
			perror(f_name);
			exit(1);
		}
		save_snake_node(f, s.tail);
		return;
	case 'n':
		return;
	}
}

int get_progress(struct snake_struct *s){
	printf("%s\n", msg_get_progress);
	int code;
	code = getchar();
	getchar();
	switch(code){
	case 'y':
		FILE *f;
		char f_name[20];
		printf("Specify the name: ");
		scanf("%19s", f_name);
		f = fopen(f_name, "r");
		if(!f){
			perror(f_name);
			exit(2);
		}
		s->dx = 0;
		s->dy = 0;
		s->tail = NULL;
		s->head = NULL;
		while(!feof(f)){
			struct snake_node *p;
			p = malloc(sizeof(struct snake_node));
			fread(&(p->x), sizeof(p->x), 1, f);
			fread(&(p->y), sizeof(p->y), 1, f);
			p->next_snake_node = s->tail;
			s->tail = p;
		}
		struct snake_node *p = s->tail;
		while(p){
			s->head = p;
			p = p->next_snake_node;
		}
		return 1;
	case 'n':
		return 0;
	}
	return 0;
}

int main()
{
	int progress;
	struct snake_struct my_snake;
	struct fruit_struct my_fruit;
	progress = (get_progress(&my_snake));
	int row, col;
	int key_code;
	initscr();
	cbreak();
	noecho();
	keypad(stdscr, 1);
	timeout(0);
	curs_set(0);
	srand(time(NULL));
	getmaxyx(stdscr, row, col);
	init_field(row, col);
	if(!progress){
		init_snake(&my_snake, row, col);
	} else{
		show_snake(my_snake);
		refresh();
	}
	make_new_fruit(&my_fruit, row, col);
	while((key_code = getch())) {
		switch(key_code){
		case ERR:
			move_snake(my_snake);
			if(is_collision(my_snake, row, col))
				goto leave_cycle;
			if(eating_fruit(my_snake, my_fruit)){
				grow_snake(&my_snake);
				make_new_fruit(&my_fruit, row, col);
			}
			usleep(130000);
			continue;
		case KEY_UP:
			set_snake_direction(&my_snake, 0, -1);
			break;
		case KEY_DOWN:
			set_snake_direction(&my_snake, 0, 1);
			break;
		case KEY_LEFT:
			set_snake_direction(&my_snake, -1, 0);
			break;
		case KEY_RIGHT:
			set_snake_direction(&my_snake, 1, 0);
			break;
		case key_space:
			set_snake_direction(&my_snake, 0, 0);
			break;
		case key_save_progress:
			clear();
			refresh();
			endwin();
			printf("%s", msg_save_progress);
			save_progress(my_snake);
			return 0;
		case key_escape:
			goto leave_cycle;
		}
	}
leave_cycle:
	game_over(row, col);
	sleep(1);
	endwin();
	return 0;
}
