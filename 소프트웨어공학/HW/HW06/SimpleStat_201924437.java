import java.util.ArrayList;
import java.util.List;
import java.util.Scanner;

public class SimpleStat {
    private static final int EXIT_CONDITION = 0;
    public static void main(String[] args) {
        Scanner scanner = new Scanner(System.in);
        List<Integer> numbers = readIntegers(scanner);
        scanner.close();

        int sum = sumAllNumbers(numbers);
        int product = multipleAllNumbers(numbers);

        printResults(sum, product);
    }

    private static List<Integer> readIntegers(Scanner scanner) {
        System.out.println("Enter integers (0 or a negative number to exit):");
        List<Integer> numbers = new ArrayList<>();

        while (true) {
            int number = scanner.nextInt();
            if (number <= EXIT_CONDITION)
                break;
            numbers.add(number);
        }

        return numbers;
    }

    //배열의 수를 모두 더하는 함수
    private static int sumAllNumbers(List<Integer> numbers) {
        int sum = 0;
        for (int number : numbers) {
            sum += number;
        }
        return sum;
    }

    //배열의 수를 모두 곱하는 함수
    private static int multipleAllNumbers(List<Integer> numbers) {
        int product = 1;
        for (int number : numbers) {
            product *= number;
        }
        return product;
    }

    private static void printResults(int sum, int product) {
        System.out.println("Sum: " + sum);
        System.out.println("Product: " + product);
    }
}