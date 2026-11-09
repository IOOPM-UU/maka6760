public class Simulation {
    private Store store; // varuhuset som simuleras
    private int time; //antalet tidssteg sedan simuleringen startade
    private int intensity; //sannolikheten (i procent) att det ska komma en ny kund vid varje tidssteg.
    private int maxGroceries; //maxantalet varor som en kund kan ha när hen kommer till kassan.
    private int thresholdForNewRegister; //vid vilken snittlängd en ny kassa öppnas.
    //private "attribut om statistiken vi vill samla"


    //Låt tiden gå ett steg i varuhuset.
    //Slumpa ett heltal mellan 0 och 100 om heltalet är mindre än intensity, skapa en ny kund och skicka den till varuhuset. Antalet varor i kundens korg slumpas fram mellan 1 och maxGroceries.
    //Hämta snittlängden på kassaköerna i varuhuset. Om den är högre än thresholdForNewRegister, öppna en ny kassa.
    //Hämta alla kunder som är klara och samla in statistik från dem.
    public void step() {
        //TODO: STUB
    }

    public static void main(String[] args) {
        
    }
}