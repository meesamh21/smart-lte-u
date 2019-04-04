/*
 * Agent.cc
 *
 *  Created on: Apr 15, 2017
 *      Author: meesam
 */

#include "Agent.h"

const float GAMMA = 0.6;

const float maxQTEMPERATURE = 1.0 / 2;
const float minQTEMPERATURE = 1.0 / 50;

        // still retains *somewhat* stochastic policy


// each (x,a) has its own varying ALPHAQ:

float alphaQ ( long int i )
{
 return ( 1.0 / i );
}




class Agent
{
public:
// keep track of Q-values for my actions:

        StateActionSpace        Q;

// what really defines me is my reward function
// (defined in subclasses):

        virtual float  reward ( state x, state y ) { return 0; }

// noQ(x,a) counts the no. of times we have "visited (x,a)"
// (tried out action a in state x)
// so e.g. we can have declining alphaQ:

        StateActionSpace        noQ;

// the action I suggest to execute:

        action         ai;

// temporary variable:

        action         af;


 alloc()
 {
   Q.allocvector ( cf, df );
  noQ.allocvector ( cf, df );
   ai.allocvector ( df );
   af.allocvector ( df );
 }


 int j;
 #define foractions_j           for ( j=0; j<=(ai.no-1); j++ )

 int randomAction() { return randomAtoB ( 0, ai.no-1 ); }
};




Agent :: updateQ ( state x, action a, state y )
{
  float r = reward(x,y);                // defined later
  float total = r + (GAMMA * Q.max(y));

  noQ.at(x,a)++;
  float ALPHA = alphaQ ( noQ.at(x,a) );

  Q.at(x,a) = ((1-ALPHA) * Q.at(x,a)) + (ALPHA * total);
}




// first, the sum of the exp(Q/T) terms:

Agent :: calculateSigma ( state x, float QTEMPERATURE )
{
 sigma = 0;
 foractions_j
 {
  af.from(j);
  sigma = sigma + exp ( Q.at(x,af)/QTEMPERATURE );
 }
}

// can show how probable each action is to be tried:

Agent :: printProb ( ostream& stream, state x, float QTEMPERATURE )
{
 foractions_j
 {
  af.from(j);
  double prob = exp ( Q.at(x,af)/QTEMPERATURE ) / sigma;
 }
}


// suggest an action ai:

Agent :: suggestBoltz ( state x, float QTEMPERATURE )
{
 calculateSigma ( x, QTEMPERATURE );
 float p = random0to1exclusive();
 float sum = 0;
 j = 0;

 while ( sum < p )
 {
  af.from(j);
  double prob = exp ( Q.at(x,af)/QTEMPERATURE ) / sigma;
  sum = sum + prob;
  j++;
 }

 // just hit p
 ai = af;
}



// suggest action with reasonable (declining) temperature:

Agent :: suggestReasonable ( state x )
{
 suggestBoltz ( x, reasonableTemperature() );
}


float Agent :: reasonableTemperature()
{
 long int total = noQ.totalNumberOfExperiences();

 if ( total >= ceiling )
  return minQTEMPERATURE;
 else
 {
  float e = total / ceiling;
  return ( minQTEMPERATURE + (1-e)*(maxQTEMPERATURE-minQTEMPERATURE) );
 }
}



// no exploration, demo mode:

Agent :: exploit ( state x )
{
 suggestBoltz ( x, minQTEMPERATURE );
}



class Creature
{
protected:
 state          s;       // temporary variables
 state          x;
 state          y;

 action ak;             // each Agent suggests an action ai
                                // one action ak wins and is executed

 virtual        observe() {}         // observe() fills up state s
 virtual        execute ( action a ) {}


 AgentArray     A;              // a list of agents 1..n

 #define foragents_i            for ( int i=1; i<=A.n; i++ )

 int    randomAgent() { return randomAtoB ( 1, A.n ); }

public:

 Creature()
 {
  s.allocvector ( cf );
  x.allocvector ( cf );
  y.allocvector ( cf );
  ak.allocvector ( df );
 }
};






// interact with the world just once:

Creature :: interact ( int mode )
{
// observe state, each agent suggests an action:

 observe(); x = s;

 if (mode == _learnQ)
 {
  ak[1] = randomAction();
 }
 else if (mode == _exploit)
 {
  foragents_i
   A[i]->exploit(x);
 }
 else
 {
  foragents_i
   A[i]->suggestReasonable(x);
 }


// somehow go through the ai's
// and pick an agent and execute its action:

 execute(ak);


// observe new state, all agents learn:

 observe(); y = s;

 if (mode == _learnQ)
  foragents_i
   A[i]->updateQ ( x, ak, y );
}
