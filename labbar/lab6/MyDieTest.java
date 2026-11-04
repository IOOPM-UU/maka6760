import java.util.Scanner;

public class MyDieTest {
    public static void main(String[] args) {
        Scanner sc = new  Scanner(System.in);
        System.out.print("Number of sides: ");
        int sides = sc.nextInt();
        Die d = new Die(sides);

        //int result = 0;
        //for (int i = 0; i < 10; i++) {
        //    int value = d.roll(); 
        //    result += value;
        //}
        //System.out.println("Sum of 10 rolls of " + sides + " sided die " + "= " + result); 
        Die a = new Die(6);
        Die b = new Die(6);
        System.out.println("a == b:         " + (a==b));
        System.out.println("a.equals(b): " + a.equals(b));
    }
}