package org.ioopm.calculator;

import java.io.IOException;

//stämmer inte, main är hämtad från parserdriver. Detta endast för att kunna köra
public class Calculator {
        public static void main(String[] args) {
        ParserExample p = new ParserExample();

        System.out.println("Welcome to the parser!");
        System.out.print("Please enter an expression: ");

        try {
            double result = p.expression();
            System.out.println("result: " + result);
        } catch(SyntaxErrorException e) {
            System.out.print("Syntax Error: ");
            System.out.println(e.getMessage());
        } catch(IOException e) {
            System.err.println("IO Exception!");
        }
    }
}

