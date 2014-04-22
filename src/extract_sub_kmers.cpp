#include <stdlib.h>
#include <stdio.h>
//#include <iostream.h>
#include <time.h>
#include <math.h>
#include <string.h>
#include <matrix.h>
#include <limits.h>

#include "matrix.h"
#include "mex.h"
#include "general.h"
#include "hmm_chrom_funcs.h"
#include "dna_utils_new.h"
#include "markov.h"
#include "uthash.h" // New! hash table (is this causing us all our memory problems???) 

#undef uthash_expand_fyi
#define uthash_expand_fyi(tbl) printf("expanded to %d buckets\n", tbl->num_buckets)




///////////////////////////////////////////////////////////////////////////////////////
/// Divide a set of sequences to kmers 
///////////////////////////////////////////////////////////////////////////////////////
//#undef MEX_TO_MATLAB
//#ifdef MEX_TO_MATLAB
void mexFunction( int nlhs, mxArray *plhs[],
				 int nrhs, const mxArray *prhs[])
{


	int nRows, nCols, i, j, K;

	long L; 
	word **seqs;
	long *seqs_lens;   // New : There may be different sequences lengths
	long num_seqs; 
	//  long seqs_len; 
	//	word **kmers; // output kmers 

	double *in, *inn, *out;
	word *in_w, *out_w;
	long total_num_kmers = 0; 
	long kmer_length_in_words; 
	long unique_flag=0; // do we want unique kmers (and keep their indices) 
	long num_unique_kmers, num_sparse_matrix_elements;
	long hash_flag; //  = 1; 
	long half_word_size = 4*sizeof(word); // half word size in bits (=word size in nucleotides)
	long word_size = 2*half_word_size;
	long num_coordinates; // counts number of output coordinates 
	long *kmer_position_inds;
	long *kmer_seq_inds; // = new long[total_num_kmers]; 
	long input_position_flag=0; // flag saying 	
	long debug_prints=0; // default is no printing  


	
		
		/*************************************************  Read Input **********************************************************************/
	/* Check for proper number of arguments. Should be five 5 (last one is double flag) */
	/* Input variables are: pwms, seqs, seqs_lens, -10, is_double */
	if(nrhs < 5) {
		mexErrMsgTxt("Usage: [seqs_packed], [lens], [K], [unique-flag], [hash-flag]");
	}


	//		(word *)(mxGetPr(prhs[0]));   // Get the sequences matrix (in packed form) 	  
	in = mxGetPr(prhs[2]);   // Get the kmer length (L)  	  
	L	= in[0]; 
	in = mxGetPr(prhs[3]);   // Get the unique flag    	  
	unique_flag = in[0]; 
	in = mxGetPr(prhs[4]);   // Get the hash flag. New! if length is 2, then second value is print flag!!!  	  


	hash_flag = in[0]; 
	long hash_flag_size = MAX(mxGetM(prhs[4]), mxGetN(prhs[4]));
	if(hash_flag_size>1) // Get debug-print flag. 2nd argument of hash flag 
		debug_prints = in[1];

	/**/
	if(debug_prints)
	{
		printf("Start mex function\n");	
		fflush(stdout);
		printf("Size of Word=%ld (bits), Size of long=%ld\n", 8*sizeof(word), 8*sizeof(long)); // print size in bits
		fflush(stdout);
	}
	/**/



	num_seqs = mxGetM(prhs[0]);    // Get number of different sequences (genes)
	in_w = (word *)(mxCalloc(num_seqs, sizeof(word))); // allocate memory to read seqs

	seqs_lens = new long[num_seqs]; // allocate the lengths array
	inn = mxGetPr(prhs[1]);   // Get the sequences original lengths in nucleotides (not packed ..) 	  	  
	//  seqs_len = (long)(inn[0]); // Give to the function the original lengths (pointer?)
	long  num_seq_lens = MAX(mxGetM(prhs[1]), mxGetN(prhs[1])); // Get number of different lengths
	for(i = 0; i < num_seqs; i++)
	{
		if(num_seq_lens>1) // read different lengths 
			seqs_lens[i] = inn[i]; // different seqs_len;    
		else   // All genes are assumed to have the same length
			seqs_lens[i] = inn[0]; // all are the same length 
		total_num_kmers += (seqs_lens[i]-L+1); // don't take from sequences ends 
		//		printf("Current seq-len=%ld, total=%ld, L=%ld num_lens=%ld\n", seqs_lens[i], total_num_kmers, L, num_seq_lens); 
		//		fflush(stdout);
	}


	//	if ( mxIsUint32(prhs[0]) != 1)
	//      mexErrMsgTxt("Input must be a 32-bit integer!!! ");

	if(nrhs > 5) // here we get also input coordinates 
	{	
		input_position_flag=1; // here we pick the positions where we extract the kmers 
		in = mxGetPr(prhs[5]);   // Get the kmer length (L)  	  
		num_coordinates = MAX(mxGetM(prhs[5]), mxGetN(prhs[5])); 
		total_num_kmers = num_coordinates; 
	}
	else
		num_coordinates = 1;


	//	printf("Obtaining %ld coordinates, %ld total kmers\n", num_coordinates, total_num_kmers); 
	//	fflush(stdout);
	kmer_seq_inds = new long[total_num_kmers];
	//	if(!input_position_flag)
	//		kmer_seq_inds = new long[total_num_kmers]; 
	kmer_position_inds = new long[num_coordinates]; 

	if(nrhs > 5) // read input coordinates 
	{	
		for(i=0; i<num_coordinates; i++)
			kmer_seq_inds[i] = in[i]-1; // subtract one (matlab->c) 
		in = mxGetPr(prhs[6]);   // Get the kmer length (L)  	  
		for(i=0; i<num_coordinates; i++)
			kmer_position_inds[i] = in[i]-1; // subtract one (matlab->c) 
	}


	if(debug_prints)
	{	
		printf("Allocate seqs lengths  %ld\n", num_seqs);
		fflush(stdout);
	}




	//	printf("Inputs: kmer-length=%ld, unique-flag=%ld, hash-flag=%ld\n", L, unique_flag, hash_flag); 
	//	fflush(stdout); 



	word **kmers = new word*[total_num_kmers]; 	//  	word **kmers = (word **) mxMalloc( total_num_kmers * sizeof(double));
	long *kmer_kmer_inds = new long[total_num_kmers]; 



	//	printf("THE SEQS LEN IS : %ld\n", seqs_lens[0]);	
	//	fflush(stdout);
	in_w = (word *)(mxGetData(prhs[0]));   // Get the sequences matrix (in packed form) 	  
	seqs = new word*[num_seqs]; // read input sequences (should be word. I.e. unsigned int) 
	for(i = 0; i < num_seqs; i++)
	{
		seqs[i] = new word[seqs_lens[i]];   // allocate memory for every sequence
		for(j = 0; j < (seqs_lens[i]-1)/half_word_size+1; j++)
			seqs[i][j] = in_w[j*num_seqs + i];  // Copy the sequences matrix (Note that they should be packed for now !!!)
	}


	// print to see that input came right
	if(debug_prints>1)
	{
		printf("Print input DNA: First Seq=%ld \n", seqs[0][0]); 
		for(i=0; i<num_seqs; i++)
			print_packed_dna(seqs[i], seqs_lens[i], 1);
		fflush(stdout);	
	}
	/*************************************************  Finished Reading Input ***************************************************************/


	kmer_length_in_words = ceil(double(L)/double(half_word_size));
	//	printf("kmer length in words =%ld\n", kmer_length_in_words);

	for(i = 0; i < total_num_kmers; i++)
		kmers[i] = new word[kmer_length_in_words]; //   (word *) mxMalloc( kmer_length_in_words * sizeof(word));		


	//	*intersect_start_pos = (double *) mxMalloc( (n1+n2) * sizeof(double));
	/*
	kmers = new word*[total_num_kmers]; // this may be too much - what to do? allocate on the fly? 
	for(i = 0; i < total_num_kmers; i++)
	kmers[i] = new word[kmer_length_in_words]; 
	*/
	//	printf("L is %d, num_seqs %d, seqs_len %d, total_kmers %d kmer-in-word %d unique_flag %ld\n", 
	//		L, num_seqs, seqs_len, total_num_kmers, kmer_length_in_words, unique_flag);


	//	printf("Extract sub-kmers\n");
	//	fflush(stdout);

	//	if(input_position_flag)
	//		return;
	//	else


/** Dpn't skip the extract kmers part **/
	extract_sub_kmers(L, seqs, seqs_lens, num_seqs, unique_flag, 
		kmers, kmer_kmer_inds, kmer_seq_inds, kmer_position_inds, &num_unique_kmers, 
		&num_sparse_matrix_elements, hash_flag, input_position_flag, num_coordinates, debug_prints);
/**/


	if(debug_prints)
	{
		printf("Finished extract kmers\n"); 
		fflush(stdout); 
	}


	/*
	for(i=0; i<num_seqs; i++) 
	for(j=0; j<5; j++)
	printf("original word = %lu\n", seqs[i][j]);
	*/

	// free memory
	for(i=0; i<num_seqs; i++) 
		delete seqs[i];
	delete seqs; 
	delete seqs_lens;
	mwSize mw_total_num_kmers = total_num_kmers;
	mwSize mw_kmer_length_in_words = kmer_length_in_words;

	/*
	for(i=0; i<total_num_kmers; i++) 
	for(j=0; j<kmer_length_in_words; j++)
	printf("%ld\n", kmers[i][j]);
	*/

	//	mwSize mw_dims[2];
	//	mw_dims[0] = mw_total_num_kmers; mw_dims[1] = mw_kmer_length_in_words;


	if(debug_prints)
	{
		printf("Num. unique kmers=%ld \n", num_unique_kmers); 
		fflush(stdout); 
	}

	long return_double = 0;
	if(return_double)
	{
		plhs[0] = mxCreateDoubleMatrix(num_unique_kmers, mw_kmer_length_in_words, mxREAL); //	  plhs[0] = mxCreateNumericArray(2, mw_dims, mxUINT32_CLASS, mxREAL);
		out = mxGetPr(plhs[0]); // output the scores array 
		for(i=0; i<num_unique_kmers; i++) 
			for(j=0; j<kmer_length_in_words; j++)
				out[i+j*num_unique_kmers] = double(kmers[i][j]); // copy kmers array to output // 0*j*total_num_kmers+
	}
	else // here return integers
	{
		//		printf("Create Output Matrix[%ld,%ld] \n",num_unique_kmers, mw_kmer_length_in_words); 
		//		fflush(stdout); 
		if(word_size == 32)
		  {
//		    		    printf("Return word-size=32\n");
			plhs[0] = mxCreateNumericMatrix(num_unique_kmers,mw_kmer_length_in_words, mxUINT32_CLASS, mxREAL);
		  }
		else
		  {
//		    		    printf("Return word-size64\n");

			plhs[0] = mxCreateNumericMatrix(num_unique_kmers,mw_kmer_length_in_words, mxUINT64_CLASS, mxREAL);
		  }

		//		printf("Copy to Output Matrix[%ld,%ld] \n",num_unique_kmers, mw_kmer_length_in_words); 
		//		fflush(stdout); 
		out_w = (word *)(mxGetData(plhs[0]));
		for(i=0; i<num_unique_kmers; i++) 
		{
			//			printf("Copy kmer %ld\n", i); 
			for(j=0; j<kmer_length_in_words; j++)
				out_w[i+j*num_unique_kmers] = (kmers[i][j]); // copy kmers array to output // 0*j*total_num_kmers+
		}
	}


	for(i = 0; i < total_num_kmers; i++)
		delete kmers[i]; 			  
	delete kmers; //   mxFree(kmers);

	//  printf("one Shift 32 is: %lu, and %lu\n", 1UL << 32, 143241231 + (1UL << 32)); 
	plhs[1] = mxCreateDoubleMatrix(num_sparse_matrix_elements, 2, mxREAL); //		plhs[1] = mxCreateNumericMatrix(total_num_kmers, 1, mxINT32_CLASS, mxREAL);

	out = mxGetPr(plhs[1]); // output the scores array 
	for(i=0; i<num_sparse_matrix_elements; i++) 
	{
		//		for(j=0; j<2; j++)
		out[i+0] = double(kmer_kmer_inds[i]+1); // copy kmer indices array to output. add 1 (indices in matlab start at 1) 
		out[i+num_sparse_matrix_elements] = double(kmer_seq_inds[i]+1); // copy kmer sequences indices array to output. add 1 (indices in matlab start at 1) 
		// out[i+2*num_sparse_matrix_elements] = double(kmer_positions_inds[i]+1); // new!! also allow return position indices  (need to allocate 3 instead of 2 for this) 
	}
	//	for(i = 0; i < total_num_kmers; i++)
	//		delete kmer_inds[i]; 
	delete kmer_kmer_inds; 
	delete kmer_seq_inds; 
	delete kmer_position_inds;

	//	for(i=0; 0<1; i++) // infinite loop 
	//		j=i; // sleep(1000); 



}
//#endif // MEX_TO_MATLAB


