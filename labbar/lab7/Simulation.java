import java.util.Random; 

public class Simulation {
    private Store store; //varuhuset som simuleras
    private int time; //antalet tidssteg sedan simuleringen startade
    private int intensity; //sannolikheten (i procent) att det ska komma en ny kund vid varje tidssteg.
    private int maxGroceries; //maxantalet varor som en kund kan ha när hen kommer till kassan.
    private int thresholdForNewRegister; //vid vilken snittlängd en ny kassa öppnas.
    private int doneCustomers;
    private double totalWaitTime;
    private Random random = new Random();
    //private "attribut om statistiken vi vill samla"

    //Konstruktor 
    public Simulation(int maxRegisters, int intensity, int maxGroceries, int thresholdForNewRegister) {
        this.store = new Store(maxRegisters);
        this.time = 0;
        this.intensity = intensity;
        this.maxGroceries = maxGroceries;
        this.thresholdForNewRegister = thresholdForNewRegister;
    }
    //Hämta alla kunder som är klara och samla in statistik från dem.
    public void step() {
        this.store.step();
        this.time++;

        if (random.nextInt(100) < intensity) {
            int groceries = random.nextInt(maxGroceries) + 1;
            Customer c = new Customer(time, groceries);
            this.store.newCustomer(c);
        }

        if (this.store.getAverageQueueLength() > thresholdForNewRegister) {
            this.store.openNewRegister();
        }
        
        for (Customer c: this.store.getDoneCustomers())
        {
            this.doneCustomers++;
            this.totalWaitTime += (this.time - c.getBornTime());
        }
    }

    //returnernar snitt-väntetiden, eller 0 om det inte finns några färdiga kunder 
    // för att undvika division med 0
    public double getAverageWaitTime() {
        if (this.doneCustomers != 0) {
            return (double) this.totalWaitTime / this.doneCustomers;
        } else {
            return 0;
        }
    }

    public int getDoneCustomers() {
        return this.doneCustomers;
    }

    @Override 
    public String toString() {
        return this.store.toString()
                + "\nMax wait-time: " + this.time
                + "\nNumber of customers served: " + this.doneCustomers
                + "\nAverage wait-time: " + this.getAverageWaitTime();
    }
    
    public static void main(String[] args) {
        
    }
}