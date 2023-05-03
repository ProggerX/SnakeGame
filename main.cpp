#include<iostream>
#include<vector>
#include<unistd.h>
#include<chrono>
#include<thread>
#include<algorithm>
#include<ncurses.h>
using namespace std;

class Food
{
	public:
		int x, y;
		void new_pos(vector<pair<int, int>> snake_arr, int score)
		{
			auto epoch = chrono::high_resolution_clock::from_time_t(0);
			auto now = chrono::high_resolution_clock::now();
			auto millis = chrono::duration_cast<chrono::milliseconds>(now-epoch).count();
			x = (score * millis) % 30;
			y = ((score-2) * (millis+1)) % 30;
			pair<int, int> p = {x, y};
			while(true)
			{
				auto epoch = chrono::high_resolution_clock::from_time_t(0);
				auto now = chrono::high_resolution_clock::now();
				auto millis = chrono::duration_cast<chrono::milliseconds>(now-epoch).count();
				x = (score * millis) % 30;
				y = ((score-2) * (millis+1)) % 30;
				pair<int, int> p = {x, y};
				bool tobreak = true;
				for(auto i : snake_arr)
					if(i == p)
						tobreak = false;
				if (tobreak) break;
			}
		}
		
};

class Snake
{
private:
	pair<int, int> head = {3, 0};
	vector<pair<int, int>> body = {{3, 0}, {2, 0}, {1, 0}, {0, 0}};
public:
	void operator() (int x, int y)
	{
		for(int i = body.size() - 1; i > 0; i--)
		{
			body[i] = body[i - 1];
		}
		pair<int, int> pair_of_coords = {x + head.first, y + head.second};
		head = pair_of_coords;
		body[0] = head;
	}
	vector<pair<int, int>> get_vec()
	{
		return body;
	}
	void add_element()
	{
		pair<int, int> p = body[body.size() - 1];
		body.push_back(p);
	}
};

int main()
{
	initscr();
	curs_set(0);
	nodelay(stdscr, true);
	//keypad(stdscr, true);

	Snake snake;
	/*vector<vector<char>> screen(31, vector<char>(31, '.'));*/
	double time = 0.3;
	int score = 0;
	string direction = "down";
	Food food;
	food.new_pos(snake.get_vec(), 10);
	while(true)
	{	
		char ch = getch();
		if(ch == 'q')
		{
			break;
		}
		else if(ch == 'w' && direction != "down")
		{
			direction = "up";
		}
		else if(ch == 's' && direction != "up")
		{
			direction = "down";
		}
		else if(ch == 'a' && direction != "right")
		{
			direction = "left";
		}
		else if(ch == 'd' && direction != "left")
		{
			direction = "right";
		}
		/*for(int i = 0; i < 30; i++)
		{
			for(int j = 0; j < 30; j++)
			{
				screen[i][j] = '.';
			}
		}*/
		//system("clear");
		clear();
		mvwprintw(stdscr, 31, 0, "------------------------------------------------------------");
		for(int y = 0; y < 31; y++)
		{
			mvwprintw(stdscr, y, 31 * 2, "|");
		}
		if(direction == "right")
		{
			snake(1, 0);
		}
		else if(direction == "down")
		{
			snake(0, 1);
		}
		else if(direction == "left")
		{
			snake(-1, 0);
		}
		else if(direction == "up")
		{
			snake(0, -1);
		}
		/*for(int i = 0; i < snake.get_vec().size(); i++)
		{
			screen[snake.get_vec()[i].second][snake.get_vec()[i].first] = '#';
		}
		/*for(int i = 0; i < 31; i++)
		{
			for(int j = 0; j < 31; j++)
			{
				//cout << screen[i][j];
				char a = screen[i][j];
				const char* b = &a;
				//printw("%s", b);
				mvwprintw(stdscr, i, j, "%s", b);
			}
			cout << '\n';
		}*/
		pair<int, int> pair_food = {food.x, food.y};
		if(snake.get_vec()[0] == pair_food)
		{
			snake.add_element();
			food.new_pos(snake.get_vec(), score);
			score += 10;
		}
		for(auto i : snake.get_vec())
		{
			mvwprintw(stdscr, i.second, i.first * 2, "#");
		}
		bool to_break = false;
		for(int i = 1; i < snake.get_vec().size(); i++)
		{
			if (snake.get_vec()[i] == snake.get_vec()[0])
			{
				to_break = true;
			}
		}
		if(to_break)
		{
			break;
		}
		pair<int, int> head = snake.get_vec()[0];
		if(head.first > 30 || head.first < 0 || head.second > 30 || head.second < 0)
		{
			break;
		}
		mvwprintw(stdscr, food.y, food.x * 2, "@");
		mvwprintw(stdscr, 35, 0, "score: %d, speed: %f", score, time);
		move(30, 50);
		refresh();
		score++;
		if(score % 100 == 0)
		{
			time -= 0.2 * time;
		}
		usleep(time * 1000000);
	}
	nodelay(stdscr, false);
	clear();
	mvwprintw(stdscr, 15, 25, "Game over!");
	getch();
	endwin();
}
