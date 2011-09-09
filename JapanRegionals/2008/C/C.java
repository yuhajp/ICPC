import java.util.*;

public class C{
	public static void main(String[] args) {
		Scanner s = new Scanner(System.in);
		
		while(true) {
			int n = s.nextInt();
			if(n == 0)break;
			
			int[] exist = new int[12 * 60 * 60];
			Arrays.fill(exist, 0);
			for(int i = 0; i < n; i++) {
				for(int time : calculateTimeList(new int[]{s.nextInt(), s.nextInt(), s.nextInt()})) {
					exist[time] |= 1 << i;
				}
			}
			int mask = (1 << n) - 1;
			int beginAnswer = 0;
			int endAnswer = 0;
			int lengthAnswer = 43200;
			for(int b = 0; b < 12*60*60; b++) {
				if(exist[b] == 0)continue;
				int m = 0;
				for(int t = 0; t < lengthAnswer; t++) {
					int e = (b+t) % 43200;
					m |= exist[e];
					if(m == mask) {
						beginAnswer = b;
						endAnswer = e;
						lengthAnswer = t;
						break;
					}
				}
			}
			printTime(beginAnswer);
			System.out.print(" ");
			printTime(endAnswer);
			System.out.print("\n");
		}
	}
	
	private static void printTime(int t) {
		int s = t % 60;
		t /= 60;
		int m = t % 60;
		t /= 60;
		int h = t;
		System.out.printf("%02d:%02d:%02d", h, m, s);
	}
	
	private static ArrayList<Integer> calculateTimeList(int[] input) {
		ArrayList<Integer> output = new ArrayList<Integer>();
		output.addAll(calculateTimeList(input[0], input[1], input[2]));
		output.addAll(calculateTimeList(input[0], input[2], input[1]));
		output.addAll(calculateTimeList(input[1], input[0], input[2]));
		output.addAll(calculateTimeList(input[1], input[2], input[0]));
		output.addAll(calculateTimeList(input[2], input[0], input[1]));
		output.addAll(calculateTimeList(input[2], input[1], input[0]));
		return output;
	}
	
	private static ArrayList<Integer> calculateTimeList(int h, int m, int s) {
		ArrayList<Integer> output  = new ArrayList<Integer>();
		for(int i = 0; i < 60; i++){
			if(h % 5 == m / 12) {
				output.add((h/5)*3600 + m*60 + s);
			}
			h++;
			h %= 60;
			m++;
			m %= 60;
			s++;
			s %= 60;
		}
		return output;
	}
}
