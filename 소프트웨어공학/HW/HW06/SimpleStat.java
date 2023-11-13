import java.util.ArrayList;
import java.util.List;
import java.util.Scanner;

public class SimpleStat {
	public static void main(String[] args) {
		Scanner s1 = new Scanner(System.in);
		System.out.println("Enter integer ( 0 or minus for Exit)");
		List<Integer> ns = new ArrayList<>();
		while (true) {
			int n = s1.nextInt();
			if (n <= 0)
				break;
			ns.add(n);
		}
		s1.close();
		long s = 0;
		long p = 1;
		for (int i : ns) {
			s += i;
			p *= i;
		}
		System.out.println("Sum: " + s);
		System.out.println("Product: " + p);
	}
}