import java.util.*;

public class A{
	int[][][] visit;
	int[] width;
	int[] src;
	int[] srcDrct;
	int[] dst;
	int[] dstDrct;
	int answer;
	
	private final static int[][] DRCT = new int[][]{
		new int[]{ 1,  0,  0},
		new int[]{-1,  0,  0},
		new int[]{ 0,  1,  0},
		new int[]{ 0, -1,  0},
		new int[]{ 0,  0,  1},
		new int[]{ 0,  0, -1},
	};
	private final static String[] DRCT_TYPE = new String[]{
		"+x", "-x", "+y", "-y", "+z", "-z"
	};
	private final static int[] ELBOW_LENGTH1 = new int[]{2, 1};
	private final static int[] ELBOW_LENGTH2 = new int[]{2, 3};
	
	public static void main(String[] args) {
		Scanner s = new Scanner(System.in);
		
		int caseNumber = 1;
		while(true) {
			String line = s.nextLine();
			if(line.equals("0"))break;
			
			A a = new A(line);
			System.out.printf("Case %d: %s\n", caseNumber++, a.solve(line));
		}
	}
	
	private static int[] getDrct(String str) {
		for(int i = 0; i < DRCT_TYPE.length; i++) {
			if(str.equals(DRCT_TYPE[i])) return DRCT[i];
		}
		return null;
	}
	
	A(String line) {
		Scanner s = new Scanner(line);
		width = new int[]{s.nextInt(), s.nextInt(), s.nextInt()};
		src = new int[]{s.nextInt()-1, s.nextInt()-1, s.nextInt()-1};
		srcDrct = getDrct(s.next());
		dst = new int[]{s.nextInt()-1, s.nextInt()-1, s.nextInt()-1};
		dstDrct = getDrct(s.next());
	}
	
	private String solve(String line) {
		visit = new int[width[0]][width[1]][width[2]];
		for(int[][] v1 : visit)for(int[] v2 : v1)Arrays.fill(v2, 0);
		
		answer = 7;
		solveDfs(src, srcDrct, 1);
		if(answer == 7)return "Impossible";
		return "" + answer;
	}
	
	private static boolean parallel(int[] d1, int[] d2) {
		for(int i = 0; i < 3; i++) {
			if(d1[i] != 0 && d2[i] != 0)return true;
		}
		return false;
	}
	
	//return false    if position is outside table
	//             or if visitTable is added addValue the visitTable value equals 2
	//return true
	private boolean addVisitTable(int[] position, int addValue) {
		for(int i = 0; i < 3; i++) {
			if(position[i] < 0 || width[i] <= position[i])return false;
		}
		visit[position[0]][position[1]][position[2]] += addValue;
		return visit[position[0]][position[1]][position[2]] != 2;
	}
	
	private void solveDfs(int[] pos, int[] posDrct, int count) {
//		System.out.printf("%d : (%d %d %d), (%d %d %d)\n", count, pos[0], pos[1], pos[2], posDrct[0], posDrct[1], posDrct[2]);
		if(count >= answer) {
			return;
		}
		for(int i = 0; i < 2; i++){
			int elbowLen1 = ELBOW_LENGTH1[i];
			int elbowLen2 = ELBOW_LENGTH2[i];
			boolean conflict1 = false;
			for(int  j = 0; j < elbowLen1; j++) {
				conflict1 |= !addVisitTable(pos, +1);
				pos[0] += posDrct[0];
				pos[1] += posDrct[1];
				pos[2] += posDrct[2];
			}
//		System.out.printf("a %d : (%d %d %d), (%d %d %d)\n", count, pos[0], pos[1], pos[2], posDrct[0], posDrct[1], posDrct[2]);
			
			if(!conflict1) {
				for(int[] d : DRCT) {
					if(parallel(d, posDrct))continue;
					boolean conflict2 = false;
					for(int j = 0; j < elbowLen2; j++) {
						conflict2 |= !addVisitTable(pos, +1);
						pos[0] += d[0];
						pos[1] += d[1];
						pos[2] += d[2];
					}
//		System.out.printf("%s %d : (%d %d %d), (%d %d %d)\n", conflict2, count, pos[0], pos[1], pos[2], d[0], d[1], d[2]);
					if(!conflict2) {
						if(pos[0]-d[0] == dst[0] && d[0] == dstDrct[0] && 
								pos[1]-d[1] == dst[1] && d[1] == dstDrct[1] && 
								pos[2]-d[2] == dst[2] && d[2] == dstDrct[2]){
							answer = count;
						} else {
							solveDfs(pos, d, count+1);
						}
					}
					
					for(int j = 0; j < elbowLen2; j++) {
						pos[0] -= d[0];
						pos[1] -= d[1];
						pos[2] -= d[2];
						addVisitTable(pos, -1);
					}
				}
			}
			
			for(int  j = 0; j < elbowLen1; j++) {
				pos[0] -= posDrct[0];
				pos[1] -= posDrct[1];
				pos[2] -= posDrct[2];
				addVisitTable(pos, -1);
			}
		}
	}
	
}
