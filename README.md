
# CORDIC Verilog generator

## About
CORDIC HDL implementation for calculating sine and cosine of given angle. 

Input and output is in twos complement signed fixed point number with 1 integer bit and variable number of fraction bits.

### Getting started

Following macros are used to change internal and external databus width. Internal is used for controlling accuracy of the algorithm at the cost of logic elements used.
```c
#define BIT_CNT_INT 62
#define BIT_CNT_EXT 28
```
To *testCordicAlgo()* function is used to pre-test algorithm for accuracy, while *generateCordicHDL()* function generates files then included by base *cordic.v* file.
```c
int main(){
    generateConsts();

    testCordicAlgo();
    generateCordicHDL();
}
```

## Examples

In *examples* folder are stored a few generated and tested algorithms with accuracy of 1 unit of least precision. More data about accuracy of each algorithm is shown below.

### 8-bit
|       | ErrSin[ULP] | ErrCos[ULP] |     ErrSin |     ErrCos |
|------:|------------:|------------:|-----------:|-----------:|
| count |  101.000000 |  101.000000 | 101.000000 | 101.000000 |
|  mean |    0.603316 |    0.485556 |   0.009427 |   0.007587 |
|   std |    0.307628 |    0.277389 |   0.004807 |   0.004334 |
|   min |    0.000000 |    0.004376 |   0.000000 |   0.000068 |
|   25% |    0.357473 |    0.251279 |   0.005586 |   0.003926 |
|   50% |    0.630724 |    0.495775 |   0.009855 |   0.007746 |
|   75% |    0.887896 |    0.689698 |   0.013873 |   0.010777 |
|   max |    0.999959 |    1.000000 |   0.015624 |   0.015625 |

### 16-bit
|       |  ErrSin[ULP] |  ErrCos[ULP] |       ErrSin |       ErrCos |
|------:|-------------:|-------------:|-------------:|-------------:|
| count | 25735.000000 | 25735.000000 | 25735.000000 | 25735.000000 |
|  mean |     0.513800 |     0.511172 |     0.000031 |     0.000031 |
|   std |     0.293323 |     0.290555 |     0.000018 |     0.000018 |
|   min |     0.000000 |     0.000000 |     0.000000 |     0.000000 |
|   25% |     0.259658 |     0.258500 |     0.000016 |     0.000016 |
|   50% |     0.517638 |     0.515402 |     0.000032 |     0.000031 |
|   75% |     0.770506 |     0.768069 |     0.000047 |     0.000047 |
|   max |     1.000000 |     0.999972 |     0.000061 |     0.000061 |

### 30-bit
|       |  ErrSin[ULP] |  ErrCos[ULP] |       ErrSin |       ErrCos |
|------:|-------------:|-------------:|-------------:|-------------:|
| count | 42166.000000 | 42166.000000 | 4.216600e+04 | 4.216600e+04 |
|  mean |     0.499999 |     0.499731 | 1.862641e-09 | 1.861641e-09 |
|   std |     0.289466 |     0.289058 | 1.078346e-09 | 1.076824e-09 |
|   min |     0.000000 |     0.000000 | 0.000000e+00 | 0.000000e+00 |
|   25% |     0.248356 |     0.248904 | 9.251998e-10 | 9.272413e-10 |
|   50% |     0.499202 |     0.498373 | 1.859671e-09 | 1.856586e-09 |
|   75% |     0.752077 |     0.750560 | 2.801706e-09 | 2.796055e-09 |
|   max |     0.999998 |     0.999980 | 3.725282e-09 | 3.725215e-09 |