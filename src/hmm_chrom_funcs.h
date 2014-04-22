#ifndef _HMM_CHROM_FUNCS_
#define _HMM_CHROM_FUNCS_

extern long TOTAL_NUM_SPLITS;
extern long TOTAL_NUM_QSORTS;
extern long TOTAL_NUM_SWAPS; 
extern long TOTAL_NUM_NO_SWAPS;



// This structure contains all the needed information on the HMM model. Should move to hmm_chr
// Currently Supported : X - discrete (finite states)
//						 Y - discrete / Mixture of Gaussians
typedef struct {

long x_dim; // dimension of the hidden variable x
long y_dim; // dimension of the observed variable y (in discrete case) or number of mixtures (in continunuos case)
long x_dim2; // dimension of another hidden variable (when we have two of them !!!) 
long y_type;   // 0 - Discrete 1 - Continunuos Mixture of Gaussian
long gauss_dim; // the dimension of a gaussian variable (we currently allow 1 or 2 dimensional gaussians)
long special_models_flag; // With this flag we use special models (such as for the SNP data), with complicated constrains on the variables and parameters

double M[MAX_X_VALS][MAX_X_VALS];  // The transition matrix. M[i][j] = Prob (X_n = j | X_{n-1} = i)
double PI[MAX_X_VALS];  // The initial distribution for the i-th

double N[MAX_X_VALS][MAX_Y_VALS];  // The ommision matrix  N[i][j] = Prob (X_n = j | X_{n-1} = i)
double MU[MAX_X_VALS][MAX_Y_VALS];  // The means of the gaussians
double SIGMA[MAX_X_VALS][MAX_Y_VALS];   // the variances of the gaussians


// Cumulative sums for better performance 
double M_cum[MAX_X_VALS][MAX_X_VALS];  // The transition matrix. M[i][j] = Prob (X_n = j | X_{n-1} = i)
double PI_cum[MAX_X_VALS];  // The initial distribution for the i-th
double N_cum[MAX_X_VALS][MAX_Y_VALS];  // The ommision matrix  N[i][j] = Prob (X_n = j | X_{n-1} = i)


// logs for better performance
double M_log[MAX_X_VALS][MAX_X_VALS];  // The transition matrix. M[i][j] = Prob (X_n = j | X_{n-1} = i)
double PI_log[MAX_X_VALS];  // The initial distribution for the i-th
double N_log[MAX_X_VALS][MAX_Y_VALS];  // The ommision matrix  N[i][j] = Prob (X_n = j | X_{n-1} = i)

double SIGMA_log[MAX_X_VALS][MAX_Y_VALS];  // the standard deviations logs 
double SIGMA_inv[MAX_X_VALS][MAX_Y_VALS];  // the standard deviations inverses


long cum_flag;  // flag saying if to compute cums
long log_flag; // flag saying if to compute logs


// Now the updating parameters flags. Used if some of the parameters are known
long update_M_flag;   // transition matrix updating
long update_N_flag;   // emission matrix updating
long update_MU_flag;   // mean matrix updating 
long update_SIGMA_flag;   // variance matrix updating 


long fold_change_flag;   // New : a flag saying if the distributions represet
						 // fold change. If so, the MU parameters are set such that 
						 // the lowest level is no more than one, and the highest level 
						 // is no less then one.  

long place_flag;  // If this flag is on, we use different gaussian for every point

long seq_len; // Note ! Exists in both structures !!! 
double M_POWERS[MAX_POWER_OF_TRANS_MATRIX][MAX_X_VALS][MAX_X_VALS];  // The transition matrix. M[i][j] = Prob (X_n = j | X_{n-1} = i)
double M_POWERS_cum[MAX_POWER_OF_TRANS_MATRIX][MAX_X_VALS][MAX_X_VALS];  // The transition matrix. M[i][j] = Prob (X_n = j | X_{n-1} = i) cumsum


double *place_gaussian_mu[MAX_2D_X_VALS][MAX_Y_VALS];  // the means of the gaussians if we have many of them
double *place_gaussian_sigma[MAX_2D_X_VALS][MAX_Y_VALS];  // the standard deviations of the gaussians if we have many of them
double *place_gaussian_sigma_inv[MAX_2D_X_VALS][MAX_Y_VALS];  // the standard deviations inverse matrix of the gaussians if we have many - for 2d case
double *place_M[MAX_X_VALS][MAX_X_VALS];  // the transition probabilities at each location
double *place_N[MAX_X_VALS][MAX_Y_VALS];  // the emission probabilities at each location
double *place_M_cum[MAX_X_VALS][MAX_X_VALS];  // the transition probabilities at each location cumsum
double *place_N_cum[MAX_X_VALS][MAX_Y_VALS];  // the emission probabilities at each location cumsum

long miss_data; // Note ! Exists in both structures !!! 


double M_upperbounds[MAX_X_VALS][MAX_X_VALS];  // Bounds on the transition matrix. M[i][j] = Prob (X_n = j | X_{n-1} = i)

long use_bounds;  // Flag saying if to use the bounds on the transition matrix

}  hmm_model;





// This structure contains all the data and related things
typedef struct { 

long miss_data;    // a flag saying if there is a missing data in the Y's - also in hmm_model !!!  

double dont_miss_prob;   // prob. of NOT missing a Y letter (currently simulation assumes i.i.d. places)

// data length 
long seq_len; 

long y_type; // also in hmm_model !!! 

// here is the data 
long *x_vec;
double *y_vec;  
double *y_vecB;  

long *y_vec_int; 
long *mix_vec; 
long *loc_vec;
long *loc_diff_vec;



} hmm_data;



// The gene struct. This contains all the neccessary information needed
// on the specific gene.
typedef struct {


char *seq;        // The sequence of nucleotide. Is this the gene or the promoter ???
long seq_len;     // The length of the sequence 
char *name;       // The name of the gene 
long locus;       // The locus link number 
long NT;          // the accession number NT

int   *seq_exp;   

double *exp_vec;  // Expression Vector for this gene

long exp_len;     // length of expression vecroe ????

char *symbol;   
char *afy_name;
char *accession;


} gene;



/**
// The expression vector struct (temporary ??). This contains all the neccessary information needed
// on the specific gene.
typedef struct {


char *seq;        // The sequence of nucleotide
long seq_len;     // The length of the sequence 
char *name;       // The name of the gene 
long locus;       // The locus link number 
long NT;          // the accession number NT

int   *seq_exp;



} gene;
***/







long split(double *vals, long first, long last, long *indexes);
// long split(word *vals[MAX_L], long first, long last, long len_each_word_in_bytes, long *indexes);
long split(word *vals[MAX_L], long first, long last, long len_each_val_in_words, long len_each_val_in_bytes, long *indexes);

double my_quantile(double *vals, long len, double quantile); // get array quantile 
long my_quicksort(double *vals, long first, long last, long *indexes);
//long quicksort(word *vals[MAX_L], long first, long last, long len_each_word, long *indexes);
long my_quicksort(word *vals[MAX_L], long first, long last, long len_each_val_in_words, long len_each_val_in_bytes, long *indexes);
long DoQuicksort(double *vals, long len, long *indexes);
long DoQuicksort(word *vals, long len, long *indexes); // overloaded
// long DoQuicksort(word *vals[MAX_L], long len, long len_each_word, long *indexes);
long DoQuicksort(word *vals[MAX_L], long len, long len_each_val_in_words, long len_each_val_in_bytes, long *indexes);

long DoOrder(double *vals, long len, long *indexes);
long DoOrder(word *vals, long len, long *indexes);
long DoOrder(long *vals, long len, long *indexes);

long MatrixMultiply(double A[MAX_X_VALS][MAX_X_VALS], long m, long n, double B[MAX_X_VALS][MAX_X_VALS], long r, 
					double C[MAX_X_VALS][MAX_X_VALS]);
long MatrixStationaryVec(double A[MAX_X_VALS][MAX_X_VALS], long n, double pi[MAX_X_VALS]);
long MatrixTranspose(double A[MAX_X_VALS][MAX_X_VALS], long m, long n, 
					 double A_t[MAX_X_VALS][MAX_X_VALS]);
long GSolve(double a[MAX_X_VALS][MAX_X_VALS],long n,double x[MAX_X_VALS]);
					

double MixtureOfGaussiansGivenInit(double *x, long num_points, long num_of_Gaussians, long num_of_iterations, 
		 double *init_p, double *init_m, double *init_s, 
		 double *p, double *m, double *s, double *log_like);
float MixtureOfGaussiansGivenInitSingle(float *x, long num_points, long num_of_Gaussians, long num_of_iterations, 
		 float *init_p, float *init_m, float *init_s, 
		 float *p, float *m, float *s, float *log_like);



double randr();
long PrintVec(long *vec, long len);
long PrintDoubleVec(double *vec, long len);
long Geometric(double p);
double Gaussian(double mu,double sigma);
double ComputeJointHMMLogLikelihood(hmm_model *hmm, hmm_data *data);
long Viterbi( hmm_model *hmm, hmm_data *data, double *y_cond_tabs[MAX_X_VALS],  double *y_cond_tabsB[MAX_X_VALS],
			 long *opt_x_vec_out, double *x_loglike, double *x_place_loglike, double *y_loglike);
long InitilizeData( hmm_data *data, long seq_len, long y_type, long miss_data, double dont_miss_prob, long copy_data, 
				   double *y_vec, long *y_vec_int, long *loc_vec);
long InitilizeModel(hmm_model *hmm, long x_dim, long y_dim, long y_type, long use_bounds, long place_flag, 
					long special_models_flag, long miss_data, long fold_change_flag);
long ComputeModelAuxillaryParameters(hmm_model *hmm);
long CompareTwoModels(hmm_model *hmm1, hmm_model *hmm2);
long SimulateSequenceFromModel(hmm_model *hmm, hmm_data *data);
long PrintModel(hmm_model *hmm);
long forward( hmm_model *hmm, hmm_data *data, double *y_cond_tabs[MAX_X_VALS], 
			  double *alpha[MAX_X_VALS], double *scale, double *scale_cum,  double *scale_exp, double *y_vec_log_prob_out);
long backward( hmm_model *hmm, hmm_data *data, double *scale_exp, double *y_cond_tabs[MAX_X_VALS],
			  double *beta[MAX_X_VALS]); 
long ComputeMarginalGamma(hmm_model *hmm, hmm_data *data, double *y_mu_square_exp_vecs[MAX_X_VALS][MAX_Y_VALS],
						 double *alpha[MAX_X_VALS], double *beta[MAX_X_VALS], double *scale_exp, double *y_cond_tabs[MAX_X_VALS],
						 double *gamma[MAX_X_VALS], double *phi[MAX_X_VALS][MAX_Y_VALS]);
long ComputePsi(hmm_model *hmm, hmm_data *data, double *alpha[MAX_X_VALS], double *beta[MAX_X_VALS], double *scale, double *y_cond_tabs[MAX_X_VALS],
				 double *psi[MAX_X_VALS][MAX_X_VALS]);
long Compute_y_cond_tabs(hmm_model *hmm,  hmm_data *data, 
					double *y_cond_tabs[MAX_X_VALS], 
					double *y_mu_square_exp_vecs[MAX_X_VALS][MAX_Y_VALS]);
long UpdateModelParams(hmm_model *hmm, hmm_data *data, double *y_mu_square_exp_vecs[MAX_X_VALS][MAX_Y_VALS],
						 double *gamma[MAX_X_VALS], double *phi[MAX_X_VALS][MAX_Y_VALS],
						 double *psi[MAX_X_VALS][MAX_X_VALS]);
long SmartStartParams(hmm_model *hmm, double *y_vec, long seq_len);
long TrainModelEM(hmm_model *hmm, hmm_data *data, long max_iters, long num_starts, double tolerance, 
				  double *best_model_score);
long CopyModel(hmm_model *hmm1, hmm_model *hmm2, hmm_data *data); 
long PermuteModelIncreasingMean(hmm_model *hmm, hmm_data *data);
long ComputeKLDistance(hmm_model *hmm1, hmm_model *hmm2, hmm_data *data, long iters, double *KL_dist);

// special functions for the 'complicated' SNPs case
//double ComputeJointHMMLogLikelihoodSNPs(hmm_model *hmm, hmm_data *data);
double ComputeJointHMMLogLikelihoodSNPs(hmm_model *hmm, hmm_data *data,
										double *x_loglike, double *place_x_loglike, double *y_loglike);
double TrainModelEMSNPs(hmm_model *hmm, hmm_data *data, long max_iters, long num_starts, double tolerance, 
				  double *best_model_score, double *tmp_vec);
long forwardSNPs( hmm_model *hmm, hmm_data *data, double *y_cond_tabs[MAX_2D_X_VALS], double *y_cond_tabsB[MAX_2D_X_VALS], 
			  double *alpha[36], double *scale, double *scale_cum,  double *scale_exp, double *y_vec_log_prob_out);
long backwardSNPs( hmm_model *hmm, hmm_data *data, double *scale_exp, 
				   double *y_cond_tabs[MAX_2D_X_VALS], double *y_cond_tabsB[MAX_2D_X_VALS],
				   double *beta[36]);
long ViterbiSNPs( hmm_model *hmm, hmm_data *data, double *y_cond_tabs[MAX_2D_X_VALS],  double *y_cond_tabsB[MAX_2D_X_VALS],
			  long *opt_x_vec_out); // , double *x_loglike, double *x_place_loglike, double *y_loglike);
long ComputeMarginalGammaSNPs(hmm_model *hmm,  hmm_data *data, 
						 double *y_mu_square_exp_vecs[MAX_2D_X_VALS][MAX_Y_VALS], double *y_mu_square_exp_vecsB[MAX_2D_X_VALS][MAX_Y_VALS],
						 double *alpha[36], double *beta[36], double *scale_exp, 
						 double *y_cond_tabs[MAX_2D_X_VALS], double *y_cond_tabsB[MAX_2D_X_VALS],
						 double *gamma[36], double *phi[36][MAX_Y_VALS]);
double UpdateModelParamsSNPs(hmm_model *hmm, hmm_data *data, 
						   double *y_mu_square_exp_vecs[MAX_2D_X_VALS][MAX_Y_VALS],
						   double *y_mu_square_exp_vecsB[MAX_2D_X_VALS][MAX_Y_VALS],
						   double *gamma[36], double *phi[36][MAX_Y_VALS],
						   double psi_sum[36][36]);
long ComputePsiSNPs(hmm_model *hmm, hmm_data *data, double *alpha[36], double *beta[36], 
				double *scale, double *y_cond_tabs[MAX_2D_X_VALS], double *y_cond_tabsB[MAX_2D_X_VALS],
				double psi_sum[36][36]);

long Compute_y_cond_tabsSNPs(hmm_model *hmm,  hmm_data *data, 
					double *y_cond_tabs[MAX_2D_X_VALS], double *y_cond_tabsB[MAX_2D_X_VALS], 
					double *y_mu_square_exp_vecs[MAX_2D_X_VALS][MAX_Y_VALS], 
					double *y_mu_square_exp_vecsB[MAX_2D_X_VALS][MAX_Y_VALS]);


long ComputeKLDistanceSNPs(hmm_model *hmm1, hmm_model *hmm2, hmm_data *data, long iters, double *KL_dist);


long NEWCompute_y_cond_tabsSNPs(hmm_model *hmm,  hmm_data *data, 
					double *y_cond_tabs[36], double *y_cond_tabsB[36], 
					double *y_mu_square_exp_vecs[MAX_2D_X_VALS][MAX_Y_VALS], 
					double *y_mu_square_exp_vecsB[MAX_2D_X_VALS][MAX_Y_VALS]);

long SLOWforwardSNPs( hmm_model *hmm, hmm_data *data, double *y_cond_tabs[MAX_2D_X_VALS], double *y_cond_tabsB[MAX_2D_X_VALS], 
			  double *alpha[36], double *scale, double *scale_cum,  double *scale_exp, double *y_vec_log_prob_out);

#endif