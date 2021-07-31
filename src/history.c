#include <stdio.h>

#define BUF_SZ 16

static struct {
	size_t head;
	size_t tail;
	int full;

	size_t cur;

	struct {
		int x;
		int y;
	} buf[BUF_SZ];
} hist = {0};

static void print()
{
	if(!hist.full && hist.tail == hist.head) return;

	size_t i = hist.tail;
	do {
		printf("%s%zd: %d, %d\n", i == hist.cur ? "* " : "", i, hist.buf[i].x, hist.buf[i].y);
		i = (i+1) % BUF_SZ;
	} while(i != hist.head);
}

static void add(int x, int y)
{
	if(hist.full) hist.tail = (hist.tail+1) % BUF_SZ;

	hist.buf[hist.head].x = x;
	hist.buf[hist.head].y = y;

	hist.cur = hist.head;
	hist.head = (hist.head+1) % BUF_SZ;
	hist.full = hist.head == hist.tail;
}

static void truncate()
{
	if(!hist.full && hist.tail == hist.head) return;

	hist.head = (hist.cur+1) % BUF_SZ;
	hist.full = hist.tail == hist.head;
}

int hist_get(int *x, int *y)
{
	if(!hist.full && hist.tail == hist.head) return -1;

	*x = hist.buf[hist.cur].x;
	*y = hist.buf[hist.cur].y;
	return 0;
}

void hist_add(int x, int y)
{
	int cx, cy;

	if(!hist_get(&cx, &cy) && cx == x && cy == y) return; //dedup

	truncate();
	add(x, y);
}



void hist_prev()
{
	if(!hist.full && hist.tail == hist.head) return;

	if(hist.cur == hist.tail) return;
	hist.cur = hist.cur == 0 ? BUF_SZ-1 : hist.cur-1;
}

void hist_next()
{
	if(!hist.full && hist.tail == hist.head) return;

	size_t n = (hist.cur + 1) % BUF_SZ;
	if(n != hist.head)
		hist.cur = n;
}