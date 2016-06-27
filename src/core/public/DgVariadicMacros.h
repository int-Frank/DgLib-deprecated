//! @file DgVariadicMacros.h
//!
//! @author Frank Hart
//! @date 4/10/2015

#ifndef DGVARIADICMACROS_H
#define DGVARIADICMACROS_H

#ifdef _MSC_VER // Microsoft compilers

#define EXPAND(x) x
#define __NARGS(_1, _2, _3, _4, _5, _6, _7, _8, _9, \
                _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, \
                _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, \
                _30, _31, _32, _33,  VAL, ...) VAL

#define NARGS_1(...) EXPAND(__NARGS(__VA_ARGS__,\
                32, 31, 30, \
                29, 28, 27, 26, 25, 24, 23, 22, 21, 20, \
                19, 18, 17, 16, 15, 14, 13, 12, 11, 10, \
                9, 8, 7, 6, 5, 4, 3, 2, 1, 0))

#define AUGMENTER(...) unused, __VA_ARGS__
#define NARGS(...) NARGS_1(AUGMENTER(__VA_ARGS__))

#else // TODO

#define NARGS(...) __NARGS(0, ## __VA_ARGS__, 5,4,3,2,1,0)
#define __NARGS(_0,_1,_2,_3,_4,_5,N,...) N

#endif

#define GLUE(x, y) x##y

#define ADD_ITEM2(itemName, t0, n0) ADD_SINGLE_ ## itemName(t0, n0)
#define ADD_ITEM4(itemName, t0, n0, t1, n1) ADD_SINGLE_ ## itemName(t0, n0) ADD_ITEM2(itemName, t1, n1)
#define ADD_ITEM6(itemName, t0, n0, t1, n1, t2, n2) ADD_SINGLE_ ## itemName(t0, n0) ADD_ITEM4(itemName, t1, n1, t2, n2)
#define ADD_ITEM8(itemName, t0, n0, t1, n1, t2, n2, t3, n3) ADD_SINGLE_ ## itemName(t0, n0) ADD_ITEM6(itemName, t1, n1, t2, n2, t3, n3)
#define ADD_ITEM10(itemName, t0, n0, t1, n1, t2, n2, t3, n3, t4, n4) ADD_SINGLE_ ## itemName(t0, n0) ADD_ITEM8(itemName, t1, n1, t2, n2, t3, n3, t4, n4)
#define ADD_ITEM12(itemName, t0, n0, t1, n1, t2, n2, t3, n3, t4, n4, t5, n5) ADD_SINGLE_ ## itemName(t0, n0) ADD_ITEM10(itemName, t1, n1, t2, n2, t3, n3, t4, n4, t5, n5)
#define ADD_ITEM14(itemName, t0, n0, t1, n1, t2, n2, t3, n3, t4, n4, t5, n5, t6, n6) ADD_SINGLE_ ## itemName(t0, n0) ADD_ITEM12(itemName, t1, n1, t2, n2, t3, n3, t4, n4, t5, n5, t6, n6)
#define ADD_ITEM16(itemName, t0, n0, t1, n1, t2, n2, t3, n3, t4, n4, t5, n5, t6, n6, t7, n7) ADD_SINGLE_ ## itemName(t0, n0) ADD_ITEM14(itemName, t1, n1, t2, n2, t3, n3, t4, n4, t5, n5, t6, n6, t7, n7)
#define ADD_ITEM18(itemName, t0, n0, t1, n1, t2, n2, t3, n3, t4, n4, t5, n5, t6, n6, t7, n7, t8, n8) ADD_SINGLE_ ## itemName(t0, n0) ADD_ITEM16(itemName, t1, n1, t2, n2, t3, n3, t4, n4, t5, n5, t6, n6, t7, n7, t8, n8)
#define ADD_ITEM20(itemName, t0, n0, t1, n1, t2, n2, t3, n3, t4, n4, t5, n5, t6, n6, t7, n7, t8, n8, t9, n9) ADD_SINGLE_ ## itemName(t0, n0) ADD_ITEM18(itemName, t1, n1, t2, n2, t3, n3, t4, n4, t5, n5, t6, n6, t7, n7, t8, n8, t9, n9)
#define ADD_ITEM22(itemName, t0, n0, t1, n1, t2, n2, t3, n3, t4, n4, t5, n5, t6, n6, t7, n7, t8, n8, t9, n9, t10, n10) ADD_SINGLE_ ## itemName(t0, n0) ADD_ITEM20(itemName, t1, n1, t2, n2, t3, n3, t4, n4, t5, n5, t6, n6, t7, n7, t8, n8, t9, n9, t10, n10)
#define ADD_ITEM24(itemName, t0, n0, t1, n1, t2, n2, t3, n3, t4, n4, t5, n5, t6, n6, t7, n7, t8, n8, t9, n9, t10, n10, t11, n11) ADD_SINGLE_ ## itemName(t0, n0) ADD_ITEM22(itemName, t1, n1, t2, n2, t3, n3, t4, n4, t5, n5, t6, n6, t7, n7, t8, n8, t9, n9, t10, n10, t11, n11)
#define ADD_ITEM26(itemName, t0, n0, t1, n1, t2, n2, t3, n3, t4, n4, t5, n5, t6, n6, t7, n7, t8, n8, t9, n9, t10, n10, t11, n11, t12, n12) ADD_SINGLE_ ## itemName(t0, n0) ADD_ITEM24(itemName, t1, n1, t2, n2, t3, n3, t4, n4, t5, n5, t6, n6, t7, n7, t8, n8, t9, n9, t10, n10, t11, n11, t12, n12)
#define ADD_ITEM28(itemName, t0, n0, t1, n1, t2, n2, t3, n3, t4, n4, t5, n5, t6, n6, t7, n7, t8, n8, t9, n9, t10, n10, t11, n11, t12, n12, t13, n13) ADD_SINGLE_ ## itemName(t0, n0) ADD_ITEM26(itemName, t1, n1, t2, n2, t3, n3, t4, n4, t5, n5, t6, n6, t7, n7, t8, n8, t9, n9, t10, n10, t11, n11, t12, n12, t13, n13)
#define ADD_ITEM30(itemName, t0, n0, t1, n1, t2, n2, t3, n3, t4, n4, t5, n5, t6, n6, t7, n7, t8, n8, t9, n9, t10, n10, t11, n11, t12, n12, t13, n13, t14, n14) ADD_SINGLE_ ## itemName(t0, n0) ADD_ITEM28(itemName, t1, n1, t2, n2, t3, n3, t4, n4, t5, n5, t6, n6, t7, n7, t8, n8, t9, n9, t10, n10, t11, n11, t12, n12, t13, n13, t14, n14)
#define ADD_ITEM32(itemName, t0, n0, t1, n1, t2, n2, t3, n3, t4, n4, t5, n5, t6, n6, t7, n7, t8, n8, t9, n9, t10, n10, t11, n11, t12, n12, t13, n13, t14, n14, t15, n15) ADD_SINGLE_ ## itemName(t0, n0) ADD_ITEM30(itemName, t1, n1, t2, n2, t3, n3, t4, n4, t5, n5, t6, n6, t7, n7, t8, n8, t9, n9, t10, n10, t11, n11, t12, n12, t13, n13, t14, n14, t15, n15)
#define ADD_ITEM34(itemName, t0, n0, t1, n1, t2, n2, t3, n3, t4, n4, t5, n5, t6, n6, t7, n7, t8, n8, t9, n9, t10, n10, t11, n11, t12, n12, t13, n13, t14, n14, t15, n15, t16, n16) ADD_SINGLE_ ## itemName(t0, n0) ADD_ITEM32(itemName, t1, n1, t2, n2, t3, n3, t4, n4, t5, n5, t6, n6, t7, n7, t8, n8, t9, n9, t10, n10, t11, n11, t12, n12, t13, n13, t14, n14, t15, n15, t16, n16)
#define ADD_ITEM36(itemName, t0, n0, t1, n1, t2, n2, t3, n3, t4, n4, t5, n5, t6, n6, t7, n7, t8, n8, t9, n9, t10, n10, t11, n11, t12, n12, t13, n13, t14, n14, t15, n15, t16, n16, t17, n17) ADD_SINGLE_ ## itemName(t0, n0) ADD_ITEM34(itemName, t1, n1, t2, n2, t3, n3, t4, n4, t5, n5, t6, n6, t7, n7, t8, n8, t9, n9, t10, n10, t11, n11, t12, n12, t13, n13, t14, n14, t15, n15, t16, n16, t17, n17)
#define ADD_ITEM38(itemName, t0, n0, t1, n1, t2, n2, t3, n3, t4, n4, t5, n5, t6, n6, t7, n7, t8, n8, t9, n9, t10, n10, t11, n11, t12, n12, t13, n13, t14, n14, t15, n15, t16, n16, t17, n17, t18, n18) ADD_SINGLE_ ## itemName(t0, n0) ADD_ITEM36(itemName, t1, n1, t2, n2, t3, n3, t4, n4, t5, n5, t6, n6, t7, n7, t8, n8, t9, n9, t10, n10, t11, n11, t12, n12, t13, n13, t14, n14, t15, n15, t16, n16, t17, n17, t18, n18)
#define ADD_ITEM40(itemName, t0, n0, t1, n1, t2, n2, t3, n3, t4, n4, t5, n5, t6, n6, t7, n7, t8, n8, t9, n9, t10, n10, t11, n11, t12, n12, t13, n13, t14, n14, t15, n15, t16, n16, t17, n17, t18, n18, t19, n19) ADD_SINGLE_ ## itemName(t0, n0) ADD_ITEM38(itemName, t1, n1, t2, n2, t3, n3, t4, n4, t5, n5, t6, n6, t7, n7, t8, n8, t9, n9, t10, n10, t11, n11, t12, n12, t13, n13, t14, n14, t15, n15, t16, n16, t17, n17, t18, n18, t19, n19)
#define ADD_ITEM42(itemName, t0, n0, t1, n1, t2, n2, t3, n3, t4, n4, t5, n5, t6, n6, t7, n7, t8, n8, t9, n9, t10, n10, t11, n11, t12, n12, t13, n13, t14, n14, t15, n15, t16, n16, t17, n17, t18, n18, t19, n19, t20, n20) ADD_SINGLE_ ## itemName(t0, n0) ADD_ITEM40(itemName, t1, n1, t2, n2, t3, n3, t4, n4, t5, n5, t6, n6, t7, n7, t8, n8, t9, n9, t10, n10, t11, n11, t12, n12, t13, n13, t14, n14, t15, n15, t16, n16, t17, n17, t18, n18, t19, n19, t20, n20)
#define ADD_ITEM44(itemName, t0, n0, t1, n1, t2, n2, t3, n3, t4, n4, t5, n5, t6, n6, t7, n7, t8, n8, t9, n9, t10, n10, t11, n11, t12, n12, t13, n13, t14, n14, t15, n15, t16, n16, t17, n17, t18, n18, t19, n19, t20, n20, t21, n21) ADD_SINGLE_ ## itemName(t0, n0) ADD_ITEM42(itemName, t1, n1, t2, n2, t3, n3, t4, n4, t5, n5, t6, n6, t7, n7, t8, n8, t9, n9, t10, n10, t11, n11, t12, n12, t13, n13, t14, n14, t15, n15, t16, n16, t17, n17, t18, n18, t19, n19, t20, n20, t21, n21)
#define ADD_ITEM46(itemName, t0, n0, t1, n1, t2, n2, t3, n3, t4, n4, t5, n5, t6, n6, t7, n7, t8, n8, t9, n9, t10, n10, t11, n11, t12, n12, t13, n13, t14, n14, t15, n15, t16, n16, t17, n17, t18, n18, t19, n19, t20, n20, t21, n21, t22, n22) ADD_SINGLE_ ## itemName(t0, n0) ADD_ITEM44(itemName, t1, n1, t2, n2, t3, n3, t4, n4, t5, n5, t6, n6, t7, n7, t8, n8, t9, n9, t10, n10, t11, n11, t12, n12, t13, n13, t14, n14, t15, n15, t16, n16, t17, n17, t18, n18, t19, n19, t20, n20, t21, n21, t22, n22)
#define ADD_ITEM48(itemName, t0, n0, t1, n1, t2, n2, t3, n3, t4, n4, t5, n5, t6, n6, t7, n7, t8, n8, t9, n9, t10, n10, t11, n11, t12, n12, t13, n13, t14, n14, t15, n15, t16, n16, t17, n17, t18, n18, t19, n19, t20, n20, t21, n21, t22, n22, t23, n23) ADD_SINGLE_ ## itemName(t0, n0) ADD_ITEM46(itemName, t1, n1, t2, n2, t3, n3, t4, n4, t5, n5, t6, n6, t7, n7, t8, n8, t9, n9, t10, n10, t11, n11, t12, n12, t13, n13, t14, n14, t15, n15, t16, n16, t17, n17, t18, n18, t19, n19, t20, n20, t21, n21, t22, n22, t23, n23)
#define ADD_ITEM50(itemName, t0, n0, t1, n1, t2, n2, t3, n3, t4, n4, t5, n5, t6, n6, t7, n7, t8, n8, t9, n9, t10, n10, t11, n11, t12, n12, t13, n13, t14, n14, t15, n15, t16, n16, t17, n17, t18, n18, t19, n19, t20, n20, t21, n21, t22, n22, t23, n23, t24, n24) ADD_SINGLE_ ## itemName(t0, n0) ADD_ITEM48(itemName, t1, n1, t2, n2, t3, n3, t4, n4, t5, n5, t6, n6, t7, n7, t8, n8, t9, n9, t10, n10, t11, n11, t12, n12, t13, n13, t14, n14, t15, n15, t16, n16, t17, n17, t18, n18, t19, n19, t20, n20, t21, n21, t22, n22, t23, n23, t24, n24)
#define ADD_ITEM52(itemName, t0, n0, t1, n1, t2, n2, t3, n3, t4, n4, t5, n5, t6, n6, t7, n7, t8, n8, t9, n9, t10, n10, t11, n11, t12, n12, t13, n13, t14, n14, t15, n15, t16, n16, t17, n17, t18, n18, t19, n19, t20, n20, t21, n21, t22, n22, t23, n23, t24, n24, t25, n25) ADD_SINGLE_ ## itemName(t0, n0) ADD_ITEM50(itemName, t1, n1, t2, n2, t3, n3, t4, n4, t5, n5, t6, n6, t7, n7, t8, n8, t9, n9, t10, n10, t11, n11, t12, n12, t13, n13, t14, n14, t15, n15, t16, n16, t17, n17, t18, n18, t19, n19, t20, n20, t21, n21, t22, n22, t23, n23, t24, n24, t25, n25)
#define ADD_ITEM54(itemName, t0, n0, t1, n1, t2, n2, t3, n3, t4, n4, t5, n5, t6, n6, t7, n7, t8, n8, t9, n9, t10, n10, t11, n11, t12, n12, t13, n13, t14, n14, t15, n15, t16, n16, t17, n17, t18, n18, t19, n19, t20, n20, t21, n21, t22, n22, t23, n23, t24, n24, t25, n25, t26, n26) ADD_SINGLE_ ## itemName(t0, n0) ADD_ITEM52(itemName, t1, n1, t2, n2, t3, n3, t4, n4, t5, n5, t6, n6, t7, n7, t8, n8, t9, n9, t10, n10, t11, n11, t12, n12, t13, n13, t14, n14, t15, n15, t16, n16, t17, n17, t18, n18, t19, n19, t20, n20, t21, n21, t22, n22, t23, n23, t24, n24, t25, n25, t26, n26)
#define ADD_ITEM56(itemName, t0, n0, t1, n1, t2, n2, t3, n3, t4, n4, t5, n5, t6, n6, t7, n7, t8, n8, t9, n9, t10, n10, t11, n11, t12, n12, t13, n13, t14, n14, t15, n15, t16, n16, t17, n17, t18, n18, t19, n19, t20, n20, t21, n21, t22, n22, t23, n23, t24, n24, t25, n25, t26, n26, t27, n27) ADD_SINGLE_ ## itemName(t0, n0) ADD_ITEM54(itemName, t1, n1, t2, n2, t3, n3, t4, n4, t5, n5, t6, n6, t7, n7, t8, n8, t9, n9, t10, n10, t11, n11, t12, n12, t13, n13, t14, n14, t15, n15, t16, n16, t17, n17, t18, n18, t19, n19, t20, n20, t21, n21, t22, n22, t23, n23, t24, n24, t25, n25, t26, n26, t27, n27)
#define ADD_ITEM58(itemName, t0, n0, t1, n1, t2, n2, t3, n3, t4, n4, t5, n5, t6, n6, t7, n7, t8, n8, t9, n9, t10, n10, t11, n11, t12, n12, t13, n13, t14, n14, t15, n15, t16, n16, t17, n17, t18, n18, t19, n19, t20, n20, t21, n21, t22, n22, t23, n23, t24, n24, t25, n25, t26, n26, t27, n27, t28, n28) ADD_SINGLE_ ## itemName(t0, n0) ADD_ITEM56(itemName, t1, n1, t2, n2, t3, n3, t4, n4, t5, n5, t6, n6, t7, n7, t8, n8, t9, n9, t10, n10, t11, n11, t12, n12, t13, n13, t14, n14, t15, n15, t16, n16, t17, n17, t18, n18, t19, n19, t20, n20, t21, n21, t22, n22, t23, n23, t24, n24, t25, n25, t26, n26, t27, n27, t28, n28)
#define ADD_ITEM60(itemName, t0, n0, t1, n1, t2, n2, t3, n3, t4, n4, t5, n5, t6, n6, t7, n7, t8, n8, t9, n9, t10, n10, t11, n11, t12, n12, t13, n13, t14, n14, t15, n15, t16, n16, t17, n17, t18, n18, t19, n19, t20, n20, t21, n21, t22, n22, t23, n23, t24, n24, t25, n25, t26, n26, t27, n27, t28, n28, t29, n29) ADD_SINGLE_ ## itemName(t0, n0) ADD_ITEM58(itemName, t1, n1, t2, n2, t3, n3, t4, n4, t5, n5, t6, n6, t7, n7, t8, n8, t9, n9, t10, n10, t11, n11, t12, n12, t13, n13, t14, n14, t15, n15, t16, n16, t17, n17, t18, n18, t19, n19, t20, n20, t21, n21, t22, n22, t23, n23, t24, n24, t25, n25, t26, n26, t27, n27, t28, n28, t29, n29)
#define ADD_ITEM62(itemName, t0, n0, t1, n1, t2, n2, t3, n3, t4, n4, t5, n5, t6, n6, t7, n7, t8, n8, t9, n9, t10, n10, t11, n11, t12, n12, t13, n13, t14, n14, t15, n15, t16, n16, t17, n17, t18, n18, t19, n19, t20, n20, t21, n21, t22, n22, t23, n23, t24, n24, t25, n25, t26, n26, t27, n27, t28, n28, t29, n29, t30, n30) ADD_SINGLE_ ## itemName(t0, n0) ADD_ITEM60(itemName, t1, n1, t2, n2, t3, n3, t4, n4, t5, n5, t6, n6, t7, n7, t8, n8, t9, n9, t10, n10, t11, n11, t12, n12, t13, n13, t14, n14, t15, n15, t16, n16, t17, n17, t18, n18, t19, n19, t20, n20, t21, n21, t22, n22, t23, n23, t24, n24, t25, n25, t26, n26, t27, n27, t28, n28, t29, n29, t30, n30)
#define ADD_ITEM64(itemName, t0, n0, t1, n1, t2, n2, t3, n3, t4, n4, t5, n5, t6, n6, t7, n7, t8, n8, t9, n9, t10, n10, t11, n11, t12, n12, t13, n13, t14, n14, t15, n15, t16, n16, t17, n17, t18, n18, t19, n19, t20, n20, t21, n21, t22, n22, t23, n23, t24, n24, t25, n25, t26, n26, t27, n27, t28, n28, t29, n29, t30, n30, t31, n31) ADD_SINGLE_ ## itemName(t0, n0) ADD_ITEM62(itemName, t1, n1, t2, n2, t3, n3, t4, n4, t5, n5, t6, n6, t7, n7, t8, n8, t9, n9, t10, n10, t11, n11, t12, n12, t13, n13, t14, n14, t15, n15, t16, n16, t17, n17, t18, n18, t19, n19, t20, n20, t21, n21, t22, n22, t23, n23, t24, n24, t25, n25, t26, n26, t27, n27, t28, n28, t29, n29, t30, n30, t31, n31)

#define ADD_ITEM_HELPER2(count) ADD_ITEM##count
#define ADD_ITEM_HELPER1(count) ADD_ITEM_HELPER2(count)
#define ADD_ITEM_HELPER(count) ADD_ITEM_HELPER1(count)

#endif