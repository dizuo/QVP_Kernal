#include <iostream>
#include "agraph_exec.hh"
#include "graph_exec.hh"

#include "../factory/aflow.hh"
#include "../factory/flow.hh"
#include "../factory/process.hh"
#include "../factory/aprocess.hh"

#include "test/IntAdder.hh"
#include "test/IntSous.hh"
#include "test/IntRandomizer.hh"
#include "test/IntDisplayer.hh"
#include "test/IntDisplayerOut.hh"
#include "test/IntFlows.hh"

#include <Windows.h>
#include <crtdbg.h>

using namespace std;

/* simple exemple the randomizer generate the integer 5 and send it in the displayer */
/*
** this simple exemple show how connecting process between them with a flow
** and execute it.
*/
void	example1()
{
	cout << endl << "===================" << endl;
	cout << "Example 1 (print 5)" << endl;
	cout << "===================" << endl;
 	AGraphExec *G = new GraphExec();
 	unsigned int	process[2];		// contains process id
 	unsigned int	flow[1];		// contains flows id

	AProcess* pRandom = new IntRandomizer();
 	AProcess* pDisplayer = new IntDisplayer();
 	AFlow*	pInt = new IntFlow;

	process[0] = G->add_process(pRandom); // add a new process in the graph of processes
	process[1] = G->add_process(pDisplayer);  // add a new process in the graph of processes
	flow[0] = G->add_flow(pInt); // add a new flow in the graph of processes

	// at this point, all elements are disconnected

	G->hang_flow_in(process[1], flow[0], 0);  // connect the input of the displayer with the flow
	G->hang_flow_out(process[0], flow[0], 0); // connect the output of the randomizer with the other flow's extremity
	G->exec(); // execute the graph of processes.

// 	G->hang_flow_in(4, flow[0], 0);  // try to connect with bad proc ID
// 	cout << G->error(); // print the error message
// 	cout << G->error(); // the error message is void
	
	G->modify_proc(process[0]);
	G->exec();

	std::cout << std::endl;
	delete G;
}


/* example with more processes 5 + 5 -> print result */
/*
** This example show that if no modifications are applied
** on the graph, the execution do nothing. 
*/
void example2()
{
	cout << endl << "============================" << endl;
	cout << "Example 2 print (add(5, 5)))" << endl;
	cout << "============================" << endl;
	AGraphExec *G = new GraphExec();
	unsigned int	process[4];
	unsigned int	flow[3];

	process[0] = G->add_process(new IntRandomizer());
	process[1] = G->add_process(new IntRandomizer());
	process[2] = G->add_process(new IntAdder());
	process[3] = G->add_process(new IntDisplayer());

	flow[0] = G->add_flow(new IntFlow());
	flow[1] = G->add_flow(new IntFlow());
	flow[2] = G->add_flow(new IntFlow());

	G->hang_flow_in(process[3], flow[2], 0);  // hang the displayer
	G->hang_flow_out(process[2], flow[2], 0); // hang the add with displayer

	G->hang_flow_in(process[2], flow[1], 1);
	G->hang_flow_out(process[0], flow[1], 0);

	G->hang_flow_in(process[2], flow[0], 0);
	G->hang_flow_out(process[1], flow[0], 0);
	cout << "first execution (do)" << endl; /* do */
	G->exec();
	cout << "second execution (do nothing)" << endl; /* do nothing */
	G->exec();
	delete G;
}


/* test the execution of needed process */
/*
** This example show that only new process 
** and his subtree is executed 
*/

void example3()
{
	cout << "=========" << endl;
	cout << "Example 3" << endl;
	cout << "=========" << endl;
	AGraphExec *G = new GraphExec();
	unsigned int	process[8];
	unsigned int	flow[5];
	AProcess* proc =  new IntDisplayerOut();
	
	AProcess* pAdder = new IntAdder;

	AProcess* changedProcess = new IntRandomizer();

	process[0] = G->add_process(changedProcess);
	process[1] = G->add_process(new IntRandomizer());
	process[2] = G->add_process(pAdder);
	process[3] = G->add_process(new IntDisplayer());
	process[4] = G->add_process(proc);
	process[5] = G->add_process(new IntDisplayerOut());

	flow[0] = G->add_flow(new IntFlow());
	flow[1] = G->add_flow(new IntFlow());

	IntFlow* terminal = new IntFlow();
	
	flow[2] = G->add_flow(terminal);
	flow[3] = G->add_flow(new IntFlow());
	flow[4] = G->add_flow(new IntFlow());

	G->hang_flow_in(process[3], flow[2], 0);  // hang the displayer
	G->hang_flow_out(process[2], flow[2], 0); // hang the add with displayer

	G->hang_flow_in(process[2], flow[1], 1);  // hang adder with displayerout
	G->hang_flow_out(process[4], flow[1], 0); // hang displayerout with adder

	G->hang_flow_in(process[2], flow[0], 0);  // hang adder with displayerout
	G->hang_flow_out(process[5], flow[0], 0); // hang displayerout with adder

	G->hang_flow_in(process[5], flow[3], 0);  // hang generator with displayerout
	G->hang_flow_out(process[0], flow[3], 0); // hang displayerout with generator

	G->hang_flow_in(process[4], flow[4], 0);  // hang generator with displayerout
	G->hang_flow_out(process[1], flow[4], 0); // hang displayerout with generator

	G->exec();

	// 从flow中提取数据，然后显示，正是IntDisplayer所做的事。
	// 下面为暴力提取：
	// cout << "Get value from Flow: " << *terminal->get_value() << endl;

	//////////////////////////////////////////////////////////////////////////
	std::cout << "----------------------" << "Change Operator";
	std::cout << "----------------------" << std::endl;

	G->del_process(process[2]);
	delete pAdder;	//需要显式地释放pAdder
	process[6] = G->add_process(new IntSous());

	G->hang_flow_out(process[6], flow[2], 0); // hang the add with displayer
	G->hang_flow_in(process[6], flow[1], 1);  // hang adder with displayerout
	G->hang_flow_in(process[6], flow[0], 0);  // hang adder with displayerout
	G->exec();

	//////////////////////////////////////////////////////////////////////////
	std::cout << "----------------------" << "Change Data";
	std::cout << "----------------------" << std::endl;

	//// wrong
	//for (int i(2); i<10; i++)
	//{
	//	IntRandomizer* p = dynamic_cast<IntRandomizer*>(changedProcess);
	//	p->setValue(i);
	//
	//	G->modify_proc(process[0]);
	//	G->exec();
	//}

	G->del_process(process[0]);
	delete changedProcess;

	process[7] = G->add_process(new IntRandomizer());
	G->hang_flow_out(process[7], flow[3], 0);
	G->exec();

	//G->print(cout);

	delete G;
}

void	example4()
{
	cout << "=========" << endl;
	cout << "Example 4" << endl;
	cout << "=========" << endl;
	AGraphExec *G = new GraphExec();
	unsigned int	process[2];
	unsigned int	flow[1];

	process[0] = G->add_process(new IntRandomizer());
	process[1] = G->add_process(new IntDisplayer());
	flow[0] = G->add_flow(new IntFlow());

	G->hang_flow_in(process[1], flow[0], 0);  // connect the input of the displayer with the flow
	G->hang_flow_out(process[0], flow[0], 0); // connect the output of the randomizer with the other flow's extremity
	G->print(cout);

	G->del_flow(flow[0]);
	cout << "Delete the flow" << endl;
	G->print(cout);

	cout << "==================" << endl;
	flow[0] = G->add_flow(new IntFlow());

	G->hang_flow_in(process[1], flow[0], 0);  // connect the input of the displayer with the flow
	G->hang_flow_out(process[0], flow[0], 0); // connect the output of the randomizer with the other flow's extremity
	G->print(cout);
	cout << "Delete process " << endl;
	G->del_process(process[0]);
	G->print(cout);
}


int	main()
{
	//char a[] ="a" 
	//			"b"
	//			"c"
	//			"d"
	//			"f";
	//cout << a << endl;

	_CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );

	// 不支持类型的隐式转化
	//AFlow* floatFlow = new Flow<float>();
	//cout << ( floatFlow->is<int>() ? "int" : "float" )<< endl;
	//delete floatFlow;

	example1();

	//example2();

	//example3();

	//example4();

	system("PAUSE");

	return 0;
}


