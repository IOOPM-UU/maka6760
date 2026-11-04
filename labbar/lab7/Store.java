public class Store {
    private Register registers[]; //kassorna i varuhuset

   //konstruktor
    public Store(int numberOfRegisters) {
        this.registers[numberOfRegisters];

        this.registers[0].open == true;
    }

    //vad är snittlängden för alla kassor i varuhuset?
    public int getAverageQueueLength() {
        //TODO: STUB
    }

    //ställ kunden c i den kortaste kön.
    public void newCustomer(Customer c) {
        //TODO: STUB
    }

    //tiden går ett steg i varuhuset.
    public void step() {
        //TODO: STUB
    }

    //öppna en ny kassa (om det går).
    public boolean openNewRegister() {
        //TODO: STUB
    }

    // returnera alla kunder som är klara i det nuvarande tidssteget.
    public Customer getDoneCustomers() {

    }
    
    public static void main(String[] args) {
        
    }
}