import java.util.*;

public class I {
	public static void main(String[] args) throws Exception {
		Scanner s = new Scanner(System.in);
		
		while(true) {
			String line1 = s.nextLine();
			if(line1.equals("."))return;
			
			String line2 = s.nextLine();
			ParseResult result1 = Polynomial.parseExpression(line1.toCharArray(), 0);
			ParseResult result2 = Polynomial.parseExpression(line2.toCharArray(), 0);
			Polynomial poly1 = result1.polynomial;
			Polynomial poly2 = result2.polynomial;
			Polynomial p = Polynomial.gcd(poly1, poly2);
			p = p.normalize();
			p.print();
		}
	}
}

/*
	NUMBER     := [0-9]
	PRIMARY    := NUMBER | x | (EXPRESSION)
	FACTOR     := PRIMARY | PRIMARY^NUMBER
	TERM       := FACTOR FACTOR*
	EXPRESSION := -? TERM ([+-] TERM)*
*/

class ParseResult {
	public Polynomial polynomial;
	public int index;
	
	public ParseResult(Polynomial _p, int _i) {
		polynomial = _p;
		index = _i;
	}
}

class Polynomial {
	private final static int MAX_N = 10;
	int[] values;
	
	Polynomial() {
		values = new int[MAX_N];
		Arrays.fill(values, 0);
	}
	
	Polynomial(Polynomial p) {
		this.values = new int[MAX_N];
		for(int i = 0; i < MAX_N; i++) {
			this.values[i] = p.values[i];
		}
	}
	
	public void setValue(int degree, int value) {
		values[degree] = value;
	}
	
	public int getValue(int degree) {
		return values[degree];
	}
	
	public Polynomial multiply(Polynomial p) {
		Polynomial output = new Polynomial();
		for(int i = 0; i < MAX_N; i++) {
			int sum = 0;
			for(int j = 0; j <= i; j++) {
				sum += this.getValue(j) * p.getValue(i-j);
			}
			output.setValue(i, sum);
		}
		return output;
	}
	
	public Polynomial add(Polynomial p) {
		Polynomial output = new Polynomial();
		for(int i = 0; i < MAX_N; i++) {
			output.setValue(i, this.getValue(i) + p.getValue(i));
		}
		return output;
	}
	
	public Polynomial sub(Polynomial p) {
		Polynomial output = new Polynomial();
		for(int i = 0; i < MAX_N; i++) {
			output.setValue(i, this.getValue(i) - p.getValue(i));
		}
		return output;
	}
	
	public Polynomial power(int number) {
		Polynomial p = new Polynomial();
		p.setValue(0, 1);
		for(int i = 0; i < number; i++) {
			p = p.multiply(this);
		}
		return p;
	}
	
	public static ParseResult parseExpression(char[] text, int index) throws Exception {
		Polynomial p = new Polynomial();
		if(text[index] == '-') {
			ParseResult result = parseTerm(text, index+1);
			p = p.sub(result.polynomial);
			index = result.index;
		} else {
			ParseResult result = parseTerm(text, index);
			p = p.add(result.polynomial);
			index = result.index;
		}
		while(index < text.length && text[index] != ')') {
			ParseResult result;
			switch(text[index]) {
			case '+':
				result = parseTerm(text, index+1);
				p = p.add(result.polynomial);
				index = result.index;
				break;
			case '-':
				result = parseTerm(text, index+1);
				p = p.sub(result.polynomial);
				index = result.index;
				break;
			default:
				throw new Exception("index: " + index + " Expected:[+-)]" + " Actual:" + text[index]);
			}
		}
		if(index < text.length)index++;
		return new ParseResult(p, index);
	}
	
	private static ParseResult parseTerm(char[] text, int index) throws Exception {
		Polynomial p = new Polynomial();
		p.setValue(0, 1);
		while(index < text.length && text[index] != '+' && text[index] != '-' && text[index] != ')') {
			ParseResult result = parseFactor(text, index);
			index = result.index;
			p = p.multiply(result.polynomial);
		}
		return new ParseResult(p, index);
	}
	
	private static ParseResult parseFactor(char[] text, int index) throws Exception {
		ParseResult resultPrimary = parsePrimary(text, index);
		index = resultPrimary.index;
		if(index < text.length && text[index] == '^') {
			index++;
			ParseResult resultNumber = parseNumber(text, index);
			index = resultNumber.index;
			int number = resultNumber.polynomial.getValue(0);
			Polynomial p = resultPrimary.polynomial.power(number);
			return new ParseResult(p, index);
		} else {
			return resultPrimary;
		}
	}
	
	private static ParseResult parsePrimary(char[] text, int index) throws Exception {
		if(Character.isDigit(text[index])) {
			return parseNumber(text, index);
		} else if(text[index] == 'x') {
			Polynomial p = new Polynomial();
			p.setValue(1, 1);
			return new ParseResult(p, index+1);
		} else if(text[index] == '(') {
			ParseResult result = parseExpression(text, index+1);
			int nextIndex = result.index;
			return new ParseResult(result.polynomial, result.index);
		} else {
			throw new Exception("index: " + index + " Expected:[0-9x(]" + " Actual:" + text[index]);
		}
	}
	
	private static ParseResult parseNumber(char[] text, int index) throws Exception {
		if(!Character.isDigit(text[index])) {
			throw new Exception("index: " + index + " Expected:[0-9]" + " Actual:" + text[index]);
		}
		int output = 0;
		while(index < text.length && Character.isDigit(text[index])) {
			output *= 10;
			output += text[index] - '0';
			index++;
		}
		Polynomial p = new Polynomial();
		p.setValue(0, output);
		return new ParseResult(p, index);
	}
	
	private int getDegree() {
		for(int i = MAX_N-1; i >= 0; i--) {
			if(values[i] != 0)return i;
		}
		return 0;
	}
	
	private Polynomial copy() {
		Polynomial p = new Polynomial();
		for(int i = 0; i < MAX_N; i++) {
			p.values[i] = this.values[i];
		}
		return p;
	}
	
	private Polynomial mod(Polynomial p) {
		Polynomial output = copy();
		int d = p.getDegree();
		for(int i = MAX_N-1; i >= d; i--) {
			if(output.values[i] != 0) {
				int diffDegree = i - d;
				int t = output.values[i] / p.values[d];
				for(int j = d; j >= 0; j--) {
					output.values[i+j-d] -= t*p.values[j];
				}
			}
		}
		return output;
	}
	
	private boolean isZero() {
		for(int i = MAX_N-1; i >= 0; i--) {
			if(values[i] != 0)return false;
		}
		return true;
	}
	
	private static int gcd(int a, int b) {
		if(a%b==0)return b;
		return gcd(b, a%b);
	}
	
	public Polynomial normalize() {
		int g = values[getDegree()];
		Polynomial output = copy();
		if(g < 0) {
			for(int i = MAX_N-1; i >= 0; i--) {
				output.values[i] *= -1;
			}
		}
		for(int i = getDegree(); i >= 0; i--) {
			if(values[i] == 0)continue;
			g = gcd(g, Math.abs(values[i]));
		}
		for(int i = MAX_N-1; i >= 0; i--) {
			output.values[i] /= g;
		}
		return output;
	}
	
	public static Polynomial gcd(Polynomial p1, Polynomial p2) {
		int v1 = p1.values[p1.getDegree()];
		int v2 = p2.values[p2.getDegree()];
		int g = gcd(v1, v2);
		Polynomial temp = new Polynomial();
		temp.values[0] = v1*v2/g/v1;
		p1 = p1.multiply(temp);
		temp.values[0] = v1*v2/g/v2;
		p2 = p2.multiply(temp);
		Polynomial p = p1.mod(p2);
		if(p.isZero()) return p2;
		return gcd(p2, p);
	}
	
	public void printValues() {
		for(int i = MAX_N-1; i >= 0; i--) {
			System.out.print(values[i]);
			System.out.print(" ");
		}
		System.out.println();
	}
	
	public void print() {
		boolean firstTerm = true;
		for(int i = MAX_N-1; i >= 0; i--) {
			if(values[i] > 0) {
				if(!firstTerm) {
					System.out.print("+");
				}
				firstTerm = false;
			} else if(values[i] < 0) {
				System.out.print("-");
				firstTerm = false;
			}
			int v = Math.abs(values[i]);
			if(v != 0) {
				if(v != 1 || i == 0){
					System.out.print(v);
				}
				if(i > 1) {
					System.out.print("x^");
					System.out.print(i);
				} else if(i == 1) {
					System.out.print("x");
				}
			}
		}
		System.out.printf("\n");
	}
}

//http://fxp.hp.infoseek.co.jp/arti/parser.html