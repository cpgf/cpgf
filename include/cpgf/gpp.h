#ifndef __GPP_H
#define __GPP_H

// preprocessor library

#define GPP_EMPTY()
#define GPP_COMMA() ,

#define GPP_STRINGIZE(s) I_GPP_STRINGIZE(s)

#define GPP_CONCAT(a, b) I_GPP_CONCAT(a, b)

// N - repeat count, M - macro to execute M(N, P), P - extra parameter to be passed to M
#define GPP_REPEAT(N, M, P)	        I_GPP_REPEAT(N, M, P)
#define GPP_REPEAT_2(N, M, P)       I_GPP_REPEAT_2(N, M, P)
#define GPP_REPEAT_3(N, M, P)       I_GPP_REPEAT_3(N, M, P)

#define GPP_INC(N)					I_GPP_INC(N)
#define GPP_DEC(N)					I_GPP_DEC(N)

#define GPP_BOOL(B)					I_GPP_BOOL(B)

#define GPP_IF(B, T, F)				I_GPP_IF(B, T, F)

#define GPP_DO_IF(N, D)				GPP_IF(N, D, GPP_EMPTY)()

#define GPP_COMMA_IF(N)				GPP_DO_IF(N, GPP_COMMA)

#define GPP_COMMA_PARAM(N, P)		GPP_COMMA_IF(N) P ## N

#define GPP_REPEAT_PARAMS(N, P)		I_GPP_REPEAT_PARAMS(N, P)
#define GPP_REPEAT_TAIL_PARAMS(N, P)	GPP_COMMA_IF(N) GPP_REPEAT_PARAMS(N, P)


// helper macros

#define I_GPP_STRINGIZE(s) #s

#define I_GPP_CONCAT(a, b) I2_GPP_CONCAT(a ## b)
#define I2_GPP_CONCAT(ab) ab

#define I_GPP_REPEAT(N, M, P)			I_GPP_REPEAT_1_##N(M, P)
#define I_GPP_REPEAT_2(N, M, P)			I_GPP_REPEAT_2_##N(M, P)
#define I_GPP_REPEAT_3(N, M, P)			I_GPP_REPEAT_3_##N(M, P)

#define I_GPP_INC(N)					I_GPP_INC_##N
#define I_GPP_DEC(N)					I_GPP_DEC_##N


#define I_GPP_BOOL(B) I_GPP_BOOL_ ## B

#define I_GPP_IF(B, T, F) I2_GPP_IF(GPP_BOOL(B), T, F)
#define I2_GPP_IF(B, T, F) I3_GPP_IF(B, T, F)
#define I3_GPP_IF(B, T, F) I4_GPP_IF(I_GPP_IF_ ## B (T, F))
#define I4_GPP_IF(B) B

#define I_GPP_IF_1(T, F) T
#define I_GPP_IF_0(T, F) F

#define I_GPP_REPEAT_PARAMS(N, P) GPP_REPEAT(N, I2_GPP_REPEAT_PARAMS, P)
#define I2_GPP_REPEAT_PARAMS(N, P) GPP_COMMA_IF(N) P ## N


//auto generated macros -- begin

#define GPP_MAX_LIMIT        30

#define I_GPP_REPEAT_1_0(M, P)
#define I_GPP_REPEAT_1_1(M, P)        M(0, P)
#define I_GPP_REPEAT_1_2(M, P)        I_GPP_REPEAT_1_1(M, P) M(1, P)
#define I_GPP_REPEAT_1_3(M, P)        I_GPP_REPEAT_1_2(M, P) M(2, P)
#define I_GPP_REPEAT_1_4(M, P)        I_GPP_REPEAT_1_3(M, P) M(3, P)
#define I_GPP_REPEAT_1_5(M, P)        I_GPP_REPEAT_1_4(M, P) M(4, P)
#define I_GPP_REPEAT_1_6(M, P)        I_GPP_REPEAT_1_5(M, P) M(5, P)
#define I_GPP_REPEAT_1_7(M, P)        I_GPP_REPEAT_1_6(M, P) M(6, P)
#define I_GPP_REPEAT_1_8(M, P)        I_GPP_REPEAT_1_7(M, P) M(7, P)
#define I_GPP_REPEAT_1_9(M, P)        I_GPP_REPEAT_1_8(M, P) M(8, P)
#define I_GPP_REPEAT_1_10(M, P)       I_GPP_REPEAT_1_9(M, P) M(9, P)
#define I_GPP_REPEAT_1_11(M, P)       I_GPP_REPEAT_1_10(M, P) M(10, P)
#define I_GPP_REPEAT_1_12(M, P)       I_GPP_REPEAT_1_11(M, P) M(11, P)
#define I_GPP_REPEAT_1_13(M, P)       I_GPP_REPEAT_1_12(M, P) M(12, P)
#define I_GPP_REPEAT_1_14(M, P)       I_GPP_REPEAT_1_13(M, P) M(13, P)
#define I_GPP_REPEAT_1_15(M, P)       I_GPP_REPEAT_1_14(M, P) M(14, P)
#define I_GPP_REPEAT_1_16(M, P)       I_GPP_REPEAT_1_15(M, P) M(15, P)
#define I_GPP_REPEAT_1_17(M, P)       I_GPP_REPEAT_1_16(M, P) M(16, P)
#define I_GPP_REPEAT_1_18(M, P)       I_GPP_REPEAT_1_17(M, P) M(17, P)
#define I_GPP_REPEAT_1_19(M, P)       I_GPP_REPEAT_1_18(M, P) M(18, P)
#define I_GPP_REPEAT_1_20(M, P)       I_GPP_REPEAT_1_19(M, P) M(19, P)
#define I_GPP_REPEAT_1_21(M, P)       I_GPP_REPEAT_1_20(M, P) M(20, P)
#define I_GPP_REPEAT_1_22(M, P)       I_GPP_REPEAT_1_21(M, P) M(21, P)
#define I_GPP_REPEAT_1_23(M, P)       I_GPP_REPEAT_1_22(M, P) M(22, P)
#define I_GPP_REPEAT_1_24(M, P)       I_GPP_REPEAT_1_23(M, P) M(23, P)
#define I_GPP_REPEAT_1_25(M, P)       I_GPP_REPEAT_1_24(M, P) M(24, P)
#define I_GPP_REPEAT_1_26(M, P)       I_GPP_REPEAT_1_25(M, P) M(25, P)
#define I_GPP_REPEAT_1_27(M, P)       I_GPP_REPEAT_1_26(M, P) M(26, P)
#define I_GPP_REPEAT_1_28(M, P)       I_GPP_REPEAT_1_27(M, P) M(27, P)
#define I_GPP_REPEAT_1_29(M, P)       I_GPP_REPEAT_1_28(M, P) M(28, P)
#define I_GPP_REPEAT_1_30(M, P)       I_GPP_REPEAT_1_29(M, P) M(29, P)

#define I_GPP_REPEAT_2_0(M, P)
#define I_GPP_REPEAT_2_1(M, P)        M(0, P)
#define I_GPP_REPEAT_2_2(M, P)        I_GPP_REPEAT_2_1(M, P) M(1, P)
#define I_GPP_REPEAT_2_3(M, P)        I_GPP_REPEAT_2_2(M, P) M(2, P)
#define I_GPP_REPEAT_2_4(M, P)        I_GPP_REPEAT_2_3(M, P) M(3, P)
#define I_GPP_REPEAT_2_5(M, P)        I_GPP_REPEAT_2_4(M, P) M(4, P)
#define I_GPP_REPEAT_2_6(M, P)        I_GPP_REPEAT_2_5(M, P) M(5, P)
#define I_GPP_REPEAT_2_7(M, P)        I_GPP_REPEAT_2_6(M, P) M(6, P)
#define I_GPP_REPEAT_2_8(M, P)        I_GPP_REPEAT_2_7(M, P) M(7, P)
#define I_GPP_REPEAT_2_9(M, P)        I_GPP_REPEAT_2_8(M, P) M(8, P)
#define I_GPP_REPEAT_2_10(M, P)       I_GPP_REPEAT_2_9(M, P) M(9, P)
#define I_GPP_REPEAT_2_11(M, P)       I_GPP_REPEAT_2_10(M, P) M(10, P)
#define I_GPP_REPEAT_2_12(M, P)       I_GPP_REPEAT_2_11(M, P) M(11, P)
#define I_GPP_REPEAT_2_13(M, P)       I_GPP_REPEAT_2_12(M, P) M(12, P)
#define I_GPP_REPEAT_2_14(M, P)       I_GPP_REPEAT_2_13(M, P) M(13, P)
#define I_GPP_REPEAT_2_15(M, P)       I_GPP_REPEAT_2_14(M, P) M(14, P)
#define I_GPP_REPEAT_2_16(M, P)       I_GPP_REPEAT_2_15(M, P) M(15, P)
#define I_GPP_REPEAT_2_17(M, P)       I_GPP_REPEAT_2_16(M, P) M(16, P)
#define I_GPP_REPEAT_2_18(M, P)       I_GPP_REPEAT_2_17(M, P) M(17, P)
#define I_GPP_REPEAT_2_19(M, P)       I_GPP_REPEAT_2_18(M, P) M(18, P)
#define I_GPP_REPEAT_2_20(M, P)       I_GPP_REPEAT_2_19(M, P) M(19, P)
#define I_GPP_REPEAT_2_21(M, P)       I_GPP_REPEAT_2_20(M, P) M(20, P)
#define I_GPP_REPEAT_2_22(M, P)       I_GPP_REPEAT_2_21(M, P) M(21, P)
#define I_GPP_REPEAT_2_23(M, P)       I_GPP_REPEAT_2_22(M, P) M(22, P)
#define I_GPP_REPEAT_2_24(M, P)       I_GPP_REPEAT_2_23(M, P) M(23, P)
#define I_GPP_REPEAT_2_25(M, P)       I_GPP_REPEAT_2_24(M, P) M(24, P)
#define I_GPP_REPEAT_2_26(M, P)       I_GPP_REPEAT_2_25(M, P) M(25, P)
#define I_GPP_REPEAT_2_27(M, P)       I_GPP_REPEAT_2_26(M, P) M(26, P)
#define I_GPP_REPEAT_2_28(M, P)       I_GPP_REPEAT_2_27(M, P) M(27, P)
#define I_GPP_REPEAT_2_29(M, P)       I_GPP_REPEAT_2_28(M, P) M(28, P)
#define I_GPP_REPEAT_2_30(M, P)       I_GPP_REPEAT_2_29(M, P) M(29, P)

#define I_GPP_REPEAT_3_0(M, P)
#define I_GPP_REPEAT_3_1(M, P)        M(0, P)
#define I_GPP_REPEAT_3_2(M, P)        I_GPP_REPEAT_3_1(M, P) M(1, P)
#define I_GPP_REPEAT_3_3(M, P)        I_GPP_REPEAT_3_2(M, P) M(2, P)
#define I_GPP_REPEAT_3_4(M, P)        I_GPP_REPEAT_3_3(M, P) M(3, P)
#define I_GPP_REPEAT_3_5(M, P)        I_GPP_REPEAT_3_4(M, P) M(4, P)
#define I_GPP_REPEAT_3_6(M, P)        I_GPP_REPEAT_3_5(M, P) M(5, P)
#define I_GPP_REPEAT_3_7(M, P)        I_GPP_REPEAT_3_6(M, P) M(6, P)
#define I_GPP_REPEAT_3_8(M, P)        I_GPP_REPEAT_3_7(M, P) M(7, P)
#define I_GPP_REPEAT_3_9(M, P)        I_GPP_REPEAT_3_8(M, P) M(8, P)
#define I_GPP_REPEAT_3_10(M, P)       I_GPP_REPEAT_3_9(M, P) M(9, P)
#define I_GPP_REPEAT_3_11(M, P)       I_GPP_REPEAT_3_10(M, P) M(10, P)
#define I_GPP_REPEAT_3_12(M, P)       I_GPP_REPEAT_3_11(M, P) M(11, P)
#define I_GPP_REPEAT_3_13(M, P)       I_GPP_REPEAT_3_12(M, P) M(12, P)
#define I_GPP_REPEAT_3_14(M, P)       I_GPP_REPEAT_3_13(M, P) M(13, P)
#define I_GPP_REPEAT_3_15(M, P)       I_GPP_REPEAT_3_14(M, P) M(14, P)
#define I_GPP_REPEAT_3_16(M, P)       I_GPP_REPEAT_3_15(M, P) M(15, P)
#define I_GPP_REPEAT_3_17(M, P)       I_GPP_REPEAT_3_16(M, P) M(16, P)
#define I_GPP_REPEAT_3_18(M, P)       I_GPP_REPEAT_3_17(M, P) M(17, P)
#define I_GPP_REPEAT_3_19(M, P)       I_GPP_REPEAT_3_18(M, P) M(18, P)
#define I_GPP_REPEAT_3_20(M, P)       I_GPP_REPEAT_3_19(M, P) M(19, P)
#define I_GPP_REPEAT_3_21(M, P)       I_GPP_REPEAT_3_20(M, P) M(20, P)
#define I_GPP_REPEAT_3_22(M, P)       I_GPP_REPEAT_3_21(M, P) M(21, P)
#define I_GPP_REPEAT_3_23(M, P)       I_GPP_REPEAT_3_22(M, P) M(22, P)
#define I_GPP_REPEAT_3_24(M, P)       I_GPP_REPEAT_3_23(M, P) M(23, P)
#define I_GPP_REPEAT_3_25(M, P)       I_GPP_REPEAT_3_24(M, P) M(24, P)
#define I_GPP_REPEAT_3_26(M, P)       I_GPP_REPEAT_3_25(M, P) M(25, P)
#define I_GPP_REPEAT_3_27(M, P)       I_GPP_REPEAT_3_26(M, P) M(26, P)
#define I_GPP_REPEAT_3_28(M, P)       I_GPP_REPEAT_3_27(M, P) M(27, P)
#define I_GPP_REPEAT_3_29(M, P)       I_GPP_REPEAT_3_28(M, P) M(28, P)
#define I_GPP_REPEAT_3_30(M, P)       I_GPP_REPEAT_3_29(M, P) M(29, P)

#define I_GPP_DEC_1        0
#define I_GPP_DEC_2        1
#define I_GPP_DEC_3        2
#define I_GPP_DEC_4        3
#define I_GPP_DEC_5        4
#define I_GPP_DEC_6        5
#define I_GPP_DEC_7        6
#define I_GPP_DEC_8        7
#define I_GPP_DEC_9        8
#define I_GPP_DEC_10       9
#define I_GPP_DEC_11       10
#define I_GPP_DEC_12       11
#define I_GPP_DEC_13       12
#define I_GPP_DEC_14       13
#define I_GPP_DEC_15       14
#define I_GPP_DEC_16       15
#define I_GPP_DEC_17       16
#define I_GPP_DEC_18       17
#define I_GPP_DEC_19       18
#define I_GPP_DEC_20       19
#define I_GPP_DEC_21       20
#define I_GPP_DEC_22       21
#define I_GPP_DEC_23       22
#define I_GPP_DEC_24       23
#define I_GPP_DEC_25       24
#define I_GPP_DEC_26       25
#define I_GPP_DEC_27       26
#define I_GPP_DEC_28       27
#define I_GPP_DEC_29       28
#define I_GPP_DEC_30       29

#define I_GPP_INC_0        1
#define I_GPP_INC_1        2
#define I_GPP_INC_2        3
#define I_GPP_INC_3        4
#define I_GPP_INC_4        5
#define I_GPP_INC_5        6
#define I_GPP_INC_6        7
#define I_GPP_INC_7        8
#define I_GPP_INC_8        9
#define I_GPP_INC_9        10
#define I_GPP_INC_10       11
#define I_GPP_INC_11       12
#define I_GPP_INC_12       13
#define I_GPP_INC_13       14
#define I_GPP_INC_14       15
#define I_GPP_INC_15       16
#define I_GPP_INC_16       17
#define I_GPP_INC_17       18
#define I_GPP_INC_18       19
#define I_GPP_INC_19       20
#define I_GPP_INC_20       21
#define I_GPP_INC_21       22
#define I_GPP_INC_22       23
#define I_GPP_INC_23       24
#define I_GPP_INC_24       25
#define I_GPP_INC_25       26
#define I_GPP_INC_26       27
#define I_GPP_INC_27       28
#define I_GPP_INC_28       29
#define I_GPP_INC_29       30

#define I_GPP_BOOL_0        0
#define I_GPP_BOOL_1        1
#define I_GPP_BOOL_2        1
#define I_GPP_BOOL_3        1
#define I_GPP_BOOL_4        1
#define I_GPP_BOOL_5        1
#define I_GPP_BOOL_6        1
#define I_GPP_BOOL_7        1
#define I_GPP_BOOL_8        1
#define I_GPP_BOOL_9        1
#define I_GPP_BOOL_10       1
#define I_GPP_BOOL_11       1
#define I_GPP_BOOL_12       1
#define I_GPP_BOOL_13       1
#define I_GPP_BOOL_14       1
#define I_GPP_BOOL_15       1
#define I_GPP_BOOL_16       1
#define I_GPP_BOOL_17       1
#define I_GPP_BOOL_18       1
#define I_GPP_BOOL_19       1
#define I_GPP_BOOL_20       1
#define I_GPP_BOOL_21       1
#define I_GPP_BOOL_22       1
#define I_GPP_BOOL_23       1
#define I_GPP_BOOL_24       1
#define I_GPP_BOOL_25       1
#define I_GPP_BOOL_26       1
#define I_GPP_BOOL_27       1
#define I_GPP_BOOL_28       1
#define I_GPP_BOOL_29       1
#define I_GPP_BOOL_30       1

//auto generated macros -- end



#endif

