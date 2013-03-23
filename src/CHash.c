/* correct for read errors and put in the matrix result vector
 * input
 * % dbseqs - a list of input sequence strings (1000000 x 50) of the matrix (char)
 * % invec - the input (noisy) reads (? x 50) (char)
 * % output
 * % resvec - the read error corrected (1000000) count vector
 * %
 * % note : input must be UPPER CASE!!!
 */

#include "stdio.h"
#include "mex.h"
#include "matrix.h"
#include "uthash.h"

#define SEQLENGTH   50
#define MAXLISTSIZE 1000000

struct SeqData {
    int seqpos;
    char seq[SEQLENGTH+1]; /* need to set the right length */
    UT_hash_handle hh; /* makes this structure hashable */
};

struct SeqData *hTable=NULL;

/* The gateway routine */
void mexFunction(int nlhs, mxArray *plhs[],
        int nrhs, const mxArray *prhs[]) {
    
    mxArray *num_minutes, *str;
    unsigned char *dbseqs, *invec;
    int numOfSeqs;
    int seqLen;
    int numReads;
    double *outDat;
    unsigned char letters[4]="ACGT";
    struct SeqData *cseqdat;
    struct SeqData *tseqdat;
    int a, b;
    unsigned char creadseq[SEQLENGTH+1];
    unsigned char oldnuc;
    int cpos;
    int cnuc;
    int found0, found1;
    int *ppos;
    double *pfreq;
    int clpos;
    unsigned char tmpbuf[255];
    unsigned char tmpstr[255];
    unsigned char tmpseq[SEQLENGTH+1];
    double totfreq;
    struct seqData *current_user, *tmp;
    
    dbseqs = (unsigned char *)  mxGetPr(prhs[0]);
    numOfSeqs = mxGetM(prhs[0]);
    seqLen = mxGetN(prhs[0]);
    if (seqLen!=SEQLENGTH) {
        printf("Different seq lens from #define!!!\n");
        return;
    }
    invec = (unsigned char *)  mxGetPr(prhs[1]);
    numReads = mxGetM(prhs[1]);
    if (mxGetN(prhs[1])!=seqLen) {
        printf("Different seq lens!!!\n");
        return;
    }
    
    plhs[0] = mxCreateDoubleMatrix(numOfSeqs, 1, mxREAL);
    outDat=mxGetPr(plhs[0]);
    
    if ((tseqdat=(struct SeqData *) calloc(numOfSeqs,sizeof(struct SeqData)))==NULL) {
        printf("Can't allocate seqdata");
        return;
    }
    
    for (a=0;a<numOfSeqs;a++) {
        for (b=0;b<seqLen;b++) {
            tmpseq[b]=dbseqs[a+b*numOfSeqs];
        }
        tmpseq[b]=0;
/*        if ((cseqdat=(struct SeqData *)malloc(sizeof(struct SeqData)))==NULL) {
            printf("Out of mem\n");
            return;
        }*/
        cseqdat=&(tseqdat[a]);
        cseqdat->seqpos=a;
        for (b=0;b<seqLen;b++) {
            cseqdat->seq[b]=dbseqs[a+b*numOfSeqs];
        }
        cseqdat->seq[b]=0;
        HASH_ADD_STR( hTable, seq, cseqdat );
    }
    
    if ((ppos=(int *) calloc(numOfSeqs, sizeof(int)))==0) {
        printf("Can't allocate ppos\n");
        return;
    }
    if ((pfreq=(double *) calloc(numOfSeqs, sizeof(double)))==0) {
        printf("Can't allocate pfreq\n");
        return;
    }
    
    
    for (a=0;a<numReads;a++) {
        for (b=0;b<seqLen;b++) {
            creadseq[b]=invec[a+b*numReads];
        }
        creadseq[b]=0;
        found0=0;
        found1=0;
        clpos=0;
        HASH_FIND_STR(hTable, creadseq, cseqdat);
        if (cseqdat) {
            found0++;
            pfreq[clpos]=1;
            ppos[clpos]=cseqdat->seqpos;
            clpos++;
        }
        for (cpos=0;cpos<seqLen;cpos++) {
            oldnuc=creadseq[cpos];
            for (cnuc=0;cnuc<4;cnuc++) {
                if (letters[cnuc]!=oldnuc) {
                    creadseq[cpos]=letters[cnuc];
                    HASH_FIND_STR( hTable, creadseq, cseqdat);
                    if (cseqdat) {
                        found1++;
                        pfreq[clpos]=0.01;
                        ppos[clpos]=cseqdat->seqpos;
                        clpos++;
                    }
                }
            }
            creadseq[cpos]=oldnuc;
        }
        if (clpos>0) {
            totfreq=0;
            for (b=0;b<clpos;b++) {
                totfreq+=pfreq[b];
            }
            if (totfreq>0) {
                for (b=0;b<clpos;b++) {
                    outDat[ppos[b]]+=pfreq[b]/totfreq;
                }
            } else {
                printf("Error-freqsum=0\n");
            }
        }
    }
    free(ppos);
    free(pfreq);
    
    HASH_CLEAR(hh,hTable);
    free(tseqdat);
}
