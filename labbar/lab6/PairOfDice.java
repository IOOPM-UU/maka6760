public class PairOfDice {
    private Die die1;
    private Die die2;

    public PairOfDice(int numberOfSides) {
        this.die1 = new Die(numberOfSides);
        this.die2 = new Die(numberOfSides);
    }

    public int rollPair() {
        die1.roll();
        die2.roll();
        return die1.get() + die2.get();
    }

    public int getFromPair(int dieNumber) {
        if (dieNumber == 1) {
            return die1.get();
        } else if (dieNumber == 2) {
            return die2.get();
        } else {
            throw new IllegalArgumentException(dieNumber + "is not a valid die number! (1/2)");
        }
    }

    public String toString() {
        return "Pair(" + getFromPair(1) + "," + getFromPair(2) + ")";
    }
    public static void main(String[] args) {
        PairOfDice p = new PairOfDice(6);
        System.out.println(p);
        System.out.println("Summa: " + p.rollPair());
        System.out.println(p);
    }
}