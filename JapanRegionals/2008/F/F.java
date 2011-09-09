import java.util.*;

public class F{
	public static void main(String[] args) {
		Scanner s = new Scanner(System.in);
		
		while(true) {
			int w = s.nextInt();
			int h = s.nextInt();
			if(w == 0 && h == 0)break;
			char[][] map = new char[h+2][w+2];
			for(char[] m : map)Arrays.fill(m, 'k');
			for(int i = 1; i <= h; i++) {
				String line = s.next();
				for(int j = 1; j <= w; j++) {
					map[i][j] = line.charAt(j-1);
				}
			}
			char[] list = new char[6];
			String str = s.next();
			for(int i = 0; i < 6; i++) {
				list[i] = str.charAt(i);
			}
			
			int answer = Dice.solve(map, list);
			System.out.print(answer == -1 ? "unreachable" : "" + answer);
			System.out.print("\n");
		}
	}
}

class Dice{
	private final static int T = 0;
	private final static int B = 1;
	private final static int N = 2;
	private final static int S = 3;
	private final static int E = 4;
	private final static int W = 5;
	
	private final static int[] DY = new int[]{1, -1, 0,  0};
	private final static int[] DX = new int[]{0,  0, 1, -1};
	private final static int[][] ROTATE_TABLE = new int[][]{
		{N, S, B, T, E, W},
		{S, N, T, B, E, W},
		{W, E, N, S, T, B},
		{E, W, N, S, B, T},
	};
	
	private static char[][] map;
	private static char[] list;
	
	private int y;
	private int x;
	private char[] colors;
	private int listIndex;
	
	public static int solve(char[][] _map, char[] _list) {
		map = _map;
		list = _list;
		
		Dice start = null;
		EXIT_FIND_SHARP : for(int i = 0; i < map.length; i++) {
			for(int j = 0; j < map[i].length; j++) {
				if(map[i][j] == '#') {
					start = new Dice(i, j);
					map[i][j] = 'w';
					break EXIT_FIND_SHARP;
				}
			}
		}
		if(start == null)return -1;
		return solveBFS(start);
	}
	
	private static int solveBFS(Dice start) {
		HashSet<Dice> visit = new HashSet<Dice>();
		visit.add(start);
		ArrayList<Dice> queue = new ArrayList<Dice>();
		queue.add(start);
		int count = 0;
		while(queue.size() > 0) {
			ArrayList<Dice> nextQueue = new ArrayList<Dice>();
			for(Dice dice : queue) {
				if(dice.isGoal())return count;
				for(int t = 0; t < 4; t++) {
					Dice nextDice = dice.rotate(t);
					if(nextDice == null)continue;
					if(visit.contains(nextDice))continue;
					visit.add(nextDice);
					nextQueue.add(nextDice);
				}
			}
			count++;
			queue = nextQueue;
		}
		return -1;
	}
	
	private Dice(int _y, int _x) {
		y = _y;
		x = _x;
		colors = new char[]{'r', 'c', 'g', 'm', 'b', 'y'};
		listIndex = 0;
	}
	
	private Dice(int _y, int _x, char[] newColors, int _listIndex) {
		y = _y;
		x = _x;
		colors = newColors;
		listIndex = _listIndex;
	}
	
	private Dice rotate(int type) {
		int nextY = y + DY[type];
		int nextX = x + DX[type];
		if(map[nextY][nextX] == 'k')return null;
		char[] nextColors = new char[6];
		for(int i = 0; i < 6; i++) {
			nextColors[i] = colors[ROTATE_TABLE[type][i]];
		}
		if(map[nextY][nextX] == 'w') {
			return new Dice(nextY, nextX, nextColors, listIndex);
		} else if(map[nextY][nextX] == list[listIndex] && map[nextY][nextX] == nextColors[T]) {
			return new Dice(nextY, nextX, nextColors, listIndex+1);
		} else {
			return null;
		}
	}
	
	private boolean isGoal() {
		return listIndex == 6;
	}
	
	public int hashCode() {
		int output = 0;
		output += y;
		output *= 31;
		output += x;
		output *= 31;
		output += listIndex;
		output *= 31;
		for(int i = 0; i < 6; i++) {
			output += colors[i];
			output *= 31;
		}
		return output;
	}
	
	public boolean equals(Object obj) {
		if(obj instanceof Dice) {
			Dice d = (Dice)obj;
			if(this.y != d.y)return false;
			if(this.x != d.x)return false;
			if(this.listIndex != d.listIndex)return false;
			for(int i = 0; i < 6; i++) {
				if(this.colors[i] != d.colors[i])return false;
			}
			return true;
		}
		return false;
	}
}
