//likt en linkad lista, men man tar bara bort första elementet, och lägger bara till i slutet
public class Queue {
    public static class EmptyQueueException extends RuntimeException {
    }

    // ett "element" i kön, likt en linkad lista
    private static class Node {
        private Customer element; // Kunden som står på just den platsen i kön
        private Node next; // Noden för platsen bakom den nuvarande.

        // Konstruktor
        public Node(Customer element) {
            this.element = element;
            this.next = null;
        }
    }

    private Node first; // Noden som håller det första elementet
    private Node last; // Noden som håller det sista elementet
    private int length; // Antalet kunder i kön just nu (frivilligt, men praktiskt att ha)

    // Konstruktor
    public Queue() {
        this.first = null;
        this.last = null;
        this.length = 0;
    }

    // hur lång är kön?
    public int length() {
        return this.length;
    }

    // Ställ en kund sist i kön
    public void enqueue(Customer c) {
        Node newNode = new Node(c);
        if (this.length == 0) {
            this.first = newNode;
            this.last = newNode;
        } else {
            this.last.next = newNode;
            this.last = newNode;
        }
        this.length++;
    }

    // Ta bort (och returnera) kunden som står först i kön.
    public Customer dequeue() {
        if (this.length == 0) {
            throw new EmptyQueueException();
        }

        Customer c = this.first.element;
        this.first = this.first.next;

        if (this.first == null) {
            this.last = null;
        }
        this.length--;
        return c;
    }

    // Returnera (men ta inte bort) kunden som står först i kön
    public Customer first() {
        if (this.first == null) { 
            return null;
        }
        return this.first.element;
    }

    public String toString() {
        Node current = this.first;
        String result = "[";
        while (current != null) {
            if (current == this.first) {
                result += current.element.toString();
            } else {
                result += ", ";
                result += current.element.toString(); 
            }
            current = current.next;
        }
        return result += "]";

    }

    public static void main(String[] args) {
        Queue q = new Queue();
        Customer c1 = new Customer(0, 1);
        Customer c2 = new Customer(1, 2);
        Customer c3 = new Customer(2, 3);

        q.enqueue(c1);
        q.enqueue(c2);
        q.enqueue(c3);
        System.out.println(q);

        q.dequeue();
        System.out.println(q.first());
        q.dequeue();
        System.out.println(q.first());
        q.dequeue();
        System.out.println(q.first());
        System.out.println(q);

    }

}