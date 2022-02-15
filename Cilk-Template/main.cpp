
#include <cstdlib>
#include <iostream>
#include <cilk/cilk.h>
#include <malloc.h>
#include "hwtimer.h"
#include <fstream>
#include <math.h>
#include <algorithm>
#include <vector>
using namespace std;

int fib(int n) 
	{
		if (n < 2)
			return n;
		int a = cilk_spawn fib(n-1);
		int b = cilk_spawn fib(n-2);
		cilk_sync;
		return a + b;
	};
	
// if the current array index is < e, keep folding right, if > e, keep folding left.
// stop when the current array index is > e and the index to the left is less than e and vice versa


int BSearch(int *Arr, int e, int n){ 
	//cout << "test Bsearch" << endl;
	//cout << "array B is: ";		
	for(int i = 0; i < n; i++){
			cout << " " << Arr[i];
		}
	cout << endl;
	cout << endl;
	int L = 0;
	int R = n-1;
	int m;
	while(e != Arr[m]){
		//cout << "is this it?" << endl;
		int m = (L+R)/2;
		if(R - L == 0){
			if(e < L){
				return L;
			}
			else{
				return R;
			}
		}
		else if(Arr[m] < e){
			//cout << "test inf loop 1 " << m << " " << L << " " << R << endl;
			L = m+1;
		}
		else if(Arr[m] > e){
			//cout << "test inf loop 2" << endl;
			R = m;
		}
		else{
			return m;
		}
	}
};

int BinSearch(int x, int *T, int p, int r){
	int low = p;
	int high = max(p, r+1);
	int mid = 0;
	while (low < high){
		mid = floor((low+high)/2);
		if(x <= T[mid]){
			high = mid;
		}
		else{
			low = mid+1;
		}
	}
	return high;
	
};
	
// ------------------------------ GO DOWN -----------------------------------------------
// ---------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------

void P_Merge(int *T, int *A, int p1, int r1, int p2, int r2, int p3, int n){
	/*cout << "p2: " << p2 << endl;
	cout << "r2: " << r2 << endl;
	cout << endl << "merge array first element: " << T[r1-1] << endl;
	cout << "merge array second element: " << T[p2] << endl;
	cout << endl;*/
	//cout << "n: " << n << endl;
	
	
	
	// p1, r1, p2, r2 are indeces in Array T
	// p3 will be an index in array A
	/*cout << endl << endl << "elements in A: ";
	for(int i = 0; i < n; i++){
		cout << A[i] << " ";
	}
	cout << endl;*/
	
	int n1 = (r1-p1)+1;
	int n2 = (r2-p2)+1;

	if (n1 < n2) { // if the size of the left half is less than the size of the right
		//cout << "test n1 < n2" << endl;
		cilk_spawn P_Merge(T, A, p2, r2, p1, r1, p3, n); // swap left and right side
	} 
	else if(n1 == 0){ // with the way I have things set up: when will this ever happen...?
		//cout << "test n1==0" << endl;
		return;
	} 
	else {
		
		int q1 = (p1+r1)/2;
		int q2 = BinSearch(T[q1], T, p2, r2); // I think this works when the size = 1?
		int q3 = p3+(q1-p1)+(q2-p2);
		if(q3 > 5){
			cout << endl << "q3 greater than n: " << " A[" << q3 << "]" << endl;
			cout << "q1: " << q1 << endl;
			cout << "q2: " << q2 << endl;
			cout << "p1: " << p1 << endl;
			cout << "r1: " << r1 << endl;
			cout << "p2: " << p2 << endl;
			cout << "r2: " << r2 << endl;
		}
		
		A[q3]=T[q1];
		

		
		cilk_spawn P_Merge(T, A, p1, q1-1, p2, q2-1, p3, n); 
		cilk_spawn P_Merge(T, A, q1+1, r1, q2, r2, q3+1, n); 
		cilk_sync;
		// the current output is: 0 0 3273 15633 33623 89600
		//A[0] = -1; // this makes it: -1 0 0 15633 33623 89600 
	}
};


void P_MergeSort(int *A, int *B, int p, int r, int s){
	// output array B holds sorted result
	// A is the input array
	// p = far left index
	// r = far right index
	// s --> B[s..s+r-p] s = beginning index of B
	int n = r - p + 1;
	if(n==1){ // base case - only 1 element
		B[p] = A[p];
	}
	else{
		int *C;
		//C = (int*) Cilk_alloca(n*sizeof(int));
		C = (int*) alloca(n*sizeof(int));
		int *T = C;
		int q1 = (p+r)/2; // index q1 of A[p...r]
		int q2 = q1-p+1; // this is the number of elements in the first subarray, A[p...q]
	
		cilk_spawn P_MergeSort(A, T, p, q1, s);
		cilk_spawn P_MergeSort(A, T, q1+1, r, q2); 
		cilk_sync;
		
		// C is the array you're merging into, B is the input array, 
		//0, q2, q2+1, and n are indeces, s is the index in B where you
		// start merging into B[s...s+r-p]
		cilk_spawn P_Merge(C, B, p, q1, q1+1, r, s, n);
		cilk_sync;
	}
	
};



	

int main(int argc, char* argv[])
	{
		ifstream inFile("/home/user/Desktop/4009-assignment-2-code/input.txt");
		ofstream outFile("output.txt");
		int n;
		inFile >> n;
		n = 8;
	    int arrA[n];
		int arrB[n];
		cout << n << endl;
		int element;
		int i = 0;
		
		while (inFile >> element){
			if(i < n){
				arrA[i] = element;
				arrB[i] = 0;
			} 
			i++;
		}
		
		cout << "this is the input array: ";
		for(int k = 0; k < n; k++){
			cout << arrA[k] << " ";
		}
		cout << endl;
		
		
		int *Aptr;
		int *Bptr;
		int *ptr;
		//ptr = testArr;
		//ptr = Aptr+5;
		Aptr = arrA;
		Bptr = arrB;
		

		P_MergeSort(Aptr, Bptr, 0, n-1, 0);
		
		cout << endl << "ArrB: ";
		for(int i = 0; i < n; i++){
			cout << Bptr[i] << " ";
		}
		cout << endl;
	
		inFile.close();
		
		
		
		if (argc != 2) {
			cout << "Usage: fib <num>" << endl;
			return 1;
		}
	
		hwtimer_t timer;
		initTimer(&timer);
	
		int param = atoi(argv[1]);
	
		startTimer(&timer);
		int answer = fib(param);
		stopTimer(&timer);
		int fibTime = getTimerNs(&timer);
	
		cout << "fib(" << param << ") = " << answer << endl;
		cout << "Total time: " << fibTime << "ns" << endl;
	
		return 0;
	};
