import java.util.Scanner;

public class NameOrder {
    public static void main(String[] args) {
        Scanner sc = new Scanner(System.in);
        System.out.print("First name: ");
        String name1 = sc.nextLine();
        System.out.print("Second name: ");
        String name2 = sc.nextLine();
        if (name1.compareToIgnoreCase(name2) < 0) {
            System.out.println(name1 + ", " + name2);
        } else {
            System.out.println(name2 + ", " + name1);
        }

    }
}