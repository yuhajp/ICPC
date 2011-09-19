/*
xhl_kogitsune

Create  Tue Nov 11 13:22:37     2008
Compile Tue Nov 11 13:43:53     2008
Accept  Tue Nov 11 14:10:37     2008
*/
#include<iostream>
#include<stdio.h>
#include<string.h>
#include<string>
using namespace std;
string sub[10]; // 部分文字列
int sublen[10]; // 部分文字列の長さ
long long DP[2][1024*10*10];
int transition[10][10][26][10];

char DPrev[26][1024*10*10];
int table[1024][10][10][26];

void solve( int N, int M, int p, int done, int lid, int llen, char *buf )
{
	if( p == N ){
		if( done == (1<<M)-1 ){
			buf[p] = 0;
			printf( "%s\n", buf );
		}
		return;
	}

	// 望みのない枝を枝刈り
	if( DPrev[p][done*100+lid*10+llen] == 0 ) return;

	if( done == (1<<M)-1 ){
		for( int c = 'a'; c <= 'z'; c ++ ){
			buf[p] = c;
			solve( N, M, p+1, done, lid, llen, buf );
		}
	}
	else{
		for( int c = 'a'; c <= 'z'; c ++ ){
			buf[p] = c;
			int nextstate = table[done][lid][llen][c-'a'];
			int nextdone = nextstate / 100;
			int nextlid  = nextstate / 10 % 10;
			int nextllen = nextstate % 10;
			solve( N, M, p+1, nextdone, nextlid, nextllen, buf );
		}
	}
}

int main( void )
{
	for(int C = 1;; C ++ ){
		// 入力を読む
		int N, M;
		cin >> N >> M;
		if( (N|M) == 0 ) break;
		for( int i = 0; i < M; i ++ ){
			cin >> sub[i];
			sublen[i] = sub[i].size();
		}

		// transition[X][l_x][c-'a'][j]: $X, l_x$のステートの時に次に文字$c$が来た場合の$l_j$の値
		// KMP使えばもっとスマートに書ける気はします
		for( int lid = 0; lid < M; lid ++ ){
			for( int llen = 0; llen < sublen[lid]; llen ++ ){
				for( int c = 'a'; c <= 'z'; c ++ ){
					for( int j = 0; j < M; j ++ ){
						int l = 0;
						for( int k = 1; k <= sublen[j] && k <= llen+1; k ++ ){
							if( c == sub[j][k-1] && memcmp( sub[j].c_str(), sub[lid].c_str()+llen+1-k, k-1 ) == 0 )
								l = k;
						}
						transition[lid][llen][c-'a'][j] = l;
					}
				}
			}
		}

		//	ステート遷移を前計算
		//  table[done][X][l_x][次に来る文字-'a'] = 次のステート番号(下に書いてあるDPの添字参照)
		for( int done = (1<<M) - 1; done >= 0; -- done ){
			for( int lid = M - 1; lid >= 0; -- lid ){
				if( done & (1<<lid) ) continue;
				for( int llen = sublen[lid] - 1; llen >= 0; -- llen ){
					for( int c = 25; c >= 0; -- c ){
						int done2 = done;
						int lid2 = -1;
						int llen2 = -1;
						int *trans = transition[lid][llen][c];
						for( int j = M - 1; j >= 0; -- j ){
							if( done & (1<<j) ) continue;
							int jlen = trans[j];
							if( jlen >= sublen[j] ) done2 |= 1<<j;
							else if( llen2 < jlen ) llen2 = jlen, lid2 = j;
						}
						if( lid2 < 0 ){ lid2 = llen2 = 0; }
						table[done][lid][llen][c] = done2*100+lid2*10+llen2;
					}
				}
			}
		}

		// 可能なパスワードが何通りあるか計算する
		// DP[p][p文字目までに完全に含まれる部分文字列の部分集合(10bitで表現) * 100 + X * 10 + l_x] = 可能なパスワードの数
		// 実装の都合で1次元化
		// 全ての部分文字列が完全に含まれる場合は X = l_x = 0 とする
		memset( DP[0], 0x00, sizeof(DP[0]) );
		DP[0][0] = 1;
		for( int p = 0; p < N; p ++ ){
			memset( DP[(p+1)&1], 0x00, sizeof(DP[(p+1)&1]) );
			for( int done = 0; done < (1<<M); done ++ ){
				for( int lid = 0; lid < M; lid ++ ){
					if( done & (1<<lid) ) continue;
					for( int llen = 0; llen < sublen[lid]; llen ++ ){
						long long d0 = DP[p&1][done*100+lid*10+llen];
						if( d0 == 0 ) continue;
						for( int c = 0; c < 26; c ++ ){
							DP[(p+1)&1][table[done][lid][llen][c]] += d0;
						}
					}
				}
			}
			DP[(p+1)&1][((1<<M)-1)*100] += DP[p&1][((1<<M)-1)*100] * 26;
		}

		long long ans = DP[N&1][((1<<M)-1)*100];
		cout << "Case " << C << ": " << ans << " suspects" << endl;
		if( ans <= 42 ){
		// 可能なパスワードを列挙
		// DPrev[p][done*100+X*10+l_x]: このステートの後文字を追加すれば最終的に可能なパスワードまで到達すれば1
		// 最後にsolve内でDPrevを利用して枝刈りしつつパスワードを列挙する
			memset( DPrev, 0x00, sizeof(DPrev) );
			DPrev[N][((1<<M)-1)*100] = 1;

			for( int p = N-1; p >= 0; -- p ){
				for( int done = (1<<M) - 1; done >= 0; -- done ){
					for( int lid = M - 1; lid >= 0; -- lid ){
						if( done & (1<<lid) ) continue;
						for( int llen = sublen[lid] - 1; llen >= 0; -- llen ){
							for( int c = 25; c >= 0; -- c ){
								if( DPrev[p+1][table[done][lid][llen][c]] )
									DPrev[p][done*100+lid*10+llen] = 1;
							}
						}
					}
				}

				if( DPrev[p+1][((1<<M)-1)*100] )
					DPrev[p][((1<<M)-1)*100] = 1;
			}
			char buf[1024];
			solve( N, M, 0, 0, 0, 0, buf );
		}
	}
	return 0;
}
