
public class Customer {
    private int bornTime; //tidssteget som kunden kom in i systemet
    private int groceries; // antalet varor i kundens korg

    //Konstruktor
    public Customer(int bornTime, int groceries) { 
        this.bornTime = bornTime;
        this.groceries = groceries;     
    }

    public int getBornTime() {
        return this.bornTime;
    }

    // registrera en av kundens varor (alltså minska på groceries)
    public void serve() { 
        this.groceries -= 1;
    }

    // fråga om kunden är färdig, dvs. om groceries är 0.
    public boolean isDone() { 
        if (this.groceries == 0) {
            return true;
        } else {
            return false;
        }
    }

    public String toString() {
        return "[" + this.groceries + "]";
    }

    public String debugString() {
        return "Customer " + this.bornTime + ": (groceries: " +  this.groceries + ")";
    }

    public static void main(String[] args) {
        Customer c = new Customer(0, 3);
        System.out.println(c);
        for (int i = 0; i < 3; i++) {
            c.serve();
        }
        System.out.println(c);
        if (c.isDone()) {
            System.out.println("The customer is done, just as expected!");
        } else {
            System.out.println("The customer is not done, but should be...");
        }
    }
}
