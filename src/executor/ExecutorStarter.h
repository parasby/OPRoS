/*
 * ExecutorStarter.cpp
 *
 *  Created on: 2009. 3. 9
 *      Author: jangcs@etri.re.kr
 */

#ifndef EXECUTORSTARTER_CPP_
#define EXECUTORSTARTER_CPP_

class Executor;
class OSThread;

////////////////////////////////////////////////////////
// SchedulerStarter
class ExecutorStarter
{
public:
	Executor *m_pExecutor;
	OSThread *m_thread;

	void setExecutor(Executor *pExecutor);
	void setThread(OSThread *t);
	void operator() ();
};


#endif /* EXECUTORSTARTER_CPP_ */
