import java.util.*;

public class G{
	private static boolean hasElement(char[] element, char[] text, int offset) {
		for(int i = 0; i < element.length; i++) {
			if(element[i] != text[offset + i])return false;
		}
		return true;
	}
	
	public static void main(String[] args) {
		Scanner s = new Scanner(System.in);
		
		int[][] table = new int[1 << 12][13];
		for(int mask = 1; mask < (1 << 12); mask++){
			int count = 0;
			for(int j = 0; j < 12; j++) {
				if((mask & (1 << j)) == 0)continue;
				table[mask][count] = j;
				count++;
			}
			table[mask][count] = -1;
		}
		
		while(true) {
			int n = s.nextInt();
			int m = s.nextInt();
			if(n == 0 && m == 0)break;
			
			char[][] e = new char[n][];
			for(int i = 0; i < n; i++) {
				e[i] = s.next().toCharArray();
			}
			
			String t = "";
			for(int i = 0; i < m; i++) {
				t += s.next();
			}
			int textLength = t.length();
			t += "********************";
			char[] text = t.toCharArray();
			
			boolean[][] dptable = new boolean[textLength + 20][1 << n];
			for(int i = textLength; i < textLength + 20; i++) {
				for(int j = 0; j < (1 << n); j++) {
					dptable[i][j] = false;
				}
			}
			for(int i = 0; i < textLength + 20; i++) {
				dptable[i][0] = true;
			}
			
			int count = 0;
			for(int i = textLength - 1; i >= 0; i--) {
				boolean[] temp = new boolean[n];
				for(int j = 0; j < n; j++) {
					temp[j] = hasElement(e[j], text, i);
				}
				for(int mask = 1; mask < (1 << n); mask++) {
					for(int x = 0; table[mask][x] >= 0; x++) {
						int j = table[mask][x];
						dptable[i][mask] = temp[j] && dptable[i + e[j].length][mask - (1 << j)];
						if(dptable[i][mask])break;
					}
				}
				if(dptable[i][(1<<n) - 1])count++;
			}
			System.out.printf("%d\n", count);
		}
	}
}
