.PHONY: all run clean compile-tests test test-assert

all:
	javac -d classes -sourcepath . org/ioopm/calculator/Calculator.java

run:
	java -cp classes org.ioopm.calculator.Calculator

compile-tests:
		javac -d classes -sourcepath . Test.java

test: compile-tests 
		java -cp classes Test 

test-assert: compile-tests 
		java -enableassertions -cp classes Test

clean:
	rm -rf classes
	find . -name "*.class" -delete
