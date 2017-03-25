#ifndef CPGF_GPP_H
#define CPGF_GPP_H

// preprocessor library

#define GPP_EMPTY()
#define GPP_EMPTY_N(...)

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

#define GPP_MAX_LIMIT        60

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
#define I_GPP_REPEAT_1_31(M, P)       I_GPP_REPEAT_1_30(M, P) M(30, P)
#define I_GPP_REPEAT_1_32(M, P)       I_GPP_REPEAT_1_31(M, P) M(31, P)
#define I_GPP_REPEAT_1_33(M, P)       I_GPP_REPEAT_1_32(M, P) M(32, P)
#define I_GPP_REPEAT_1_34(M, P)       I_GPP_REPEAT_1_33(M, P) M(33, P)
#define I_GPP_REPEAT_1_35(M, P)       I_GPP_REPEAT_1_34(M, P) M(34, P)
#define I_GPP_REPEAT_1_36(M, P)       I_GPP_REPEAT_1_35(M, P) M(35, P)
#define I_GPP_REPEAT_1_37(M, P)       I_GPP_REPEAT_1_36(M, P) M(36, P)
#define I_GPP_REPEAT_1_38(M, P)       I_GPP_REPEAT_1_37(M, P) M(37, P)
#define I_GPP_REPEAT_1_39(M, P)       I_GPP_REPEAT_1_38(M, P) M(38, P)
#define I_GPP_REPEAT_1_40(M, P)       I_GPP_REPEAT_1_39(M, P) M(39, P)
#define I_GPP_REPEAT_1_41(M, P)       I_GPP_REPEAT_1_40(M, P) M(40, P)
#define I_GPP_REPEAT_1_42(M, P)       I_GPP_REPEAT_1_41(M, P) M(41, P)
#define I_GPP_REPEAT_1_43(M, P)       I_GPP_REPEAT_1_42(M, P) M(42, P)
#define I_GPP_REPEAT_1_44(M, P)       I_GPP_REPEAT_1_43(M, P) M(43, P)
#define I_GPP_REPEAT_1_45(M, P)       I_GPP_REPEAT_1_44(M, P) M(44, P)
#define I_GPP_REPEAT_1_46(M, P)       I_GPP_REPEAT_1_45(M, P) M(45, P)
#define I_GPP_REPEAT_1_47(M, P)       I_GPP_REPEAT_1_46(M, P) M(46, P)
#define I_GPP_REPEAT_1_48(M, P)       I_GPP_REPEAT_1_47(M, P) M(47, P)
#define I_GPP_REPEAT_1_49(M, P)       I_GPP_REPEAT_1_48(M, P) M(48, P)
#define I_GPP_REPEAT_1_50(M, P)       I_GPP_REPEAT_1_49(M, P) M(49, P)
#define I_GPP_REPEAT_1_51(M, P)       I_GPP_REPEAT_1_50(M, P) M(50, P)
#define I_GPP_REPEAT_1_52(M, P)       I_GPP_REPEAT_1_51(M, P) M(51, P)
#define I_GPP_REPEAT_1_53(M, P)       I_GPP_REPEAT_1_52(M, P) M(52, P)
#define I_GPP_REPEAT_1_54(M, P)       I_GPP_REPEAT_1_53(M, P) M(53, P)
#define I_GPP_REPEAT_1_55(M, P)       I_GPP_REPEAT_1_54(M, P) M(54, P)
#define I_GPP_REPEAT_1_56(M, P)       I_GPP_REPEAT_1_55(M, P) M(55, P)
#define I_GPP_REPEAT_1_57(M, P)       I_GPP_REPEAT_1_56(M, P) M(56, P)
#define I_GPP_REPEAT_1_58(M, P)       I_GPP_REPEAT_1_57(M, P) M(57, P)
#define I_GPP_REPEAT_1_59(M, P)       I_GPP_REPEAT_1_58(M, P) M(58, P)
#define I_GPP_REPEAT_1_60(M, P)       I_GPP_REPEAT_1_59(M, P) M(59, P)

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
#define I_GPP_REPEAT_2_31(M, P)       I_GPP_REPEAT_2_30(M, P) M(30, P)
#define I_GPP_REPEAT_2_32(M, P)       I_GPP_REPEAT_2_31(M, P) M(31, P)
#define I_GPP_REPEAT_2_33(M, P)       I_GPP_REPEAT_2_32(M, P) M(32, P)
#define I_GPP_REPEAT_2_34(M, P)       I_GPP_REPEAT_2_33(M, P) M(33, P)
#define I_GPP_REPEAT_2_35(M, P)       I_GPP_REPEAT_2_34(M, P) M(34, P)
#define I_GPP_REPEAT_2_36(M, P)       I_GPP_REPEAT_2_35(M, P) M(35, P)
#define I_GPP_REPEAT_2_37(M, P)       I_GPP_REPEAT_2_36(M, P) M(36, P)
#define I_GPP_REPEAT_2_38(M, P)       I_GPP_REPEAT_2_37(M, P) M(37, P)
#define I_GPP_REPEAT_2_39(M, P)       I_GPP_REPEAT_2_38(M, P) M(38, P)
#define I_GPP_REPEAT_2_40(M, P)       I_GPP_REPEAT_2_39(M, P) M(39, P)
#define I_GPP_REPEAT_2_41(M, P)       I_GPP_REPEAT_2_40(M, P) M(40, P)
#define I_GPP_REPEAT_2_42(M, P)       I_GPP_REPEAT_2_41(M, P) M(41, P)
#define I_GPP_REPEAT_2_43(M, P)       I_GPP_REPEAT_2_42(M, P) M(42, P)
#define I_GPP_REPEAT_2_44(M, P)       I_GPP_REPEAT_2_43(M, P) M(43, P)
#define I_GPP_REPEAT_2_45(M, P)       I_GPP_REPEAT_2_44(M, P) M(44, P)
#define I_GPP_REPEAT_2_46(M, P)       I_GPP_REPEAT_2_45(M, P) M(45, P)
#define I_GPP_REPEAT_2_47(M, P)       I_GPP_REPEAT_2_46(M, P) M(46, P)
#define I_GPP_REPEAT_2_48(M, P)       I_GPP_REPEAT_2_47(M, P) M(47, P)
#define I_GPP_REPEAT_2_49(M, P)       I_GPP_REPEAT_2_48(M, P) M(48, P)
#define I_GPP_REPEAT_2_50(M, P)       I_GPP_REPEAT_2_49(M, P) M(49, P)
#define I_GPP_REPEAT_2_51(M, P)       I_GPP_REPEAT_2_50(M, P) M(50, P)
#define I_GPP_REPEAT_2_52(M, P)       I_GPP_REPEAT_2_51(M, P) M(51, P)
#define I_GPP_REPEAT_2_53(M, P)       I_GPP_REPEAT_2_52(M, P) M(52, P)
#define I_GPP_REPEAT_2_54(M, P)       I_GPP_REPEAT_2_53(M, P) M(53, P)
#define I_GPP_REPEAT_2_55(M, P)       I_GPP_REPEAT_2_54(M, P) M(54, P)
#define I_GPP_REPEAT_2_56(M, P)       I_GPP_REPEAT_2_55(M, P) M(55, P)
#define I_GPP_REPEAT_2_57(M, P)       I_GPP_REPEAT_2_56(M, P) M(56, P)
#define I_GPP_REPEAT_2_58(M, P)       I_GPP_REPEAT_2_57(M, P) M(57, P)
#define I_GPP_REPEAT_2_59(M, P)       I_GPP_REPEAT_2_58(M, P) M(58, P)
#define I_GPP_REPEAT_2_60(M, P)       I_GPP_REPEAT_2_59(M, P) M(59, P)

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
#define I_GPP_REPEAT_3_31(M, P)       I_GPP_REPEAT_3_30(M, P) M(30, P)
#define I_GPP_REPEAT_3_32(M, P)       I_GPP_REPEAT_3_31(M, P) M(31, P)
#define I_GPP_REPEAT_3_33(M, P)       I_GPP_REPEAT_3_32(M, P) M(32, P)
#define I_GPP_REPEAT_3_34(M, P)       I_GPP_REPEAT_3_33(M, P) M(33, P)
#define I_GPP_REPEAT_3_35(M, P)       I_GPP_REPEAT_3_34(M, P) M(34, P)
#define I_GPP_REPEAT_3_36(M, P)       I_GPP_REPEAT_3_35(M, P) M(35, P)
#define I_GPP_REPEAT_3_37(M, P)       I_GPP_REPEAT_3_36(M, P) M(36, P)
#define I_GPP_REPEAT_3_38(M, P)       I_GPP_REPEAT_3_37(M, P) M(37, P)
#define I_GPP_REPEAT_3_39(M, P)       I_GPP_REPEAT_3_38(M, P) M(38, P)
#define I_GPP_REPEAT_3_40(M, P)       I_GPP_REPEAT_3_39(M, P) M(39, P)
#define I_GPP_REPEAT_3_41(M, P)       I_GPP_REPEAT_3_40(M, P) M(40, P)
#define I_GPP_REPEAT_3_42(M, P)       I_GPP_REPEAT_3_41(M, P) M(41, P)
#define I_GPP_REPEAT_3_43(M, P)       I_GPP_REPEAT_3_42(M, P) M(42, P)
#define I_GPP_REPEAT_3_44(M, P)       I_GPP_REPEAT_3_43(M, P) M(43, P)
#define I_GPP_REPEAT_3_45(M, P)       I_GPP_REPEAT_3_44(M, P) M(44, P)
#define I_GPP_REPEAT_3_46(M, P)       I_GPP_REPEAT_3_45(M, P) M(45, P)
#define I_GPP_REPEAT_3_47(M, P)       I_GPP_REPEAT_3_46(M, P) M(46, P)
#define I_GPP_REPEAT_3_48(M, P)       I_GPP_REPEAT_3_47(M, P) M(47, P)
#define I_GPP_REPEAT_3_49(M, P)       I_GPP_REPEAT_3_48(M, P) M(48, P)
#define I_GPP_REPEAT_3_50(M, P)       I_GPP_REPEAT_3_49(M, P) M(49, P)
#define I_GPP_REPEAT_3_51(M, P)       I_GPP_REPEAT_3_50(M, P) M(50, P)
#define I_GPP_REPEAT_3_52(M, P)       I_GPP_REPEAT_3_51(M, P) M(51, P)
#define I_GPP_REPEAT_3_53(M, P)       I_GPP_REPEAT_3_52(M, P) M(52, P)
#define I_GPP_REPEAT_3_54(M, P)       I_GPP_REPEAT_3_53(M, P) M(53, P)
#define I_GPP_REPEAT_3_55(M, P)       I_GPP_REPEAT_3_54(M, P) M(54, P)
#define I_GPP_REPEAT_3_56(M, P)       I_GPP_REPEAT_3_55(M, P) M(55, P)
#define I_GPP_REPEAT_3_57(M, P)       I_GPP_REPEAT_3_56(M, P) M(56, P)
#define I_GPP_REPEAT_3_58(M, P)       I_GPP_REPEAT_3_57(M, P) M(57, P)
#define I_GPP_REPEAT_3_59(M, P)       I_GPP_REPEAT_3_58(M, P) M(58, P)
#define I_GPP_REPEAT_3_60(M, P)       I_GPP_REPEAT_3_59(M, P) M(59, P)

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
#define I_GPP_DEC_31       30
#define I_GPP_DEC_32       31
#define I_GPP_DEC_33       32
#define I_GPP_DEC_34       33
#define I_GPP_DEC_35       34
#define I_GPP_DEC_36       35
#define I_GPP_DEC_37       36
#define I_GPP_DEC_38       37
#define I_GPP_DEC_39       38
#define I_GPP_DEC_40       39
#define I_GPP_DEC_41       40
#define I_GPP_DEC_42       41
#define I_GPP_DEC_43       42
#define I_GPP_DEC_44       43
#define I_GPP_DEC_45       44
#define I_GPP_DEC_46       45
#define I_GPP_DEC_47       46
#define I_GPP_DEC_48       47
#define I_GPP_DEC_49       48
#define I_GPP_DEC_50       49
#define I_GPP_DEC_51       50
#define I_GPP_DEC_52       51
#define I_GPP_DEC_53       52
#define I_GPP_DEC_54       53
#define I_GPP_DEC_55       54
#define I_GPP_DEC_56       55
#define I_GPP_DEC_57       56
#define I_GPP_DEC_58       57
#define I_GPP_DEC_59       58
#define I_GPP_DEC_60       59

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
#define I_GPP_INC_30       31
#define I_GPP_INC_31       32
#define I_GPP_INC_32       33
#define I_GPP_INC_33       34
#define I_GPP_INC_34       35
#define I_GPP_INC_35       36
#define I_GPP_INC_36       37
#define I_GPP_INC_37       38
#define I_GPP_INC_38       39
#define I_GPP_INC_39       40
#define I_GPP_INC_40       41
#define I_GPP_INC_41       42
#define I_GPP_INC_42       43
#define I_GPP_INC_43       44
#define I_GPP_INC_44       45
#define I_GPP_INC_45       46
#define I_GPP_INC_46       47
#define I_GPP_INC_47       48
#define I_GPP_INC_48       49
#define I_GPP_INC_49       50
#define I_GPP_INC_50       51
#define I_GPP_INC_51       52
#define I_GPP_INC_52       53
#define I_GPP_INC_53       54
#define I_GPP_INC_54       55
#define I_GPP_INC_55       56
#define I_GPP_INC_56       57
#define I_GPP_INC_57       58
#define I_GPP_INC_58       59
#define I_GPP_INC_59       60

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
#define I_GPP_BOOL_31       1
#define I_GPP_BOOL_32       1
#define I_GPP_BOOL_33       1
#define I_GPP_BOOL_34       1
#define I_GPP_BOOL_35       1
#define I_GPP_BOOL_36       1
#define I_GPP_BOOL_37       1
#define I_GPP_BOOL_38       1
#define I_GPP_BOOL_39       1
#define I_GPP_BOOL_40       1
#define I_GPP_BOOL_41       1
#define I_GPP_BOOL_42       1
#define I_GPP_BOOL_43       1
#define I_GPP_BOOL_44       1
#define I_GPP_BOOL_45       1
#define I_GPP_BOOL_46       1
#define I_GPP_BOOL_47       1
#define I_GPP_BOOL_48       1
#define I_GPP_BOOL_49       1
#define I_GPP_BOOL_50       1
#define I_GPP_BOOL_51       1
#define I_GPP_BOOL_52       1
#define I_GPP_BOOL_53       1
#define I_GPP_BOOL_54       1
#define I_GPP_BOOL_55       1
#define I_GPP_BOOL_56       1
#define I_GPP_BOOL_57       1
#define I_GPP_BOOL_58       1
#define I_GPP_BOOL_59       1
#define I_GPP_BOOL_60       1

//auto generated macros -- end




#endif

