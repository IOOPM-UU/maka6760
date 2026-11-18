package org.ioopm.calculator.ast;

import java.util.HashMap;
import org.ioopm.calculator.ast.atom.Variable;

public class Environment extends HashMap<Variable, SymbolicExpression> {}
