#include <cstdio>
#include <cstring>
#include <cstdlib>
#include "byutr.h"
#include "lookup.hpp"

int main(int argc, char **argv) {
    /**
     * n - number specified by the -n argument
     * count - number of levels specified in the command-line
     * total - the total number of bits used for the page-table
     */
    int n, count, total;
    count = total = DEF_INT;
    /**
     * setn - true if -n was used
     * setp - true if -p was used
     * t - true if -t was used
     * setLevels - true after we read the input file argument
     */
    bool setn, setp, t, setLevels;
    setn = setp = t = setLevels = DEF_BOOL;
    /**
     * p - the output file name
     * fileName - the input file name
     */
    char *p, *fileName;
    int *bitfields = new int[MAX_BF]; // stores the number of bits for each level
    Pagetable *pagetable;

    // evaluates each argument
    for (int i = 1; i < argc; i++) { // start at 1 ("./pagetable" is #0)
        if (!strcmp(argv[i], "-n")) {
            n = atoi(argv[++i]); // string to int
            setn = true;
        } else if (!strcmp(argv[i], "-p")) {
            p = argv[++i];
            setp = true;
        } else if (!strcmp(argv[i], "-t")) {
            t = true;
        } else {
            if (setLevels) { // take in the remaining arguements
                bitfields[count] = atoi(argv[i]); // bitfields[0] is level 1
                total += bitfields[count++];
            } else { // take in the input file arguement
                fileName = argv[i];
                setLevels = true;
            }
        }
    }
    pagetable = new Pagetable(bitfields, count); // init pagetable
    
    // int doit = DEF_INT; // for progress meter
    int frames = DEF_INT; // frame counter
    double hit, miss, processed; // used to calculate hit-miss ratios
    hit = miss = processed = DEF_DBL;
    FILE *traceFile = fopen(fileName, "rb");
    p2AddrTr trace;

    while (!feof(traceFile)) {
        if (setn && (n > 0)) { // there's some left
            n--;
        } else if (setn) {
            break;
        }

        if (t) { // print translations
            printf("%08x -> %08x\n", trace.addr, ((frames << (MAX_BF - total)) & ~mask(MAX_BF - total)) | (trace.addr & mask(MAX_BF - total)));
        }
        NextAddress(traceFile, &trace);
        if (!pagetable->pageLookup(trace.addr)->valid) {
            pagetable->pageInsert(trace.addr, frames++);
            miss++;
        } else {
            hit++;
        }

        // // for progress meter
        // if (++doit > 224) {
        //     printf("%.2f%% done\r", (hit + miss) / 224449.);
        //     doit = 0;
        // }
    }
    fclose(traceFile);

    if (setp) {
        FILE *output = fopen(p, "w");
        Map *temp;
        // lookup each valid page
        for (int i = 0; i < pow(2, total); i++) {
            temp = pagetable->pageLookup(i);
            if (temp->valid) { // and print it out
                fprintf(output, "%08x -> %08x\n", i, temp->frame);
            }
        }
        fclose(output);
    }

    printf("Page table size:  %ld\n", getTBSize());
    
    // calculate ratios
    processed = hit + miss;
    double hitP = hit / processed * 100.;
    double missP = miss / processed * 100.;
    printf("Hits %.0f (%.2f%%), Misses %.0f (%.2f%%) # Addresses %.0f\n",
        hit, hitP, miss, missP, processed);

    printf("Bytes used:  %ld\n", getUsed());

    return 0;
}
