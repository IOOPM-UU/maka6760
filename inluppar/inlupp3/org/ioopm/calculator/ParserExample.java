package org.ioopm.calculator;

/// File ParserExample.java
import java.io.StreamTokenizer;
import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.Reader;

public class ParserExample {
    private final Reader r = new BufferedReader(new InputStreamReader(System.in));
    private final StreamTokenizer st = new StreamTokenizer(r);

    public ParserExample() {
        /// We want to treat - and end of line as an ordinary tokens
        this.st.ordinaryChar('-'); /// parse object-oriented as "object" - "oriented" :) 
        this.st.eolIsSignificant(true);
    }

    /// This is the top-level expression -- the "entry point" 
    public double expression() throws IOException {
        /// Read a term and make it the current sum 
        double sum = term();
        /// Read the next token and put it in sval/nval/ttype depending on the token
        this.st.nextToken();
        /// If the token read was + or -, go into the loop 
        while (this.st.ttype == '+' || this.st.ttype == '-') {
            if(this.st.ttype == '+'){
                /// If we are adding things, read a term and add it to the current sum
                sum += term();
            } else {
                /// If we are adding things, read a term and subtract it from the current sum
                sum -= term();
            }
            /// Read the next token into sval/nval/ttype so we can go back in the loop again
            this.st.nextToken();
        }
        /// If we came here, we read something which was not a + or -, so we need to put
        /// that back again (whatever it was) so that we did not accidentally ate it up!
        this.st.pushBack();
        /// We are done, return sum
        return sum;
    }

    /// This method works like expression, but with factors and * instead of terms and +/-
    private double term() throws IOException {
        double prod = factor();
        while (this.st.nextToken() == '*') {
            prod *= factor();
        }
        this.st.pushBack();
        return prod;
    }

    private double factor() throws IOException {
        double result;
        /// If we encounter a (, we know we are reading a full expression, so we call back up 
        /// to that method, and then try to read a closing ) at the end 
        if (this.st.nextToken() == '('){
            result = expression();
            /// This captures unbalanced parentheses!
            if (this.st.nextToken() != ')') {
                throw new SyntaxErrorException("expected ')'");
            }
        } else {
            this.st.pushBack();
            result = number();
        }
        return result;
    }

    private double number() throws IOException {
        if (this.st.nextToken() == StreamTokenizer.TT_NUMBER) {
            return this.st.nval;
        } else {
            throw new SyntaxErrorException("Expected number");
        }
    }
}
