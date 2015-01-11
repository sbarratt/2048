//2048.c - 2048 (C style)
//Author: Shane Barratt, 1/9/2015

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <stdlib.h>
#include <limits.h>
#include <math.h>
#include <float.h>

typedef int64_t int64;

uint32_t d[65536];

#define UP 0
#define DOWN 1
#define LEFT 2
#define RIGHT 3

/*

0000000000000000
0000000000000000
0000000000000000
0000000000000000

*/

int get_col(int64 b, int c) {
	int r = (b >> (c*4)) & 0xF;
	r |= ((b >> (c*4+16)) & 0xF) << 4;
	r |= ((b >> (c*4+32)) & 0xF) << 8;
	r |= ((b >> (c*4+48)) & 0xF) << 12;
	return r;
}

int get_row(int64 b, int r) {
	return (b >> r*16) & 0xFFFF;
}

int64 board_from_rows(char row_0[], char row_1[], char row_2[], char row_3[]) {
	int64 b = 0;
	for (int i = 0; i < 4; i++) {
		b |= (int64)row_0[i] << i*4;
	}
	for (int i = 0; i < 4; i++) {
		b |= (int64)row_1[i] << (16+i*4);
	}
	for (int i = 0; i < 4; i++) {
		b |= (int64)row_2[i] << (32+i*4);
	}
	for (int i = 0; i < 4; i++) {
		b |= (int64)row_3[i] << (48+i*4);
	}
	return b;
}

void print_board(int64 b) {
	for(int i = 0; i < 4; i++) {
		printf("+-------------------------------+\n|       |       |       |       |\n");
		for(int j = 0; j < 4; j++) {
			if ((0xF & (int)(b >> (i*16+j*4))) == 0) {
				printf("|       ");				
			} else {
				printf("|   %d   ", 0xF & (int)(b >> (i*16+j*4)));				
			}
		}
		printf("|\n|       |       |       |       |\n");
	}
	printf("+-------------------------------+\n");
}

int reverse_fourbits(int num) {
	int new_num = 0;
	new_num |= (num&0xF000) >> 12;
	new_num |= (num&0x0F00) >> 4;
	new_num |= (num&0x00F0) << 4;
	new_num |= (num&0x000F) << 12;
	return new_num;
}

//TODO - move lookup table for moving the other direction.
int64 move_right(int64 b) {
	int r = 0;
	int64 new_b = 0;
	for(int i = 0; i < 4; i++) {
		r = get_row(b,i);
		new_b |= (int64)reverse_fourbits(d[reverse_fourbits(r)]) << i*16;
	}
	return new_b;
}

int64 move_left(int64 b) {
	int r = 0;
	int64 new_b = 0;
	for(int i = 0; i < 4; i++) {
		r = get_row(b,i);
		new_b |= (int64)d[r] << i*16;
	}
	return new_b;
}

int64 move_up(int64 b) {
	int c = 0;
	int64 new_b = 0;
	for(int i = 0; i < 4; i++) {
		c = get_col(b,i);
		int new_col = d[c];

		new_b |= (int64)((new_col & 0xF000) >> 12) << (48+ i*4);
		new_b |= (int64)((new_col & 0x0F00) >> 8) << (32+i*4);
		new_b |= (int64)((new_col & 0x00F0) >> 4) << (16+i*4);
		new_b |= (int64)((new_col & 0x000F)) << (0+i*4);
	}
	return new_b;
}

//TODO - alternate move lookup table
int64 move_down(int64 b) {
	int c = 0;
	int64 new_b = 0;
	for(int i = 0; i < 4; i++) {
		c = get_col(b,i);
		int new_col = reverse_fourbits(d[reverse_fourbits(c)]);

		new_b |= (int64)((new_col & 0xF000) >> 12) << (48+i*4);
		new_b |= (int64)((new_col & 0x0F00) >> 8) << (32+i*4);
		new_b |= (int64)((new_col & 0x00F0) >> 4) << (16+i*4);
		new_b |= (int64)((new_col & 0x000F)) << (0+i*4);
	}
	return new_b;
}

int64 rand_piece(int64 b, int seed_offset) {
	srand(time(NULL) + seed_offset);
	int p = 1;
	if (rand()%10 == 0) {
		p = 2;
	}
	int zero_count = 0;
	for(int i = 0; i < 16; ++i) {
		zero_count += (((b >> 4*i) & 0xF) == 0);
	}
	int zero_places[zero_count];
	int j = 0;
	for(int i = 0; i < 16; i++) {
		if (((b >> 4*i) & 0xF) == 0) {
			zero_places[j++] = 4*i;
		}
	}
	if (zero_count == 0) {
		printf("no zeros\n");
		return b;
	}
	else {
		int r = rand()%zero_count;
		return b | ((int64)p << zero_places[r]);
	}
}

int64 makemovenum(int64 b, int move) {
	if (move == UP) {
		return move_up(b);
	}
	else if (move == RIGHT) {
		return move_right(b);
	}
	else if (move == DOWN) {
		return move_down(b);
	}
	else {
		return move_left(b);
	}
}

int64 makemove(int64 b, char move) {
	if (move == 'w') {
		return move_up(b);
	}
	else if (move == 'd') {
		return move_right(b);
	}
	else if (move == 's') {
		return move_down(b);
	}
	else {
		return move_left(b);
	}
}

void playgame() {
	int64 b = 0;
	b = rand_piece(b,0);	
	b = rand_piece(b,1);

	// char row_0[4] = {1,2,3,4};
	// char row_1[4] = {2,1,4,3};
	// char row_2[4] = {1,2,3,4};
	// char row_3[4] = {2,1,4,3};
	// b = board_from_rows(row_0,row_1,row_2,row_3);

	char row_0[4] = {3,4,0,0};
	char row_1[4] = {3,4,0,1};
	char row_2[4] = {7,0,0,0};
	char row_3[4] = {2,0,0,0};
	b = board_from_rows(row_0,row_1,row_2,row_3);
	
	printf("\n");
	print_board(b);

	getchar();
	int move_ct = 0;
	while (1) {
		char move;
		printf("Make Move:");
		scanf("%c",&move);
		int skip_move = 0;
		int64 newb = 0;
		if (move == EOF) {
			printf("\n");
			exit(0);
		}
		else if (move == 'w' || move == 'd' || move == 's' || move == 'a') {
			newb = makemove(b,move);
			if (newb == b) {
				skip_move = 1;
			}
			else {
				b = newb;
			}
		}
		else {
			skip_move = 1;
		}
		if (skip_move == 0) {
			printf("\n\n");
			print_board(b);
			b = rand_piece(b, move_ct);
			++move_ct;
			printf("\n\n");
			print_board(b);
		}
	}
}

int getrandommove(int64 b, int seed) {
	srand(time(NULL) + seed);
	int r = rand() % 4;
	if (r == UP) {
		return 'w';
	}
	else if (r == DOWN) {
		return 's';
	}
	else if (r == RIGHT) {
		return 'd';
	}
	else {
		return 'a';
	}
}


//TODO - better eval function
int eval(int64 b) {
	int s = 0;
	for(int i = 0; i < 16; i++) {
		s += (((b >> (i*4)) & 0xF) == 0);
	}
	int toprow = get_row(b, 0);
	int bottomrow = get_row(b, 3);
	int leftcol = get_col(b, 0);
	int rightcol = get_col(b, 3);

	int tr = toprow & 0xF;
	int br = bottomrow & 0xF;
	int lc = leftcol & 0xF;
	int rc = rightcol & 0xF;
	int ct = 0;
	for(int i = 1; i < 4; i++) {
		if (((toprow >> i*4) & 0xF) > tr) {
			// ct++;
		} 
		if (((bottomrow >> i*4) & 0xF) > br) {
			ct++;
		}
		if (((leftcol >> i*4) & 0xF) > lc) {
			// ct++;
		} 
		if (((rightcol >> i*4) & 0xF) > rc) {
			// ct++;
		} 
		tr = ((toprow >> i*4) & 0xF);
		br = ((bottomrow >> i*4) & 0xF);
		lc = ((leftcol >> i*4) & 0xF);
		rc = ((rightcol >> i*4) & 0xF);
	}


	return s*2 + ct;
}

//TODO - incorporate 4's in expectimax (right now it only does 2's for speed).
double expectimax(int64 b, int depth, int maximizingPlayer) {
	if (depth == 0) {
		return eval(b);
	}
	if (maximizingPlayer) {
		double bestValue = -1;
		int64 newb;
		for(int i = 0; i < 4; ++i) {
			newb = makemovenum(b,i);
			if (newb != b) {
				double newValue = expectimax(newb,depth-1,1-maximizingPlayer);
				if (newValue > bestValue) {
					bestValue = newValue;
				}
			}
		}
		return bestValue;
	}
	else {
		double s = 0;
		int64 newb;
		double ct = 0;
		for(int i = 0; i < 16; ++i) {
			if (((b >> i*4) & 0xF) == 0) {
				newb = b | ((int64)1 << i*4);
				s += expectimax(newb, depth-1, 1-maximizingPlayer);
				ct++;
			}
		}
		return s*1.0/ct;
	}
}

int getmove(int64 b, int seed) {
	int best_move = -1;
	double best_score = -1;
	for(int i = 0; i < 4; i++) {
		int64 newb = makemovenum(b,i);
		double competing_score = expectimax(newb,7,0);
		if (competing_score >= best_score && newb != b) {
			best_move = i;
			best_score = competing_score;
		}
	}
	return best_move;
}

void aiplay() {
	int64 b = 0;
	b = rand_piece(b,0);	
	b = rand_piece(b,1);

	// char row_0[4] = {3,4,0,0};
	// char row_1[4] = {3,4,0,1};
	// char row_2[4] = {7,0,0,0};
	// char row_3[4] = {2,0,0,0};
	// b = board_from_rows(row_0,row_1,row_2,row_3);	

	printf("\n");
	print_board(b);

	int move_ct = 0;
	for(int i = 0; i < 2000; i++) {
		int skip_move = 0;
		int64 newb = makemovenum(b,getmove(b, i));
		if (newb == b){
			skip_move = 1;
		} else {
			b = newb;
		}
		if (skip_move == 0) {
			b = rand_piece(b, move_ct);
			++move_ct;
		}
		printf("\n\n");
		print_board(b);
		// getchar();
	}
}

int main() {
	//fill move table

	for(int i = 0; i < 65536; i++) {
		char rc[4];
		rc[3] = (i >> 12) & 0xF;
		rc[2] = (i >> 8)  & 0xF;
		rc[1] = (i >> 4)  & 0xF;
		rc[0] = i 		  & 0xF;

		for(int z = 0; z < 3; z++) {
			for(int j = 0; j < 3; j++) {
				if (rc[j] == 0 && rc[j+1] != 0) {
					rc[j] = rc[j+1];
					rc[j+1] = 0;
				}
			}
		}

		if (rc[0] == rc[1] && rc[0] != 0) {
			rc[0] += 1;
			rc[1] = rc[2];
			rc[2] = rc[3];
			rc[3] = 0;
		}

		if (rc[1] == rc[2] && rc[2] != 0) {
			rc[1] += 1;
			rc[2] = rc[3];
			rc[3] = 0;
		}

		if (rc[2] == rc[3] && rc[3] != 0) {
			rc[2] += 1;
			rc[3] = 0;
		}

		d[i] = (rc[3] << 12) | (rc[2] << 8) | (rc[1] << 4) | (rc[0]);
	}

	//game logic
	// char row_0[4] = {2,2,1,1};
	// char row_1[4] = {1,2,1,1};
	// char row_2[4] = {2,1,3,4};
	// char row_3[4] = {1,2,4,6};
	// int64 b = board_from_rows(row_0,row_1,row_2,row_3);
	aiplay();
	// playgame();
}