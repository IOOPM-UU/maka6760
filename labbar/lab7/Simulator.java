public class Simulator {

    public static void main(String[] args) throws InterruptedException {
        int steps = 100;
        Simulation s = new Simulation(4, 80, 8, 2); 
        for(int i = 0; i < steps; i++) {
            System.out.print("\033[H\033[2J\033[3J");
            s.step();
            System.out.println(s);
            Thread.sleep(500);
        }
        System.out.println("");
        System.out.println("Simulation finished!");
    }
}