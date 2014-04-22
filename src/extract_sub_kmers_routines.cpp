#include <stdlib.h>
#include <stdio.h>
//#include <iostream.h>
#include <time.h>
#include <math.h>
#include <string.h>
#include <matrix.h>
#include <limits.h>

#include "matrix.h"
#include "general.h"
#include "hmm_chrom_funcs.h"
#include "dna_utils_new.h"
#include "markov.h"
#include "uthash.h" // New! hash table (is this causing us all our memory problems???) 

#undef uthash_expand_fyi
#define uthash_expand_fyi(tbl) printf("expanded to %d buckets\n", tbl->num_buckets)

long TOTAL_NUM_SPLITS; 
long TOTAL_NUM_QSORTS;
long TOTAL_NUM_SWAPS; 
long TOTAL_NUM_NO_SWAPS; 

// Hash table structures 
struct SeqData {
	int seqpos;
	word seq[MAX_L]; /* need to set the right length */
	UT_hash_handle hh; /* makes this structure hashable */
	int rand_int; /* temp for debug */
};
struct SeqData *hTable=NULL;





// free hash memory
void delete_all() {
	struct SeqData *current_user, *tmp;

	HASH_ITER(hh, hTable, current_user, tmp) {
		HASH_DEL(hTable,current_user);  /* delete; users advances to next */
		free(current_user);            /* optional- if you want to free  */
	}
}



// 
// 
////////////////////////////////////////////
//
//  function : extract_sub_kmers
// 
//  input    : L - length of kmers to extract (in nucleotides)
//			   seqs - array of sequences (packed form 2-dimensional array)
//			   seqs_lens - array of sequences lengths (num. of nucleotides in each sequence) 
//			   unique_flag - if 'ON' keep a unique list of the kmers and save indices showing multiple occurances of each kmer
//			
//			   hash_flag - do we use hash in program? (currently always use hash)						   
//			   input_position_flag - use only input positions and don't scan entire sequence
//			   num_coordinates - ??? 
//	
//
//  output   : kmers - array of unique kmers (in packed form 2-dim array) 
//			   kmer_kmer_inds - array of indices for each kmer. Index in the list of unique kmers
//			   kmer_seq_inds - array of indices for each kmer. Index is in the list of sequences (species) 
//			   kmer_position_inds - array of indices for each kmer. Index is the position in the sequences (in each species)
//			   num_unique_kmers - total number of unique kmers
//			   num_sparse_matrix_elements - total number of (non-unique) kmers to be used in sparse matrix
//
//  purpose  : Scan sequences and extract all subsequences of length L. 
//			   Use a hash table to speed the process.   
//			   
//
////////////////////////////////////////////

long extract_sub_kmers(long L, word *seqs[MAX_NUM_SEQS], long *seqs_lens, long num_seqs, long unique_flag, 
					   word *kmers[MAX_L], long *kmer_kmer_inds, long *kmer_seq_inds, long *kmer_position_inds, 					   
					   long *num_unique_kmers, long *num_sparse_matrix_elements, long hash_flag, 
					   long input_position_flag, long num_coordinates, long debug_prints)
{
	long i, j, k;
	long i_seq, j_position; 
	long num_kmers_enum;
	long half_word_size = 4*sizeof(word);
	long word_size = 2*half_word_size;

	long num_words_in_kmer = ceil(double(L) / double(half_word_size)); 
	long num_bytes_in_kmer = ceil(double(L) / 4.0); // temp - a bit wasteful but easier to make sure it's correct 
	long num_bytes_in_kmer_ceil;

	if(word_size == 32)
	 num_bytes_in_kmer_ceil = 4 * ceil(double(L) / 16.0); // temp - a bit wasteful but easier to make sure it's correct (4 bytes in word)
	else
	 num_bytes_in_kmer_ceil = 8 * ceil(double(L) / 32.0); // temp - a bit wasteful but easier to make sure it's correct (8 bytes in word)


	long last_word_in_kmer = (L-1)%half_word_size+1; 
	long kmer_ctr=0; // count which unique kmer are we at 
	long index_ctr=0; // count which index are we at the sequence (non-unique kmer) 
	struct SeqData *found_hash_ptr;
	struct SeqData *temp_ptr;
	long current_idx_in_hash;
	word current_kmer[MAX_L]; // save one kmer 
//	long debug_prints = 0; 
	long rand_int2;
	long add_int = 0; 
	word ones_mask;
	if(word_size == 32)
		ones_mask = 0xFFFFFFFF;
	else // assume it's 64 
		ones_mask = 0xFFFFFFFFFFFFFFFF;
	double start_time = clock(); 
	long cur_time; 


	if(debug_prints)
	{
		printf("Inside extract kmers\n"); 
		printf("ONES_MASK=%lx\n", ones_mask); 
		printf("ONES_MASK_DEC=%lu\n", ones_mask); 
		printf("SIZEOFLONG=%ld (bits)\n", 8*sizeof(long)); 
		printf("kmer_len=%ld, Num. Words in kmer=%ld, num. bytes=%ld num. bytes to copy=%ld\n", L, num_words_in_kmer, num_bytes_in_kmer, num_bytes_in_kmer_ceil); 
		fflush(stdout); 

	}

	long num_seqs_enum = num_seqs;
	//	printf("Input positions = %ld\n", input_position_flag); 
	if(input_position_flag)
	{
		num_seqs_enum = num_coordinates;
		if(debug_prints)
		{
			for(i=0; i<num_coordinates; i++)
				printf("Pos %ld=%ld\n", i, kmer_position_inds[i]); 

			printf("Run on %ld Sequences\n", num_seqs_enum); 
			for(i = 0; i < num_seqs_enum; i++) // loop on all sequences 
			{
				printf("Before loop position ind %ld= %ld\n", i, kmer_position_inds[i]);
				fflush(stdout); 
			}
		}
	}

	if(debug_prints)
		printf("Before main loop. Time=%lf Sec.\n", (cur_time-start_time) / CLOCKS_PER_SEC); fflush(stdout);
	for(i = 0; i < num_seqs_enum; i++) // loop on all sequences 
	{
		if(i%50 == 0)
			if(debug_prints)
			{
				printf("Run on seq %ld\n", i); 
				fflush(stdout); 
			}
		if(input_position_flag)
		{
			i_seq = kmer_seq_inds[i]; 
			num_kmers_enum = 1;
//			printf("Cur kmer ind: %ld\n", i_seq);
//			fflush(stdout); 

		}
		else
		{
			i_seq = i;
			num_kmers_enum = seqs_lens[i]-L+1;
		}
		if(i%50 == 0)
			if(debug_prints)
			{		
				printf("Cur seq ind: %ld\n", i_seq);
				fflush(stdout); 
			}
		for(j = 0; j < num_kmers_enum; j++) // loop on all possible kmers in a sequence 
		{
			if(input_position_flag)
				j_position = kmer_position_inds[i]; 
			else
				j_position = j;
			//			printf("Use words: %lu %lu, kmer_ctr=%ld \n", seqs[i][j/half_word_size], seqs[i][j/half_word_size+1], kmer_ctr);
			for(k = 0; k < num_words_in_kmer; k++) // full words (get 16 nucleotides each time)
			{
				current_kmer[k] = ((seqs[i_seq][j_position/half_word_size+k] >> (2*(j_position%half_word_size)))&ones_mask) + 
					(((seqs[i_seq][j_position/half_word_size+1+k]&((1UL << (2*(j_position%half_word_size)))-1)) << 
					(2*(half_word_size-(j_position%half_word_size))))&ones_mask); // merge two half words from both adjacent words 

/*				print_packed_dna(&(seqs[i_seq][j_position/half_word_size+k]), 32, 1); 
				print_packed_dna(&(seqs[i_seq][j_position/half_word_size+1+k]), 32, 1); 				
				printf(" seq[j]=%ld  seq[j+1]=%ld word_kmer=%ld,\n ", seqs[i_seq][j_position/half_word_size+k], seqs[i_seq][j_position/half_word_size+1+k], current_kmer[k]); 
								fflush(stdout);
*/
				if((k == num_words_in_kmer-1) && (last_word_in_kmer < half_word_size)) 
					current_kmer[k] = current_kmer[k]&((1UL << (2*last_word_in_kmer))-1); // take only L nucleotides from last word 
			}			

			if(debug_prints>1)
			{
				printf("Extracted Kmer: (L=%ld)\n", L); 
				print_packed_dna(current_kmer, L, 1); // print extracted kmer 
				fflush(stdout); 
			}

			if( (!unique_flag) || (!hash_flag) ) // here just concatenate 
			{							
				if(debug_prints>1)
				{
					printf("Problem! Not Unique!\n");
					fflush(stdout); 

					printf("seq kmer[%ld]=%ld\n", kmer_ctr, i_seq);
					fflush(stdout);
					printf("index_ctr=%ld\n", index_ctr); 
					fflush(stdout);
					printf("num_bytes_to_copy=%ld\n", num_bytes_in_kmer_ceil); 
					fflush(stdout);
				}
				kmer_kmer_inds[index_ctr] =  kmer_ctr; //  index where kmer is in the list of all kmers 
				kmer_seq_inds[index_ctr++] = i_seq; // index of species for this kmer
				memcpy(kmers[kmer_ctr++], current_kmer, num_bytes_in_kmer_ceil); // copy kmer into output list 
				/**
				if(problematic_kmer)					// print kmer again
				{
				printf("Put prob. kmer in list place=%ld: \n", kmer_ctr); 						
				print_packed_dna(kmers[kmer_ctr-1], 32, 1);

				}
				**/

				//				kmer_seq_inds[kmer_ctr++] = i_seq; // record which sequence did the kmer come from 	(there's only one index here)		kmer_ctr++; 
			}
			else // keep a unique list by using Hash 
			{
				if(add_int)
				{
					rand_int2 = rand(); 
					HASH_FIND_INT(hTable, &rand_int2, found_hash_ptr);
				}
				else
					HASH_FIND(hh, hTable, current_kmer, num_bytes_in_kmer, found_hash_ptr);  
				if(found_hash_ptr == NULL) // found a new kmer 
				{
					if(i%50 == 0)
					{
						if(debug_prints)
						{
							printf("Not found in Hash New ID=%ld, seq=%ld pos=%ld, kmer=%ld\n", kmer_ctr, i, j, current_kmer[0]); 
							fflush(stdout); 
						}
					}

					temp_ptr = (struct SeqData *) calloc(1,sizeof(struct SeqData));	
					memcpy(kmers[kmer_ctr], current_kmer, num_bytes_in_kmer); // copy kmer into output list 
					memcpy(temp_ptr->seq, current_kmer, num_bytes_in_kmer); // copy kmer into hash table 
					temp_ptr->seqpos = kmer_ctr; // copy index (what is this?)

					if(add_int)
					{
						temp_ptr->rand_int = rand(); 
						HASH_ADD_INT(hTable, rand_int, temp_ptr); // add value to Hash table 
					}
					else
					{
						HASH_ADD(hh, hTable, seq, num_bytes_in_kmer, temp_ptr); // add value to Hash table 
					}
					kmer_kmer_inds[index_ctr] = kmer_ctr++; // save index of this kmer in the kmers long list
				}
				else // kmer already appears in hash table. Just update indices 
				{	
					if(i%50 == 0)
					{
						if(debug_prints)
						{
							printf("Found in Hash Before, seq=%ld pos=%ld, kmer=%ld\n", i, j, current_kmer[0]); 
							fflush(stdout); 
							printf("Found in Hash OLD ID=%ld\n", found_hash_ptr->seqpos); 
							fflush(stdout); 
						}
					}
					kmer_kmer_inds[index_ctr] = found_hash_ptr->seqpos; // index where kmer was found in hash.  save index of this kmer in the kmers long list 
					//						if(debug_prints)
					//						{
					//							printf("Found Hash After\n");
					//							fflush(stdout); 
					//						}
				}
				kmer_seq_inds[index_ctr++] = i_seq; // save index of species for this kmer 
			} // end if (non unique or non hash) 

			//			printf("Ind: %ld\n", kmer_inds[kmer_ctr-1]); 
			//			kmers[kmer_ctr][num_words_in_kmer-1] = // last word may be part 
			//				seqs[i][j/HALF_WORD_SIZE+1+k]
		} // end loop on kmers 
	} // end loop on sequences 
	/**/
	if(debug_prints)
		printf("After main loop. Time=%lf Sec.\n", (clock()-start_time) / CLOCKS_PER_SEC); fflush(stdout);


	if( (!hash_flag) && unique_flag ) // perform sort and unique , not using hash - is this the time-consuming part?? 
	{

	//		printf("No hashing used \n"); 
		//		word *tmp_kmers;
		//		tmp_kmers = new word[kmer_ctr]; 
		long *tmp_kmer_perm_indices;
		tmp_kmer_perm_indices = new long[kmer_ctr]; 
		long *kmer_keep_inds; kmer_keep_inds = new long[kmer_ctr]; 
		long run_slow_loop_temp=1;

		if(debug_prints)
		{
			printf("Before QSort, Num. kmers=%ld: Num words in kmer %ld\n", kmer_ctr, num_words_in_kmer);
			if(debug_prints>1)
				for(i=0; i<10/*kmer_ctr*/; i++)
				{
					printf("\n i=%ld kmer= ", i); 		
					for(j=0; j<num_words_in_kmer; j++)
						printf(" %lu, ", kmers[i][j]); 		
				}
		}
		DoQuicksort(kmers, kmer_ctr, num_words_in_kmer, num_bytes_in_kmer, tmp_kmer_perm_indices); // sort the ORIGINAL KMERS 		
		if(debug_prints)
			printf("After QSort. Time=%lf Sec. Total Splits %ld. Total Qsorts %ld Total Swaps %ld Total No-Swaps %ld\n", 
			(clock()-start_time) / CLOCKS_PER_SEC, TOTAL_NUM_SPLITS, TOTAL_NUM_QSORTS, TOTAL_NUM_SWAPS, TOTAL_NUM_NO_SWAPS); fflush(stdout);

		if(debug_prints)
		{
			printf("\n\nAfter QSort, Before DoOrder:\n");
			printf("LEN WORD USED=%ld\n", num_words_in_kmer);
		}

		if(run_slow_loop_temp)
		{
		DoOrder(kmer_seq_inds, kmer_ctr, tmp_kmer_perm_indices); // what does ordering do? 
		if(debug_prints)
			printf("After DoOrder. Time=%lf Sec.\n", (clock()-start_time) / CLOCKS_PER_SEC); fflush(stdout);

		delete tmp_kmer_perm_indices;
		index_ctr = kmer_ctr; 
		if(debug_prints)
			printf("\n\nAfter DoOrder. Index_ctr is %ld:\n", index_ctr);
		kmer_ctr = -1; // first time this is incremented to zero  
		for(i=0; i<index_ctr; i++) // run over all kmers and do unique 
		{
			//			printf("i=%ld kmer=%ld. ", i, tmp_kmers[i]); 
			if((i>0) && (!memcmp(kmers[i], kmers[i-1], num_bytes_in_kmer)))	 // same as previous 
			{
				kmer_kmer_inds[i] = kmer_ctr; // index of this kmer in list of all kmers 
				//				kmer_seq_inds[i] = tmp_kmer_inds[1][i]; // index of species for this kmer 
				if(debug_prints)
				{
					//					printf("Same kmers again!!!!\n"); 
					//					fflush(stdout); 
				}

			}
			else // found a new value 
			{
				kmer_ctr++;
				kmer_keep_inds[kmer_ctr] = i; 
				//				memcpy(kmers[kmer_ctr], kmers[i], num_bytes_in_kmer); 
				//				kmers[kmer_ctr][0] = tmp_kmers[i]; // copy new value 
				kmer_kmer_inds[i] = kmer_ctr; // index of this kmer in list of all kmers 
				//				kmer_inds[i][1] = tmp_kmer_inds[1][i]; // index of species for this kmer 
				if(debug_prints)
				{
					//					printf("Incrementing kmers!!!!\n"); 
					//					fflush(stdout); 
				}
			}
		} // finish loop on all kmers with unique
		if(debug_prints)
			printf("After unique loop. Time=%lf Sec.\n", (clock()-start_time) / CLOCKS_PER_SEC); fflush(stdout);

		kmer_ctr++; // true number of found kmers 
		//		printf("NUM UNIQUE KMERS IN EXTRACT FUNCTION: %ld\n", kmer_ctr); 
		for(i=0; i<kmer_ctr; i++) // copy again into same array
		{
			if(kmer_keep_inds[i] > i)
				memcpy(kmers[i], kmers[kmer_keep_inds[i]], num_bytes_in_kmer);
		}
		} // end of heavy loop (temp)
		delete kmer_keep_inds;
	} // end of if unique and not hash part  
	else
	{
		if(debug_prints)
		{
			if(hash_flag)
				printf("Used hash \n"); 
			else
				printf("No unique needed \n"); 
		}
	}
	/**/	


	/*
	for(i=0; i<kmer_ctr; i++)
	printf("kmer[%ld]=%lu\n", i, kmers[i][0]); 
	printf("Found %ld unique kmers\n", kmer_ctr);  
	*/

	num_unique_kmers[0] = kmer_ctr; 
	num_sparse_matrix_elements[0] = index_ctr;

	delete_all(); // free hash memory 
	if(hash_flag)
		HASH_CLEAR(hh,hTable); // free memory 

	return 0; 
} // end of function extract_sub_kmers

////////////// Temp /////////////////////
// len is in nucleotides 
// print_mode: 1 - letters, 0 - just numbers 
void print_packed_dna(word *seqs, long len, long print_mode)
{
	long i, j;
	word tmp;
	long half_word_size = 4*sizeof(word);
	word ones_mask=0xF;
	if(half_word_size == 32)
		ones_mask = 0x1F; 


	for(i = 0; i < len; i++)
	{
		//		printf("\nSeq[i/16]=%ld\n", seqs[i/16]);
		tmp = seqs[i/half_word_size];

		tmp = tmp >> (2*(i&ones_mask));
		tmp = tmp&0x3;
		tmp++;
		if(print_mode == 0)
			printf("%lx ", tmp);
		else
		{
			//			printf("%lx ", tmp);
			switch (tmp)
			{
			case 1:
				printf("A"); break;
			case 2:
				printf("C"); break;
			case 3: 
				printf("G"); break;
			case 4:
				printf("T"); break;
			}
		}
	}
	printf("\n");

}
