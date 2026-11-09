import java.util.ArrayList;

public class Store {
    private Register registers[]; //kassorna i varuhuset

   //konstruktor
    public Store(int numberOfRegisters) {
        this.registers = new Register[numberOfRegisters];
        for (int i = 0; i < numberOfRegisters; i++) {
            registers[i] = new Register(); 
        }
        this.registers[0].open();
    }

    //vad är snittlängden för alla kassor i varuhuset?
    public double getAverageQueueLength() {
        int sum = 0;
        int openRegisters = 0;
        for (Register r: this.registers) {
            if (r.isOpen()) {
                sum += r.getQueuelength(); 
                openRegisters++;
            }  
        }
        return (double) sum / openRegisters;
    }

    //ställ kunden c i den kortaste kön.
    public void newCustomer(Customer c) {
        int bestIndex = -1;
        int bestLength = 999999999;

        for (int i = 0; i < this.registers.length; i++) {
            Register r = this.registers[i];
            if (r.isOpen() && r.getQueuelength() < bestLength) {
                bestIndex = i;
                bestLength = r.getQueuelength();
            }
        }
        if (bestIndex != -1) {
            this.registers[bestIndex].addToQueue(c);
        }
    }

    //tiden går ett steg i varuhuset.
    public void step() {
        for (Register r: this.registers) {
            r.step();
        }
    }

    //öppna en ny kassa (om det går).
    public boolean openNewRegister() {
        for (Register r: this.registers) {
            if (!r.isOpen()) {
                r.open();
                return true;
            }
        }
        return false;
    }

    // returnera alla kunder som är klara i det nuvarande tidssteget.
    public ArrayList<Customer> getDoneCustomers() {
        ArrayList<Customer> done = new ArrayList<>();
        for (Register r: this.registers) {
            if (r.hasCustomers() && r.currentCustomerIsDone()) {
                done.add(r.removeCurrentCustomer());
            }
        }
        return done;
    }

    @Override 
    public String toString() {
        String result = "";
        for (Register r: this.registers) {
            result += r + "\n";
        }
        return result;
    } 
    
    public static void main(String[] args) {
        //AI (Claude) gav ideerna till testerna

        Customer c1 = new Customer(0, 3);
        Customer c2 = new Customer(1, 2);
        Customer c3 = new Customer(2, 1);

        //Konstruktor

        // Öppna en store med 3 kassor, det ska inte finnas några kunder i kön.
        Store s1 = new Store(3);
        System.out.println("Bara en kassa öppen från start: " + (s1.getAverageQueueLength() == 0));

        // newCustomer

        //Lägg till en kund, kolla att snittkö-längd = 1
        s1.newCustomer(c1); 
        System.out.println("Bara en kassa öppen från start: " + (s1.getAverageQueueLength() == 1));

        // Öppna en till kassa och lägg till en till kund, snittkölängd ska fortfarande vara 1
        s1.openNewRegister();
        s1.newCustomer(c2);
        System.out.println("2 kassor, 2 kunder: " + (s1.getAverageQueueLength() == 1));

        //Lägg till en till kund med 2 öppn kassor, snittkölängd ska bli 1.5
        s1.newCustomer(c3);
        System.out.println("2 kassor, 3 kunder: " + (s1.getAverageQueueLength() == 1.5));

        //Kunder får inte hamna i stängda kassor. 1 kassa öppen men 3 kunder ska ge snittkölängd = 3.
        Store s2 = new Store(3);
        s2.newCustomer(c1);
        s2.newCustomer(c2);
        s2.newCustomer(c3);
        System.out.println("1 kassa, 3 kunder: " + (s2.getAverageQueueLength() == 3));

        System.out.println();

        //openNewRegister

        // med 3 kassor ska två anrop av funktionen ge true men det tredje ska ge false, eftersom alla är öppna
        Store s3 = new Store(3);
        System.out.println("Öppna kassa 2: " + s3.openNewRegister());
        System.out.println("Öppna kassa 3: " + s3.openNewRegister());
        System.out.println("Inga fler kassor att öppna: " + s3.openNewRegister());

        //Kontrollera att snittlängden räknas rätt när fler kassor öppnas. 3 kunder i en kassa ger 3.0
        //öppnar man en till (tom) ska snittet bli 1.5
        Store s4 = new Store(3);
        s4.newCustomer(c1);
        s4.newCustomer(c2);
        s4.newCustomer(c3);
        System.out.println("1 kassa, 3 kunder: " + (s4.getAverageQueueLength() == 3));
        s4.openNewRegister();
        System.out.println("2 kassor (en tom), 3 kunder, : " + (s4.getAverageQueueLength() == 1.5));

        System.out.println();

        //step och getDoneCustomers

        //Lägg in en kund med 2 varor, efter första step ska getDoneCustomers vara tom. Efter andra step() ska 
        //den innehålla kunden
        Store s5 = new Store(3);
        s5.newCustomer(c2);
        s5.openNewRegister();
        s5.step();
        System.out.println("Kunden har en vara kvar: " + s5.getDoneCustomers().isEmpty());
        s5.step();
        ArrayList<Customer> done1 = s5.getDoneCustomers();
        System.out.println("Kunden ska finnas i listan: " + (done1.size() == 1 && done1.get(0) == c2));

        //Kolla att kunden verkligen tagits bort. Snittlängden ska vara 0 och ett anrop till getDoneCustomers ska ge en tom lista.
        System.out.println("Ingen kund kvar: " + (s5.getAverageQueueLength() == 0));
        System.out.println("Listan ska vara tom: " + s5.getDoneCustomers().isEmpty());

        // Kunder med olika varor i kassan. Kunden med 1 vara ska bli klar efter första step(),
        // den med 3 varor först efter 3e.
        Store s6 = new Store(3);
        s6.openNewRegister();
        s6.openNewRegister();

        Customer c4 = new Customer(4, 1);
        Customer c5 = new Customer(5, 2);
        Customer c6 = new Customer(6, 3);

        s6.newCustomer(c4);
        s6.newCustomer(c5);
        s6.newCustomer(c6);

        s6.step();
        ArrayList<Customer> done2 = s6.getDoneCustomers();
        System.out.println("c4 ska vara klar: " + (done2.size() == 1 && done2.get(0) == c4)); 

        s6.step();
        done2 = s6.getDoneCustomers();
        System.out.println("c5 ska vara klar: " + done2.contains(c5));

        s6.step();
        done2 = s6.getDoneCustomers();
        System.out.println("c6 ska vara klar: " + done2.contains(c6));


        //två kunder i samma kö, med 1 vara var. Den första ska bli klar efter 1 steg.
        //Den andra ska inte bli serverad förrän den första tagits bort, så den kommer ut först efter steg 2.
        Store s7 = new Store(3);
        Customer c7 = new Customer(7, 1);
        Customer c8 = new Customer(8, 1);

        s7.newCustomer(c7);
        s7.newCustomer(c8);

        s7.step();
        ArrayList<Customer> done3 = s7.getDoneCustomers();
        System.out.println("c7 ska vara klar: " + done3.contains(c7));

        s7.step();
        done3 = s7.getDoneCustomers();
        System.out.println("c8 ska vara klar: " + done3.contains(c8));

        System.out.println();

        //Gränsfall

        //Kalla step() och getDoneCustomers() på en Store utan kunder. Det ska inte krascha, och listan ska vara tom
        Store s8 = new Store(3);

        s8.step();
        System.out.println("Listan ska vara tom: " + s8.getDoneCustomers().isEmpty());


    }
}