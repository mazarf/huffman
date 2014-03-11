/*
 * THIS CLASS REQUIRES OVERLOADING <=, <, AND >
 */

#include <iostream>
using namespace std;

template < class T >
class Heap
{
	public:
		Heap(int n = 1);
		~Heap();
		void push(T entry);
		void pop();
		T top();
		void print();
		bool empty();
	private:
		T *data;
		int max;
		int nodes;
		void expand();
}; // heap

template < class T >
Heap<T>::Heap(int n)
{
	if(n < 1) {
		cout << "heap must be larger than 1" << endl;
	} else {
		data = new T[n];
		max = n;
		nodes = 0;
	}	
	
} // constructor

template < class T >
Heap<T>::~Heap()
{
	delete[] data;
} // destructor

template < class T >
void Heap<T>::push(T entry)
{ // bubble up

	if(nodes == max)
		expand();
		
	int location = nodes; // place new node at end of complete tree
	while(location != 0) // not at root
	{ // move up if parent bigger than node
		if((*data[location / 2]) > (*entry))
		{
			data[location] = data[location / 2]; // shift down
			location = location / 2; // go up a level
		}
		else
			break;
	}
	
	data[location] = entry; // insert
	nodes++;
} // push

template < class T >
void Heap<T>::pop()
{ // trickle down

	if(nodes == 0)
		return;

	data[0].~T(); // delete if applicable
	int location = 0; // start at root
	int child  = 1;
	T temp = data[nodes - 1];
	nodes--;
	
	// look at smallest child. if smaller than last entry
	// shift up and continue down. otherwise stop
	while(child <= nodes)
	{
		if(child < nodes && (*data[child]) > (*data[child + 1]))
			child++;

		if((*temp) <= (*data[child])) break;

		data[location] = data[child];
		location = child; child *= 2;

	}
	
	data[location] = temp;
	
	return;
} // pop


template < class T >
T Heap<T>::top()
{
	if(nodes == 0)
		throw "empty";
	
	return data[0];
} // top

template < class T >
void Heap<T>::expand()
{ // increases array size when max exeeded
	T *temp = new T[2*max];
	
	for(int i = 0; i < max; i++)
		temp[i] = data[i];
	
	delete[] data;
	data = temp;
	max*=2;
} // expand

template < class T >
void Heap<T>::print()
{
	int j = 1;
	int k = 2;
	
	for(int i = 0; i < nodes; i++)
	{
		cout << data[i] << ' ';
		j++;
		if(j % k == 0)
		{ 
			cout << endl;
			k *= 2; // see i can make shitty unclear code too
		}
	}
} // print

template < class T >
bool Heap<T>::empty()
{
	if(nodes == 0)
		return true;
		
	return false;
}
