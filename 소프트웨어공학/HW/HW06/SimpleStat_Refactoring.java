import java.util.ArrayList;
import java.util.List;
import java.util.Scanner;

public class SimpleStat_Refactoring {
	public static void main(String[] args) {
		Scanner scanner = new Scanner(System.in);
		final String promptMessage = "Enter integer ( 0 or minus for Exit)";

		showPrompt(promptMessage);

		List<Integer> numbers = readValues(scanner, createList())
		while (true) {
			int n = s1.nextInt();
			if (n <= 0)
				break;
				numbers.add(n);
		}
		scanner.close();
		long sum = 0;
		long product = 1;
		for (int i : numbers) {
			sum += i;
			product *= i;
		}
		showResult(sum, product);
	}
	private static void showPrompt(final String promptMessage) {
		System.out.println(promptMessage);
	}
	private static List<Integer> createList() {
		List<Integer> ns = new ArrayList<>();
		return ns;
	}
	private static List<Integer> readValues(Scanner scanner, List<Integer> ns) {
		while(true) {
			int n = scanner.nextInt();
			if (n <= 0) 
				break;
			ns.add(n);
		}
		return ns;
	}
	private static void showResult(long sum, long product) {
		showLabelandValue("Sum: ", sum);
		showLabelandValue("Product: ", product);
	}	
	private static void showLabelandValue(final String label1, long sum) {
		System.out.println(label1 + sum);
	}
	private static long getProduct(List<Integer> numbers) {
		
	}
}