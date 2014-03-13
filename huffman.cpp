// huffman encoding

#include <iostream> // cin, cout
//#include <fstream>
#include <assert.h> // where has this been all my life????
#include <string> // code is stored as a string
#include <cstring> // for command line args

#include "heap.cpp"
#include "treenode.cpp"


using namespace std;

/* mode */
void compressionMode();
void decompressionMode();

/* codemaking functions */
void huffman(Heap<TreeNode*> &h, int n);
void inorder(TreeNode* t); // for verifying huffman tree
void dfs(TreeNode* t, string *codes, string code);

/* compression functions */
void compress(string *codes, unsigned int *frequencies);
void littleEndian(unsigned int n);
void putInBuffer(string code, bool end = false);
char binaryToDecimal(string code);

/* decompression functions */
unsigned int parseFrequency();
void decompress();
void getBits(string &buffer);
void findChar(TreeNode *node, string &buffer);

int main(int argc, char **argv)
{
	if(argc == 1)
		compressionMode();
	else if(strcmp(argv[1], "-d") == 0)
		decompressionMode();
	
	return 0;
} // main

void compressionMode()
{ // main routine when compressing

	// input part
	
	unsigned int frequencies[256]; // a character could have up to 4^32-1 occurrences...
	fill(frequencies, frequencies + 256, 0);
	
	if(cin.peek() == -1) // EOF
	{
		cout << "HUFFMA5" << '\0';
		for(int i = 0; i < 256 * 4; i++)
			cout.put('\0');
		return;
	} // exceptional case where the file is empty (arguably invalid input...)
	
	char c; // read character by character
	while(cin.get(c)) // takes 2 eofs? what the heck?
	{	
		unsigned char u = c;
		//cerr << (int)u << endl;
		frequencies[(int)u]++;
	}
	cin.clear();
    cin.seekg(0, ios::beg); // reset cin to beginning of stream
	
	// end input part
	
	// heapify part
	
	Heap<TreeNode*> h; // heap will sort the characters by frequency
	int n = 0; // keeps track of the number of nodes in h
	
	for(int i = 0; i < 256; i++)
	{
		if(frequencies[i]) // if frequency > 0
		{
			TreeNode *node = new TreeNode(frequencies[i], (char)i, 0, 0); // put in heap
			h.push(node);
			n++;
		} // if
	} // for
	
	// at this point, we should have a heap containing
	// all appearing characters along with their frequencies
	
	//while(!h.empty())
	//{
	//	cout << h.top()->frequency << ' ' << h.top()->character << endl;
	//	h.pop();
	//}
	
	// huffman tree part
	
	huffman(h, n);
	
	// end huffman tree
	
	// now with our beautiful huffman tree, we need to create the codes
	
	string codes[256]; // will hold our codes
	string code; // empty string
	
	dfs(h.top(), codes, code);
	
	//for(int i = 0; i < 255; i++)
	//	if(!codes[i].empty())
	//		cout << (char)i << ' ' << codes[i] << endl;
	
	//cout << h.top()->frequency << ' ' << h.top()->character << endl;
	
	//inorder(h.top());
	//for(int i = 0; i < 256; i++)
	//	if(!codes[i].empty())
	//		cerr << (char)i << ' ' << codes[i] << endl;
	compress(codes, frequencies);

} // compressionMode

void decompressionMode()
{ // main routine when decompressing
	//cerr << "Sorry Hao, but your decompression routine is in another castle.\n";
	
	// Read and discard the "HUFFMA5\0" string
	for(int i = 0; i < 8; i++)
		cin.get();
			
	// Read all 256 frequencies. Store as TreeNodes if frequency != 0
	unsigned int frequencies[256];
	fill(frequencies, frequencies + 256, 0);
	
	for(int i = 0; i < 256; i++) // for all chars
		frequencies[i] = parseFrequency();
		
	//for(int i = 0; i < 256; i++) // for all chars
	//	cout << frequencies[i] << ' ';	
	//cout << endl;
	
	Heap<TreeNode*> h; // sort nodes in heap
	int n = 0; // number of nodes in heap
	unsigned int totalSize = 0; // keep track of total # of chars
	
	for(int i = 0; i < 256; i++) // create nodes
	{
		if(frequencies[i])
		{
			TreeNode *node = new TreeNode(frequencies[i], (char)i, 0, 0);
			h.push(node);
			n++;
			
			totalSize = totalSize + frequencies[i];
			//cout << totalSize << endl;
		}
	}
	
	// now reuse huffman() to create huffman tree
	huffman(h, n);
			
	// this is where decompression truly diverges. we must use this tree
	// to decode, not encode
	
	/*
	 * CURRENT PLAN: traverse the tree using a buffer of bits
	 *	if(left == 0 && right == 0)
	 * 		return character;
	 *  else // not a leaf
	 * 		if(stillMoreBits)
	 * 			if(nextBit == 1) goLeft (was it left)?
	 * 			else if(nextBit == 0) goRight
	 * 		else // no more bits
	 * 			getMoreBits;
	 * 
	 * 	once we decode totalSize characters, stop calling this
	 */
	 
	 string buffer;
	 
	 for(unsigned int i = 0; i < totalSize; i++)
	 { // convert chars to string bits, use bits to traverse tree
		//getBits(buffer);
		findChar(h.top(), buffer);
	 } // for
	 
	 if(n == 1) // only one char. seems to break my program
	 {
		 for(unsigned int i = 0; i < totalSize; i++)
			cout.put(h.top()->character);
	 }
		 
	 
	 
} // decompressionMode

/* codemaking functions */

void huffman(Heap<TreeNode*> &heap, int n)
{ // straight outta the textbook
	for(int i = 0; i < n - 1; i++)
	{
		TreeNode *first = heap.top(); // required some tweaks to heap
		heap.pop();
		TreeNode *second = heap.top();
		heap.pop(); // take smallest & second-smallest frequencies
		
		int totalFrequency = first->frequency + second->frequency;
		
		char smallestChar;
		if(first->character < second->character) smallestChar = first->character;
			else smallestChar = second->character;
		
		TreeNode *combined = new TreeNode(totalFrequency, smallestChar, first, second);
											
		heap.push(combined);
	} // for
	
	// after huffman's algorithm does its thing, there should only
	// be one node in the tree: the root of the decode tree
								
}

void inorder(TreeNode* t)
{ // print inorder traversal
	
		if(t->left)
			inorder(t->left);
	
	cout << t->character << ' ';
	cout << t->frequency << '\n';
			
		if(t->right)
			inorder(t->right);
} // inorder

void dfs(TreeNode *t, string *codes, string code)
{ // this function will traverse the huffman tree
  // and assign each leaf a unique binary sequence
  
	if(t->left || t->right) // not a leaf, so not a "real" character
	{
		if(t->left)
			dfs(t->left, codes, code + '1');
		if(t->right)
			dfs(t->right, codes, code + '0');
	}
	else // if a leaf
	{
		codes[(int)(t->character)] = code; // save the code
	}
  
} // dfs

/* compression functions */
void compress(string *codes, unsigned int *frequencies)
{ // prints a compressed version of stdin to stdout
	
	cout << "HUFFMA5" << '\0'; // cookie? whatever that means
	for(int i = 0; i < 256; i++)
	{
		littleEndian(frequencies[i]);
	} // print each frequency in little-endian form
	
	// substitute codes. assume cin already reset
	// (which it is)
	
	char c;
	while(cin.get(c))
	{
		unsigned char u = c;
		putInBuffer(codes[(int)u]); // ???
	}
	
	putInBuffer("", true); // stupid, but i need to do this to clear the buffer
	
} // compress

void littleEndian(unsigned int n)
{ // use horner's algorithm base 256 (from ECS 50 ...)

	for(int i = 0; i < 4; i++)
	{
		char c = n % 256;
		cout << c;
		n /= 256;
	} // for

} // littleEndian

void putInBuffer(string code, bool end)
{ // will print a character whenever
  // the buffer string has 8 'bits'
  //cerr << code << endl;
  
  static string buffer;
  buffer = buffer + code; // append bits to buffer
  
  if(end)
  { // end of file: clear buffer
  
	while(buffer.length() >= 8)
	{
		cout.put(binaryToDecimal(buffer.substr(0,8)));
		buffer.erase(0,8);
	} // print any extra bytes built up (a problem with huffman codes larger than 8 bits)
  
	if(buffer.length() == 0)
		return; // don't print an empty buffer!
  
	string padding;
	for(int i = 0; i < 8 - (int)buffer.length(); i++)
	{
		padding += '0';
	}
  
	buffer += padding;
	
	//cerr << buffer << endl;
  
	cout.put(binaryToDecimal(buffer));
	return;
  } // if eof
	  
  
  if(buffer.length() >= 8) // big enough, print
  {
	  //cerr << buffer << endl;
	  cout.put(binaryToDecimal(buffer.substr(0,8)));
	  buffer.erase(0,8); // remove printed part
	  //cerr << buffer << endl;	  
  }
  
} // putInBuffer

char binaryToDecimal(string code)
{ // convert string to actual value

	char place = 1; // start in 1's place
	char total = 0;
	
	for(int i = 0; i < (int)code.length(); i++)
	{
		total += (code[i] - '0') * place;
		place *= 2;
	}
	
	//cerr << (int)total << endl;
	
	return total;

} // binaryToDecimal

/* decompression functions */
unsigned int parseFrequency()
{ // reads next 4 chars and turns them into an unsigned int
	
	unsigned int total = 0;
	unsigned int place = 1;
	
	for(unsigned int i = 0; i < 4; i++)
	{
		unsigned int c = cin.get(); // get next char
		total = total + c * place; // add weighted value
		place *= 256; // next place 
	} // for
	
	return total;
	
} // parseFrequency

void getBits(string &buffer)
{ // convert next char to 8 string "bits" and append to buffer
	//unsigned char c = cin.get();
	// consider changing to if(cin.get(c)) u = c else return
	//if(cin.eof()) return;
	
	char c;
	unsigned char u;
	if(cin.get(c))
		u = c;
	else // failed. eof
		return;
	
	for(int i = 0; i < 8; i++) // this aint right
	{
		string temp; // needed to reverse the bits
		temp = (char)(u % 2 + 48);
		buffer = buffer + temp;
		u = u / 2;
	}
	//cout << buffer << endl;
	//buffer.erase(0,8);
} // getBits

void findChar(TreeNode *node, string &buffer)
{
	
	if(buffer.empty())
		getBits(buffer);
		
	if(buffer.empty()) // if still empty give up
		return;
		
	
	// CURRENT PLAN: traverse the tree using a buffer of bits
	if(node->left == 0 && node->right == 0)
	  	cout.put(node->character);
	else // not a leaf
	{ // we need to go deeper
			char c = buffer[0]; // c is a '0' or '1'
			buffer.erase(0, 1);
	  		if(c == '1')
				findChar(node->left, buffer);
	  		else if(c == '0')
				findChar(node->right, buffer);
	}
	  
	// once we decode totalSize characters, stop calling this
		
	
	
	
} // findChar
