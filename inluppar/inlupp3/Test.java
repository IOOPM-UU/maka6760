import org.ioopm.calculator.ast.*;
import org.ioopm.calculator.ast.atom.*;
import org.ioopm.calculator.ast.binary.*;
import org.ioopm.calculator.ast.unary.*;

public class Test {
    public static void testEvaluating(SymbolicExpression expected, SymbolicExpression e, Environment vars) {
        SymbolicExpression r = e.eval(vars);
        if (r.equals(expected)) {
            System.out.println("Passed: " + e);
        } else {
            System.out.println("Error: expected '" + expected + "' but got '" + r + "'");
        }
}

    // Generated with Claude: convenience overload, evaluates with an empty environment
    public static void testEvaluating(SymbolicExpression expected, SymbolicExpression e) {
        testEvaluating(expected, e, new Environment());
    }

    public static void testPrinting(String expected, SymbolicExpression e) {
        if (expected.equals("" + e)) {
            System.out.println("Passed: " + e);
        } else {
            System.out.println("Error: expected '" + expected + "' but got '" + e + "'");
        }
        assert expected.equals("" + e);
    }

    public static void main(String[] args) {
        Constant c1 = new Constant(5);
        Constant c2 = new Constant(2);
        Variable v = new Variable("x");
        Addition a = new Addition(c1, v);
        Multiplication m = new Multiplication(a, c2);

        testPrinting("(5.0 + x) * 2.0", m);

        // Test cases below generated with Claude (expected strings assume
        // Constant prints as String.valueOf(double), e.g. "5.0")

        // Simple expressions
        testPrinting("42.0", new Constant(42));
        testPrinting("x", new Variable("x"));

        // Binary operators, no parentheses needed
        testPrinting("1.0 + 2.0", new Addition(new Constant(1), new Constant(2)));
        testPrinting("1.0 - 2.0", new Subtraction(new Constant(1), new Constant(2)));
        testPrinting("1.0 * 2.0", new Multiplication(new Constant(1), new Constant(2)));
        testPrinting("1.0 / 2.0", new Division(new Constant(1), new Constant(2)));

        // Priority: * binds harder than +
        testPrinting("5.0 + x * 2.0",
                new Addition(new Constant(5), new Multiplication(new Variable("x"), new Constant(2))));
        testPrinting("(5.0 + x) * 2.0",
                new Multiplication(new Addition(new Constant(5), new Variable("x")), new Constant(2)));
        testPrinting("2.0 * (a - b)",
                new Multiplication(new Constant(2), new Subtraction(new Variable("a"), new Variable("b"))));
        testPrinting("a * b * c / d",
                new Division(new Multiplication(new Multiplication(new Variable("a"), new Variable("b")),
                        new Variable("c")), new Variable("d")));
        testPrinting("(a + b) * (c - d)",
                new Multiplication(new Addition(new Variable("a"), new Variable("b")),
                        new Subtraction(new Variable("c"), new Variable("d"))));

        // Unary operators
        testPrinting("sin x", new Sin(new Variable("x")));
        testPrinting("cos 1.0", new Cos(new Constant(1)));
        testPrinting("exp x", new Exp(new Variable("x")));
        testPrinting("log 10.0", new Log(new Constant(10)));
        testPrinting("- x", new Negation(new Variable("x")));

        // Unary combined with binary
        testPrinting("sin x * 5.0",
                new Multiplication(new Sin(new Variable("x")), new Constant(5)));
        testPrinting("sin (x * 5.0)",
                new Sin(new Multiplication(new Variable("x"), new Constant(5))));
        testPrinting("sin (x + 1.0)",
                new Sin(new Addition(new Variable("x"), new Constant(1))));
        testPrinting("sin cos x", new Sin(new Cos(new Variable("x"))));

        // Assignment (value = variable)
        testPrinting("3.0 + 4.0 = x",
                new Assignment(new Addition(new Constant(3), new Constant(4)), new Variable("x")));
        testPrinting("(3.0 = x) + 1.0",
                new Addition(new Assignment(new Constant(3), new Variable("x")), new Constant(1)));

        // Same priority on the right-hand side (see discussion about a - (b - c))
        testPrinting("a - (b - c)",
                new Subtraction(new Variable("a"), new Subtraction(new Variable("b"), new Variable("c"))));
        testPrinting("a - b - c",
                new Subtraction(new Subtraction(new Variable("a"), new Variable("b")), new Variable("c")));

        System.out.println();

        // Equality tests generated with Claude
        // Leaves
        testEquals(true, new Constant(5), new Constant(5));
        testEquals(false, new Constant(5), new Constant(6));
        testEquals(false, new Constant(5), new Variable("x"));
        testEquals(true, new Variable("x"), new Variable("x"));
        testEquals(false, new Variable("x"), new Variable("y"));
        testEquals(false, new Variable("x"), new Constant(5));

        // Unary
        testEquals(true, new Sin(new Constant(1)), new Sin(new Constant(1)));
        testEquals(false, new Sin(new Constant(1)), new Cos(new Constant(1)));
        testEquals(false, new Sin(new Constant(1)), new Sin(new Constant(2)));
        testEquals(false, new Sin(new Constant(1)), new Constant(1));
        testEquals(true, new Negation(new Variable("x")), new Negation(new Variable("x")));

        // Binary
        testEquals(true,
                new Addition(new Constant(1), new Constant(2)),
                new Addition(new Constant(1), new Constant(2)));
        testEquals(false,
                new Addition(new Constant(1), new Constant(2)),
                new Subtraction(new Constant(1), new Constant(2)));
        testEquals(false,
                new Subtraction(new Variable("a"), new Variable("b")),
                new Subtraction(new Variable("b"), new Variable("a")));
        testEquals(false,
                new Addition(new Constant(1), new Constant(2)),
                new Addition(new Constant(1), new Constant(3)));
        testEquals(false, new Addition(new Constant(1), new Constant(2)), new Constant(3));

        // Nested trees: (5 + x) * 2 built twice
        testEquals(true,
                new Multiplication(new Addition(new Constant(5), new Variable("x")), new Constant(2)),
                new Multiplication(new Addition(new Constant(5), new Variable("x")), new Constant(2)));
        testEquals(false,
                new Multiplication(new Addition(new Constant(5), new Variable("x")), new Constant(2)),
                new Multiplication(new Addition(new Constant(5), new Variable("y")), new Constant(2)));

        System.out.println();

        // Evaluation tests generated with Claude
        // (these are expected to fail until eval() is implemented in 8.7)
        // Leaves are not reduced
        testEvaluating(new Constant(5), new Constant(5));
        testEvaluating(new Variable("x"), new Variable("x"));

        // Binary, fully reducible
        testEvaluating(new Constant(6), new Multiplication(new Constant(2), new Constant(3)));
        testEvaluating(new Constant(6), new Subtraction(new Constant(10), new Constant(4)));
        testEvaluating(new Constant(4), new Division(new Constant(8), new Constant(2)));
        testEvaluating(new Constant(9),
                new Multiplication(new Addition(new Constant(1), new Constant(2)), new Constant(3)));

        // Binary, not reducible because of a variable
        testEvaluating(new Addition(new Variable("x"), new Constant(37)),
                new Addition(new Variable("x"), new Constant(37)));
        testEvaluating(
                new Multiplication(new Addition(new Constant(5), new Variable("x")), new Constant(2)),
                new Multiplication(new Addition(new Constant(5), new Variable("x")), new Constant(2)));

        // Partially reducible: 1 + 2 + x ==> 3 + x
        testEvaluating(new Addition(new Constant(3), new Variable("x")),
                new Addition(new Addition(new Constant(1), new Constant(2)), new Variable("x")));

        // Unary
        testEvaluating(new Constant(0), new Sin(new Constant(0)));
        testEvaluating(new Constant(1), new Cos(new Constant(0)));
        testEvaluating(new Constant(1), new Exp(new Constant(0)));
        testEvaluating(new Constant(0), new Log(new Constant(1)));
        testEvaluating(new Constant(-5), new Negation(new Constant(5)));
        testEvaluating(new Constant(-5), new Negation(new Addition(new Constant(2), new Constant(3))));
        testEvaluating(new Sin(new Variable("x")), new Sin(new Variable("x")));

        System.out.println();

        // Variable tests generated with Claude (8.7.2)
        // Part 1: variables put directly in the environment (tests Variable.eval and hashCode)
        Environment env = new Environment();
        env.put(new Variable("a"), new Constant(3));
        testEvaluating(new Constant(3), new Variable("a"), env);
        testEvaluating(new Variable("b"), new Variable("b"), env);      // unknown variable stays
        testEvaluating(new Constant(4), new Addition(new Variable("a"), new Constant(1)), env);
        testEvaluating(new Constant(6), new Multiplication(new Variable("a"), new Constant(2)), env);
        testEvaluating(new Addition(new Constant(3), new Variable("b")),
                new Addition(new Variable("a"), new Variable("b")), env);  // partially reduced
        testEvaluating(new Constant(0), new Sin(new Subtraction(new Variable("a"), new Constant(3))), env);
        testEvaluating(new Variable("a"), new Variable("a"), new Environment()); // empty env: stays

        // Part 2: assignment stores its value (expected to fail until Assignment.eval stores in vars)
        Environment vars = new Environment();
        new Assignment(new Constant(5), new Variable("x")).eval(vars);
        testEvaluating(new Constant(5), new Variable("x"), vars);
        testEvaluating(new Constant(6), new Addition(new Variable("x"), new Constant(1)), vars);

        // The assigned value is evaluated first: 3 + 4 = y
        new Assignment(new Addition(new Constant(3), new Constant(4)), new Variable("y")).eval(vars);
        testEvaluating(new Constant(7), new Variable("y"), vars);

        // Assigning from another variable: x * 2 = z (x is 5)
        new Assignment(new Multiplication(new Variable("x"), new Constant(2)), new Variable("z")).eval(vars);
        testEvaluating(new Constant(10), new Variable("z"), vars);

        // Reassigning overwrites the old value
        new Assignment(new Constant(1), new Variable("x")).eval(vars);
        testEvaluating(new Constant(1), new Variable("x"), vars);

        // Variable never assigned stays symbolic, others are reduced
        testEvaluating(new Addition(new Constant(1), new Variable("w")),
                new Addition(new Variable("x"), new Variable("w")), vars);

        // A fresh environment knows nothing
        testEvaluating(new Variable("x"), new Variable("x"), new Environment());

        // (5 + x) * 2 with x bound to 8 ==> 26 (example from the instructions, generated with Claude)
        Environment envX = new Environment();
        envX.put(new Variable("x"), new Constant(8));
        testEvaluating(new Constant(26),
                new Multiplication(new Addition(new Constant(5), new Variable("x")), new Constant(2)),
                envX);
    }

    // Generated with Claude
    public static void testEquals(boolean expected, SymbolicExpression a, SymbolicExpression b) {
        if (a.equals(b) == expected) {
            System.out.println("Passed: " + a + " equals " + b + " ==> " + expected);
        } else {
            System.out.println("Error: expected " + expected + " for '" + a + "' equals '" + b + "'");
        }
    }
}