import java.util.*;

public class B{
	public static void main(String[] args) {
		Scanner s = new Scanner(System.in);
		
		while(true) {
			int n = s.nextInt();
			int m = s.nextInt();
			int k = s.nextInt();
			if(n == 0 && m == 0 && k == 0)break;
			
			int[] dpBefore = new int[n*m+1];
			Arrays.fill(dpBefore, 0);
			dpBefore[0] = 1;
			for(int i = 0; i < n; i++) {
				int[] dpAfter = new int[n*m+1];
				Arrays.fill(dpAfter, 0);
				for(int j = i; j <= m*i; j++) {
					for(int a = 1; a <= m; a++) {
						dpAfter[j+a] += dpBefore[j];
					}
				}
				dpBefore = dpAfter;
			}
			long sum = 0;
			for(int i = n; i <= n*m; i++) {
				sum += Math.max(1, i - k) * dpBefore[i];
			}
			System.out.printf("%.8f\n", sum / Math.pow(m, n));
		}
	}
}
