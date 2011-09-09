import java.util.*;
import java.io.*;

public class Diff{
	public static void main(String[] args)throws IOException{
		Scanner s0 = new Scanner(new FileReader(args[0]));
		Scanner s1 = new Scanner(new FileReader(args[1]));
		
		int lineno = 1;
		while(s0.hasNextDouble() && s1.hasNextDouble()){
			double d0 = s0.nextDouble();
			double d1 = s1.nextDouble();
			if(Math.abs(d0 - d1) > 1e-5){
				System.out.println("lineno: " + lineno);
				System.out.println(d0);
				System.out.println(d1);
			}
			lineno++;
		}
		if(s0.hasNextDouble() || s1.hasNextDouble()){
			System.out.println("lineno error");
		}
	}
}
