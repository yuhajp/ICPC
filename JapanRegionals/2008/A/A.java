import java.util.*;

public class A{
	public static void main(String args[]) {
		Scanner s = new Scanner(System.in);
		
		int[] count = new int[20];
		while(true) {
			int n = s.nextInt();
			int w = s.nextInt();
			if(n == 0)break;
			
			Arrays.fill(count, 0);
			int countMax = 0;
			int indexMax = 0;
			for(int i = 0; i < n; i++) {
				int index = s.nextInt() / w;
				indexMax = Math.max(indexMax, index);
				count[index]++;
				countMax = Math.max(countMax, count[index]);
			}
			
			double sum = 0.01;
			for(int i = 0; i <= indexMax; i++){
				sum += (double)(count[i] * (indexMax - i)) / (countMax * indexMax);
			}
			System.out.println(sum);
		}
	}
}
