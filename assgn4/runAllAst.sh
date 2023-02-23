#!/bin/bash

	echo "--------------------Testing testArrayIndex-------------------------"
	./obj/mcc --ast test/cases/testArrayIndex.mC #> all.out
#	diff -s all.out ./test/exp/testArrayIndex.exp

	echo "--------------------Testing testFuncCallMatch-------------------------"
	./obj/mcc --ast test/cases/testFuncCallMatch.mC #> all.out
#	diff -s all.out ./test/exp/testFuncCallMatch.exp

	echo "--------------------Testing testMulDeclFunc-------------------------"
	./obj/mcc --ast test/cases/testMulDeclFunc.mC #> all.out
#	diff -s all.out ./test/exp/testMulDeclFunc.exp

	echo "--------------------Testing testMulDeclVars-------------------------"
	./obj/mcc --ast test/cases/testMulDeclVars.mC #> all.out
#	diff -s all.out ./test/exp/testMulDeclVars.exp

	echo "--------------------Testing testTypeMismatch-------------------------"
	./obj/mcc --ast test/cases/testTypeMismatch.mC #> all.out
#	diff -s all.out ./test/exp/testTypeMismatch.exp

	echo "--------------------Testing testUnDeclFunc-------------------------"
	./obj/mcc --ast test/cases/testUnDeclFunc.mC #> all.out
#	diff -s all.out ./test/exp/testUnDeclFunc.exp

	echo "--------------------Testing testUnDeclVars-------------------------"
	./obj/mcc --ast test/cases/testUnDeclVars.mC #> all.out
#	diff -s all.out ./test/exp/testUnDeclVars.exp

