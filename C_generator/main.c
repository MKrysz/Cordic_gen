#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#define BIT_CNT_INT 62
#define BIT_CNT_EXT 28

#define BIT_SHIFT (BIT_CNT_INT-BIT_CNT_EXT)
#define Deg2Rad(X) ((X) * 0.0174533)
#define Rad2Deg(X) ((X) / 0.0174533)
#define FMT_FIX "%llx"

#if (BIT_CNT_INT+2) > 64
#error TOO MUCH BITS USED
#endif
#if (BIT_CNT_INT < BIT_CNT_EXT)
#error INTERNAL WIDTH MUST BE GREATER OR EQUAL TO EXTERNAL WIDTH
#endif

int64_t doubleToFixed(double arg, char* str, int bit_cnt);

int64_t doubleToFixed(double arg, char* str, int bit_cnt){
    double temp = 1;
    int64_t result = 0;
    if(arg < 0){
        result = 1;
        result = result << 1;
    }
    for(int i = 0; i < bit_cnt; i++){
        if(arg>=temp){
            arg -= temp;
            result |= 1;
        }
        result = result << 1;
        temp = temp/2;
    }
    if(str != NULL)
        sprintf(str, FMT_FIX, result);
    return result;
}

double FixedToDouble(int64_t arg, int bit_cnt){
    int isNeg = 0;
//    int64_t mask = 1 << bit_cnt+1;
    int64_t mask = 1 << (bit_cnt+1);
    double result = 0;
    double temp = 1;
    if(arg&mask){
        result -= 2;
    }
    mask = mask >> 1;
    for(int i = 0; i < bit_cnt+1; i++){
        if(arg & mask){
            result += temp;
        }
        temp = temp / 2;
        mask = mask >> 1;
    }
    return result;
}

int64_t ATan_tab[BIT_CNT_INT];
double multiplier;


int64_t dropBits(int64_t input){
//    if(input & 1<<(BIT_SHIFT-1)){
//        input += 1;
//    }
    return input>>BIT_SHIFT;
}

void fixedCordic(int64_t *sin, int64_t *cos, int64_t angle, bool log){
    int64_t x = doubleToFixed(multiplier, NULL, BIT_CNT_INT);
    int64_t y = 0;
    int64_t z = 0;
    angle = angle<<BIT_SHIFT;
    if (log)
        printf("x\t\t\ty\t\t\tz\t\t\tangle\n");
    for (size_t i = 0; i < BIT_CNT_INT; i++)
    {
        int64_t x_new,y_new;
        if(z <= angle){
            z += ATan_tab[i];
            x_new = x - (y>>i);
            y_new = y + (x>>i);
        }
        else {
            z -= ATan_tab[i];
            x_new = x + (y>>i);
            y_new = y - (x>>i);
        }
        x = x_new;
        y = y_new;
        if (log)
              printf("%llx\t%llx\t%llx\t%llx\n", x, y, z, angle);
    }
    *sin = dropBits(y);
    *cos = dropBits(x);
}

void generateConsts(){
    double tangens = 1;
    multiplier = 1;
//    printf("int64_t ATan_tab[%i] = {\n", BIT_CNT_INT);
    for (int i = 0; i < BIT_CNT_INT; ++i) {
        double a_tan = atan(tangens);
        ATan_tab[i] = doubleToFixed(a_tan, NULL, BIT_CNT_INT);
        multiplier *= cos(a_tan);
        tangens = tangens / 2;
    }
}


void testCordicAlgo(){


    double ULP = 1;
    for (int i = 0; i < BIT_CNT_EXT; ++i) {
        ULP = ULP/2;
    }
    double maxErr = 0;
    double angle = 0;
    double prev_progress = 0;
    printf("start\n");
    printf("%0.1f%%\r", prev_progress);
    fflush(stdout);
    int64_t isin,icos;
    double fsin, fcos, gccsin, gcccos, sinErr, cosErr, sinErrULP, cosErrULP, jedynka;
//    printf("deg\t\t\trad\t\t\tsin\t\tfsin\t\tgccsin\t\tsinErr\t\t[ULP]\t\tcos\t\tfcos\t\tgcccos\t\tcosErr\t\t[ULP]\t\t\tjedynka\n");
    int64_t finish = doubleToFixed(Deg2Rad(90), NULL, BIT_CNT_EXT);
//    int64_t finish = 0x4;
    for (int64_t fixed = 0; fixed < finish; fixed += 1000) {
//        if(fixed != 0x4e30) continue;
        double rad = FixedToDouble(fixed, BIT_CNT_EXT);
        double deg = Rad2Deg(rad);
        double progress = ((double)fixed)/((double)finish)*100;
        if (progress >= prev_progress + 0.1){
            prev_progress = progress;
            printf("%.1f%%\r", prev_progress);
            fflush(stdout);
        }
        fixedCordic(&isin, &icos, fixed, false);
        fsin = FixedToDouble(isin, BIT_CNT_EXT);
        fcos = FixedToDouble(icos, BIT_CNT_EXT);
        gccsin = sin(rad);
        gcccos = cos(rad);
        sinErr = fabs(fsin-gccsin);
        cosErr = fabs(fcos-gcccos);
        sinErrULP = sinErr/ULP;
        cosErrULP = cosErr/ULP;
        if(sinErr>maxErr){
            maxErr = sinErr;
            angle = deg;
        }
        if(cosErr>maxErr){
            maxErr = cosErr;
            angle = deg;
        }
        jedynka = fsin * fsin + fcos * fcos;
        const double errThr = 0;
//        if(sinErrULP > errThr || cosErrULP > errThr)
//            printf("%f\t%f\t"FMT_FIX"\t%f\t%f\t%e\t%f\t"FMT_FIX"\t%f\t%f\t%e\t%f\t\t%f\n",
//                   deg, rad,
//                   isin, fsin, gccsin, sinErr, sinErrULP,
//                   icos, fcos, gcccos, cosErr, cosErrULP,
//                   jedynka);
    }
    printf("deg\t\t\trad\t\t\tsin\t\t\tfsin\t\tgccsin\t\tsinErr\t\t\t[ULP]\t\tcos\t\t\tfcos\t\tgcccos\t\tcosErr\t\t\t[ULP]\t\t\tjedynka\n");

    printf("\nMax Err = %e,\t%f [ULP]\nfor angle = %f\n", maxErr, maxErr/ULP, angle);
    if(maxErr <= ULP)
        printf("Max Err  <= 1[ULP]\n");
    else
        printf("!!! Max Err > 1[ULP] !!!\n");
}

void cordicTestOne(){
//    int64_t fixed = doubleToFixed(Deg2Rad(deg), NULL, BIT_CNT_EXT);
    int64_t fixed = 3;
//    printf("input %f rad = %i'h"FMT_FIX"\n", deg, BIT_CNT_EXT+2, fixed);
    int64_t isin, icos;
    fixedCordic(&isin, &icos, fixed, true);
    printf("sin\t\tcos\n");
    printf(FMT_FIX"\t"FMT_FIX"\n", isin, icos);
}

void generateCordicHDL(){
    FILE *hfile;
    hfile = fopen("../cordic_params.v", "w");

    // generate localparams
    fprintf(hfile, "localparam WD_EXT = %d;\n", BIT_CNT_EXT);
    fprintf(hfile, "localparam WD_INT = %d;\n", BIT_CNT_INT);
    fprintf(hfile, "localparam START_VAL = %i'h%llx;\n",
            BIT_CNT_INT+2, doubleToFixed(multiplier, NULL, BIT_CNT_INT));
    fprintf(hfile, "localparam max_i = %i'd%d;\n",
            8, BIT_CNT_INT);

    fclose(hfile);

    // generate a_tan table
    hfile = fopen("../cordic_atan_table.v", "w");
    for (int i = 0; i < BIT_CNT_INT; ++i) {
        fprintf(hfile, "assign atan[%i] = \t%i'h%llx;\n", i, BIT_CNT_INT+1, ATan_tab[i]);
    }

    fclose(hfile);


    printf("deg 90 = %i'h%llx\n",
           BIT_CNT_EXT+2, doubleToFixed(Deg2Rad(90), NULL, BIT_CNT_EXT));
}

int main(){
    generateConsts();

    testCordicAlgo();
    generateCordicHDL();
}
