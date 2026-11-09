public class Register {
    private boolean open; //true om kassan är öppen, annars false
    private Queue queue; //En kö av kunderna som väntar på att behandlas. Kunden som står längst fram i kön är den som får sina varor behandlade.

    //Konstruktor 
    public Register() {
        this.open = false;
        this.queue = new Queue();
    }

    //öppna kassan
    public void open() {
        this.open = true;
    }

    //stäng kassan
    public void close() {
        this.open = false;
    }

    //är kassan öppen?
    public boolean isOpen() {
        if (this.open) {
            return true;
        } else {
            return false;
        }
    }

    //låt tiden gå ett steg i kassan. Det betyder att kunden som står först i kön får en vara registrerad.
    public void step() {
        if (hasCustomers()) {
            this.queue.first().serve();
        }
    }

    //har kassan några kunder?
    public boolean hasCustomers() {
        if (this.queue.length() > 0) {
            return true;
        } else {
            return false;
        }
    }

    //är kunden som står längst fram i kön klar?
    public boolean currentCustomerIsDone() {
        if (this.queue.first().isDone()) {
            return true;
        } else {
            return false;
        }
    }

    //sätt kunden c sist i kön
    public void addToQueue(Customer c) {
        this.queue.enqueue(c);
    }

    //ta bort (och returnera) kunden som står först i kön
    public Customer removeCurrentCustomer() {
        return this.queue.dequeue(); 
    }

    //hur lång är kön?
    public int getQueuelength() {
        return this.queue.length();
    }

    public static void main(String[] args) {
        //AI (Claude) gav ideen till testerna, de är implementerade av mig själv.
        Register r = new Register();

        //Ny kassa, stängd och tom
        System.out.println("Stängd från start: " + !r.isOpen());
        System.out.println("Inga kunder: " + !r.hasCustomers());
        System.out.println("Kölängd 0: " + (r.getQueuelength() == 0));

        //Öppna och stäng 
        r.open();
        System.out.println("Öppen: " + r.isOpen());
        r.close();
        System.out.println("Stängd: " + !r.isOpen());
        r.open();

        // Lägg till kunder
        Customer c1 = new Customer(0, 2);
        Customer c2 = new Customer(1, 1);
        r.addToQueue(c1);  
        r.addToQueue(c2);
        System.out.println("Har kunder: " + r.hasCustomers());
        System.out.println("Kölängd 2: " + (r.getQueuelength() == 2));
        
        //Serva första kunden steg för steg
        System.out.println("c1 inte klar: " + !r.currentCustomerIsDone());
        r.step();
        System.out.println("c1 inte klar efter ett steg: " + !r.currentCustomerIsDone());
        r.step();
        System.out.println("c1 klar efter ett 2 steg: " + r.currentCustomerIsDone());

        //Ta bort den färdiga kunden och kontrollera om det är c1
        Customer removed = r.removeCurrentCustomer();
        System.out.println("c1 är borttagen: " + (removed == c1));
        System.out.println("Kölängd 1: " + (r.getQueuelength() == 1));

        //c2 borde vara först 
        r.step();
        System.out.println("c2 klar efter ett steg: " + r.currentCustomerIsDone());
        r.removeCurrentCustomer();

        //Kön tom igen
        System.out.println("Inga kunder igen: " + !r.hasCustomers());

        //Köa igen efter tömning
        r.addToQueue(new Customer(5, 1));
        System.out.println("Fungerar efter tömning: " + (r.getQueuelength() == 1));
    }
}
