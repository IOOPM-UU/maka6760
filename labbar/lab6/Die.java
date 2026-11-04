import java.util.Scanner;

public class Die {
  private int numberOfSides;
  private int value;

  public Die() {
      this(6);
  }

  public Die(int numberOfSides) {
    if (numberOfSides < 1) {
        throw new IllegalArgumentException(numberOfSides + " is an illegal number of sides for a die!");
    }
      this.numberOfSides = numberOfSides;
      this.roll();
  }

  public int roll() {
    this.value = (int) (Math.random() * numberOfSides) + 1;
    return this.get();
  }

  public int get() {
      return value;
  }

  public String toString() {
    return "Die(" + value + ")";
  }

  boolean equals(Die otherDie) {
    if (this.numberOfSides == otherDie.numberOfSides) {
        return true;
    } else {
        return false;
    }
  }

  public static void main(String [] args) {
      Scanner sc = new Scanner(System.in);
      System.out.print("Number of sides: ");
      int sides = sc.nextInt();
      Die d = new Die(sides);
      System.out.println(d);
      System.out.println("Alea iacta est: " + d.roll());
  }
}
