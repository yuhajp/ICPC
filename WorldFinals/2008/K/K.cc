/*
xhl_kogitsune

Create  Tue Nov 11 12:34:41     2008
Compile Tue Nov 11 13:02:02     2008
Accept  Tue Nov 11 13:14:11     2008
*/
#include<iostream>
#include<queue>
using namespace std;
int costx[100][99], costy[99][100];
int getcost( int W, int H, int x0, int y0, int x1, int y1 )
{
	if( x1 < 0 || x1 >= W || y1 < 0 || y1 >= H )
		return 0;
	if( y0 == y1 ) return costx[y0][min(x0,x1)];
	else return costy[min(y0,y1)][x0];
}


class Pos{
public:
	enum Dir{
		RIGHT = 0,
		UP,
		LEFT,
		DOWN,
		STOP = 4
	};
	Pos(int x_,int y_,enum Dir dir_) : x(x_), y(y_), dir(dir_) {}
	bool operator<(const Pos &p) const{ return 0; }
	int x, y;
	enum Dir dir;
};
int dx[] = {1,0,-1,0};
int dy[] = {0,-1,0,1};

int main( void )
{
	for(int C = 1; ; C ++ ){
		int H, W, y1, x1, y2, x2;
		cin >> H >> W >> y1 >> x1 >> y2 >> x2;
		if( (H|W|y1|x1|y2|x2) == 0 ) break;
		-- y1, -- x1, -- y2, -- x2;
		for( int y = 0; y < H; y ++ ){
			for( int x = 0; x + 1 < W; x ++ )
				cin >> costx[y][x];
			if( y + 1 < H )
				for( int x = 0; x < W; x ++ )
					cin >> costy[y][x];
		}

	// ダイクストラ
		priority_queue< pair<int,Pos> > wl;
		wl.push( pair<int,Pos>(0, Pos(x1,y1,Pos::STOP)) );
		int D[100][100][5];
		int visited[100][100][5];
		memset( D, 0x7f, sizeof(D) );
		D[y1][x1][Pos::STOP] = 0;
		memset( visited, 0x00, sizeof(visited) );

#define go(X,Y,DIR,DIST) { int &md = D[Y][X][DIR]; if( md > (DIST) ){ md = (DIST); wl.push( pair<int,Pos>(-(DIST), Pos((X),(Y),(enum Pos::Dir)(DIR))) ); } }

	// ダイクストラメインループ
		while( !wl.empty() ){
			int d   = -wl.top().first;
			Pos pos = wl.top().second;
			wl.pop();
			if( visited[pos.y][pos.x][pos.dir] ) continue;
			visited[pos.y][pos.x][pos.dir] = 1;
			if( pos.x == x2 && pos.y == y2 && pos.dir == Pos::STOP ) break;
			if( pos.dir == Pos::STOP ){
				// 停止状態 -> 四方に行ける
				for( int newdir = 0; newdir < 4; newdir ++ ){
					int newx = pos.x+dx[newdir], newy = pos.y+dy[newdir];
					int v = getcost(W, H, pos.x, pos.y, newx, newy);
					if( v == 0 ) continue;
					go( newx, newy, Pos::STOP, d + v * 2 ); // 次の交差点で停止
					go( newx, newy, newdir,    d + v * 2 ); // 次の交差点で停止せず直進
				}
			}
			else{ // フルスピード直進 -> そのままの方向のみ
				int newx = pos.x+dx[pos.dir], newy = pos.y+dy[pos.dir];
				int v = getcost(W, H, pos.x, pos.y, newx, newy);
				if( v == 0 ) continue;
				go( newx, newy, pos.dir,   d + v     ); // 次の交差点で停止せず直進
				go( newx, newy, Pos::STOP, d + v * 2 ); // 次の交差点で停止
			}
		}

		cout << "Case " << C << ": ";
		if( visited[y2][x2][Pos::STOP] )
			cout << D[y2][x2][Pos::STOP];
		else
			cout << "Impossible";
		cout << endl;
	}
	return 0;
}
