// Contains functions for parsing and pruning snp files 



// Traverse the SNPs list from top to bottom and exclude all SNPs with LD with each SNP
function prune_snps_by_LD(char snp_ids[], double snp_pvals[], ) 






// 
// Read a tab-delimited .txt file 
long loadcellfile(FILE *data_file_ptr, char *data_file_name, char str_array[][MAX_FIELDS]);
long loadcellfile(FILE *data_file_ptr, char *data_file_name, char str_array[][MAX_FIELDS])
{
	long i, j, k;
	char c;


	// open the file to enable reading from it
	data_file_ptr = fopen(data_file_name, "r");
	printf("Start Reading Data File ...\n");


	// Read first line do determine how many fields per line 	

	c = 'k';

	printf("Start Reading The labels ... \n");

	while( int(c) != EOLN)
	{
		fscanf(promoter_f, "%c", &c);
		str_array[i][j] = c;
	}
	printf("Finished Reading The labels  ...\n");


	return 0; 
}