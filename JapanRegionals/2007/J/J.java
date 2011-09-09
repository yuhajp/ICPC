import java.util.*;
import java.math.*;

public class J {

    public static void main(String[] args) {
        Scanner sc = new Scanner(System.in);
        while (true) {
            int a = sc.nextInt();
            int m = sc.nextInt();
            int b = sc.nextInt();
            int n = sc.nextInt();
            if (a == 0 && m == 0 && b == 0 && n == 0) { break; }

            BigInteger[] ans = solve(a, m, b, n);

            boolean first = true;
            for (int i = ans.length - 1; i >= 0; --i) {
                int ai = ans[i].intValue();
                if (first) { first = false; } else { System.out.print(" "); }
                System.out.print(ai);
            }
            System.out.println();
        }
    }

    static BigInteger gcd(BigInteger a, BigInteger b)
    {
        return (BigInteger.ZERO.equals(b) ? a.abs() : gcd(b.abs(), a.abs().remainder(b.abs())));
    }

    static void normalize(BigInteger[] vs, BigInteger[] ts) {
        BigInteger gcdv = null;
        for (int i = 0; i < vs.length; ++i) {
            if (!BigInteger.ZERO.equals(vs[i])) {
                if (gcdv == null) { gcdv = vs[i]; }
                else { gcdv = gcd(gcdv, vs[i]); }
            }
        }
        for (int i = 0; i < ts.length; ++i) {
            if (!BigInteger.ZERO.equals(ts[i])) {
                if (gcdv == null) { gcdv = ts[i]; }
                else { gcdv = gcd(gcdv, ts[i]); }
            }
        }

        for (int i = 0; i < vs.length; ++i) {
            vs[i] = vs[i].divide(gcdv);
        }
        for (int i = 0; i < ts.length; ++i) {
            ts[i] = ts[i].divide(gcdv);
        }
    }

    static BigInteger[] solve(int a, int m, int b, int n) {
        int mn = m * n;
        BigInteger[][] matrix = new BigInteger[mn + 1][mn];
        for (int i = 0; i <= mn; ++i) {
            for (int j = 0; j < mn; ++j) {
                matrix[i][j] = BigInteger.ZERO;
            }
        }
        matrix[0][0] = BigInteger.ONE;

        for (int i = 1; i <= mn; ++i) {
            BigInteger[] poly = matrix[i];
            for (int mi = 0; mi < m; ++mi) {
                for (int ni = 0; ni < n; ++ni) {
                    if (mi + 1 == m) {
                        matrix[i][ni] =  matrix[i][ni].add(matrix[i - 1][mi * n + ni].multiply(BigInteger.valueOf(a)));
                    } else {
                        matrix[i][(mi + 1) * n + ni] = matrix[i][(mi + 1) * n + ni].add(matrix[i - 1][mi * n + ni]);
                    }
                }
            }
            for (int mi = 0; mi < m; ++mi) {
                for (int ni = 0; ni < n; ++ni) {
                    if (ni + 1 == n) {
                        matrix[i][mi * n] = matrix[i][mi * n].add(matrix[i - 1][mi * n + ni].multiply(BigInteger.valueOf(b)));
                    } else {
                        matrix[i][mi * n + (ni + 1)] = matrix[i][mi * n + (ni + 1)].add(matrix[i - 1][mi * n + ni]);
                    }
                }
            }
            
        }

        BigInteger[][] tpoly = new BigInteger[mn + 1][mn + 1];
        for (int i = 0; i <= mn; ++i) {
            for (int j = 0; j <= mn; ++j) {
                tpoly[i][j] = BigInteger.ZERO;
            }
            tpoly[i][i] = BigInteger.ONE;
        }

        if (true) {
            System.out.println("------------------------------");
            for (int s = 0; s <= mn; ++s) {
                for (int t = 0; t < mn; ++t) {
                    System.out.print(matrix[s][t]);
                    System.out.print(' ');
                }
                System.out.print('|');
                for (int t = 0; t <= mn; ++t) {
                    System.out.print(' ');
                    System.out.print(tpoly[s][t]);
                }
                System.out.println();
            }
        }

        // ERASING
        for (int i = 0; i < mn; ++i) {
            int idx = -1;
            for (int j = i; j <= mn; ++j) {
                if (!BigInteger.ZERO.equals(matrix[j][i])) { idx = j; break; }
            }
            if (idx < 0) { System.out.println("CNT"); continue; }
            
            // swap
            {
                BigInteger[] t = matrix[i];
                matrix[i] = matrix[idx];
                matrix[idx] = t;
            }
            {
                BigInteger[] t = tpoly[i];
                tpoly[i] = tpoly[idx];
                tpoly[idx] = t;
            }

            for (int j = i + 1; j <= mn; ++j) {
                if (BigInteger.ZERO.equals(matrix[j][i])) { continue; }
                BigInteger x = matrix[j][i];
                for (int k = 0; k <= mn; ++k) {
                    tpoly[j][k]  = tpoly[j][k].multiply(matrix[i][i]).subtract(tpoly[i][k].multiply(x));
                }
                for (int k = 0; k < mn; ++k) {
                    matrix[j][k] = matrix[j][k].multiply(matrix[i][i]).subtract(matrix[i][k].multiply(x));
                }
                normalize(matrix[j], tpoly[j]);
            }

            if (true) {
            System.out.println("------------------------------");
            for (int s = 0; s <= mn; ++s) {
                for (int t = 0; t < mn; ++t) {
                    System.out.print(matrix[s][t]);
                    System.out.print(' ');
                }
                System.out.print('|');
                for (int t = 0; t <= mn; ++t) {
                    System.out.print(' ');
                    System.out.print(tpoly[s][t]);
                }
                System.out.println();
            }
            }
        }

        BigInteger[] answer = tpoly[mn];
        if (answer[mn].compareTo(BigInteger.ZERO) < 0) {
            for (int i = 0; i < answer.length; ++i) {
                answer[i] = answer[i].negate();
            }
        }
        return answer;
    }
}





