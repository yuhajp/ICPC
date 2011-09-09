import java.util.*;

public class E{
	private final static double INF = 1e+10;
	private final static double EPS = 1e-8;
	
	public static void main(String[] args) {
		Scanner s = new Scanner(System.in);
		
		while(true) {
			int n = s.nextInt();
			if(n == 0)break;
			
			Vec3 p = new Vec3(0, 0, 0);
			Vec3 v = new Vec3(s.nextDouble(), s.nextDouble(), s.nextDouble());
			double[] cr = new double[n];
			Vec3[] c = new Vec3[n];
			for(int i = 0; i < n; i++) {
				c[i] = new Vec3(s.nextDouble(), s.nextDouble(), s.nextDouble());
				cr[i] = s.nextDouble();
			}
			while(true) {
				ArrayList<DoubleAndIndex> cross = new ArrayList<DoubleAndIndex>();
				for(int i = 0; i < n; i++) {
					DoubleAndIndex di = new DoubleAndIndex(crossPoint(v, p.sub(c[i]), cr[i]),i);
					cross.add(di);
				}
				Collections.sort(cross);
				DoubleAndIndex di = cross.get(0);
				double t = di.getDouble();
				if(t == INF)break;
				int index = di.getIndex();
				Vec3 nextP = p.add(v.scale(t));
				Vec3 tempA = nextP.sub(c[index]).scale(2.0);
				Vec3 tempB = nextP.sub(p);
				double alpha = crossPoint(tempA, tempB, tempB.abs());
				Vec3 nextV = tempA.scale(alpha).add(tempB);
				
				p = nextP;
				v = nextV;
			}
			p.print();
		}
	}
	
	//solve |tv + p| = r
	//return positive and minumum t
	//if nothing return INF
	private static double crossPoint(Vec3 v, Vec3 p, double r) {
		double a2 = v.norm();
		double a1 = 2.0*v.innerProduct(p);
		double a0 = p.norm() - r*r;
		
		a1 /= 2.0;
		double d = a1*a1 - a2*a0;
		if(d < EPS)return INF;
		double t1 = (-a1+Math.sqrt(d))/a2;
		if(t1 < EPS)t1 = INF;
		double t2 = (-a1-Math.sqrt(d))/a2;
		if(t2 < EPS)t2 = INF;
		return Math.min(t1, t2);
	}
}


class Vec3{
	private double x, y, z;
	public Vec3(double _x, double _y, double _z){
		x = _x;
		y = _y;
		z = _z;
	}
	public Vec3 add(Vec3 v) {
		return new Vec3(this.x+v.x, this.y+v.y, this.z+v.z);
	}
	public Vec3 sub(Vec3 v) {
		return new Vec3(this.x-v.x, this.y-v.y, this.z-v.z);
	}
	public Vec3 scale(double s) {
		return new Vec3(s*x, s*y, s*z);
	}
	public double norm() {
		return innerProduct(this);
	}
	public double abs() {
		return Math.sqrt(norm());
	}
	public double innerProduct(Vec3 v) {
		return this.x*v.x + this.y*v.y + this.z*v.z;
	}
	public void print() {
		System.out.printf("%.3f %.3f %.3f\n", x, y, z);
	}
}

class DoubleAndIndex implements Comparable<DoubleAndIndex>{
	private Double d;
	private int index;
	
	DoubleAndIndex(Double _d, int _index) {
		d = _d;
		index = _index;
	}
	public int compareTo(DoubleAndIndex di) {
		return this.d.compareTo(di.d);
	}
	public double getDouble() {
		return d;
	}
	public int getIndex() {
		return index;
	}
}
