//! @file DgVariadicMacros.h
//!
//! @author Frank Hart
//! @date 4/10/2016

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

#else // TODO Not tested

#define NARGS(...) __NARGS(0, ## __VA_ARGS__, 32, 31, 30, \
                29, 28, 27, 26, 25, 24, 23, 22, 21, 20, \
                19, 18, 17, 16, 15, 14, 13, 12, 11, 10, \
                9, 8, 7, 6, 5, 4, 3, 2, 1, 0)
#define __NARGS(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, \
                _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, \
                _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, \
                _30, _31, _32, N,...) N

#endif

#define ADD_ITEM0(itemName)
#define ADD_ITEM2(itemName, n0, t0) ADD_SINGLE_ ## itemName(n0, t0)
#define ADD_ITEM4(itemName, n0, t0, n1, t1) ADD_SINGLE_ ## itemName(n0, t0) ADD_ITEM2(itemName, n1, t1)
#define ADD_ITEM6(itemName, n0, t0, n1, t1, n2, t2) ADD_SINGLE_ ## itemName(n0, t0) ADD_ITEM4(itemName, n1, t1, n2, t2)
#define ADD_ITEM8(itemName, n0, t0, n1, t1, n2, t2, n3, t3) ADD_SINGLE_ ## itemName(n0, t0) ADD_ITEM6(itemName, n1, t1, n2, t2, n3, t3)
#define ADD_ITEM10(itemName, n0, t0, n1, t1, n2, t2, n3, t3, n4, t4) ADD_SINGLE_ ## itemName(n0, t0) ADD_ITEM8(itemName, n1, t1, n2, t2, n3, t3, n4, t4)
#define ADD_ITEM12(itemName, n0, t0, n1, t1, n2, t2, n3, t3, n4, t4, n5, t5) ADD_SINGLE_ ## itemName(n0, t0) ADD_ITEM10(itemName, n1, t1, n2, t2, n3, t3, n4, t4, n5, t5)
#define ADD_ITEM14(itemName, n0, t0, n1, t1, n2, t2, n3, t3, n4, t4, n5, t5, n6, t6) ADD_SINGLE_ ## itemName(n0, t0) ADD_ITEM12(itemName, n1, t1, n2, t2, n3, t3, n4, t4, n5, t5, n6, t6)
#define ADD_ITEM16(itemName, n0, t0, n1, t1, n2, t2, n3, t3, n4, t4, n5, t5, n6, t6, n7, t7) ADD_SINGLE_ ## itemName(n0, t0) ADD_ITEM14(itemName, n1, t1, n2, t2, n3, t3, n4, t4, n5, t5, n6, t6, n7, t7)
#define ADD_ITEM18(itemName, n0, t0, n1, t1, n2, t2, n3, t3, n4, t4, n5, t5, n6, t6, n7, t7, n8, t8) ADD_SINGLE_ ## itemName(n0, t0) ADD_ITEM16(itemName, n1, t1, n2, t2, n3, t3, n4, t4, n5, t5, n6, t6, n7, t7, n8, t8)
#define ADD_ITEM20(itemName, n0, t0, n1, t1, n2, t2, n3, t3, n4, t4, n5, t5, n6, t6, n7, t7, n8, t8, n9, t9) ADD_SINGLE_ ## itemName(n0, t0) ADD_ITEM18(itemName, n1, t1, n2, t2, n3, t3, n4, t4, n5, t5, n6, t6, n7, t7, n8, t8, n9, t9)
#define ADD_ITEM22(itemName, n0, t0, n1, t1, n2, t2, n3, t3, n4, t4, n5, t5, n6, t6, n7, t7, n8, t8, n9, t9, n10, t10) ADD_SINGLE_ ## itemName(n0, t0) ADD_ITEM20(itemName, n1, t1, n2, t2, n3, t3, n4, t4, n5, t5, n6, t6, n7, t7, n8, t8, n9, t9, n10, t10)
#define ADD_ITEM24(itemName, n0, t0, n1, t1, n2, t2, n3, t3, n4, t4, n5, t5, n6, t6, n7, t7, n8, t8, n9, t9, n10, t10, n11, t11) ADD_SINGLE_ ## itemName(n0, t0) ADD_ITEM22(itemName, n1, t1, n2, t2, n3, t3, n4, t4, n5, t5, n6, t6, n7, t7, n8, t8, n9, t9, n10, t10, n11, t11)
#define ADD_ITEM26(itemName, n0, t0, n1, t1, n2, t2, n3, t3, n4, t4, n5, t5, n6, t6, n7, t7, n8, t8, n9, t9, n10, t10, n11, t11, n12, t12) ADD_SINGLE_ ## itemName(n0, t0) ADD_ITEM24(itemName, n1, t1, n2, t2, n3, t3, n4, t4, n5, t5, n6, t6, n7, t7, n8, t8, n9, t9, n10, t10, n11, t11, n12, t12)
#define ADD_ITEM28(itemName, n0, t0, n1, t1, n2, t2, n3, t3, n4, t4, n5, t5, n6, t6, n7, t7, n8, t8, n9, t9, n10, t10, n11, t11, n12, t12, n13, t13) ADD_SINGLE_ ## itemName(n0, t0) ADD_ITEM26(itemName, n1, t1, n2, t2, n3, t3, n4, t4, n5, t5, n6, t6, n7, t7, n8, t8, n9, t9, n10, t10, n11, t11, n12, t12, n13, t13)
#define ADD_ITEM30(itemName, n0, t0, n1, t1, n2, t2, n3, t3, n4, t4, n5, t5, n6, t6, n7, t7, n8, t8, n9, t9, n10, t10, n11, t11, n12, t12, n13, t13, n14, t14) ADD_SINGLE_ ## itemName(n0, t0) ADD_ITEM28(itemName, n1, t1, n2, t2, n3, t3, n4, t4, n5, t5, n6, t6, n7, t7, n8, t8, n9, t9, n10, t10, n11, t11, n12, t12, n13, t13, n14, t14)
#define ADD_ITEM32(itemName, n0, t0, n1, t1, n2, t2, n3, t3, n4, t4, n5, t5, n6, t6, n7, t7, n8, t8, n9, t9, n10, t10, n11, t11, n12, t12, n13, t13, n14, t14, n15, t15) ADD_SINGLE_ ## itemName(n0, t0) ADD_ITEM30(itemName, n1, t1, n2, t2, n3, t3, n4, t4, n5, t5, n6, t6, n7, t7, n8, t8, n9, t9, n10, t10, n11, t11, n12, t12, n13, t13, n14, t14, n15, t15)
#define ADD_ITEM34(itemName, n0, t0, n1, t1, n2, t2, n3, t3, n4, t4, n5, t5, n6, t6, n7, t7, n8, t8, n9, t9, n10, t10, n11, t11, n12, t12, n13, t13, n14, t14, n15, t15, n16, t16) ADD_SINGLE_ ## itemName(n0, t0) ADD_ITEM32(itemName, n1, t1, n2, t2, n3, t3, n4, t4, n5, t5, n6, t6, n7, t7, n8, t8, n9, t9, n10, t10, n11, t11, n12, t12, n13, t13, n14, t14, n15, t15, n16, t16)
#define ADD_ITEM36(itemName, n0, t0, n1, t1, n2, t2, n3, t3, n4, t4, n5, t5, n6, t6, n7, t7, n8, t8, n9, t9, n10, t10, n11, t11, n12, t12, n13, t13, n14, t14, n15, t15, n16, t16, n17, t17) ADD_SINGLE_ ## itemName(n0, t0) ADD_ITEM34(itemName, n1, t1, n2, t2, n3, t3, n4, t4, n5, t5, n6, t6, n7, t7, n8, t8, n9, t9, n10, t10, n11, t11, n12, t12, n13, t13, n14, t14, n15, t15, n16, t16, n17, t17)
#define ADD_ITEM38(itemName, n0, t0, n1, t1, n2, t2, n3, t3, n4, t4, n5, t5, n6, t6, n7, t7, n8, t8, n9, t9, n10, t10, n11, t11, n12, t12, n13, t13, n14, t14, n15, t15, n16, t16, n17, t17, n18, t18) ADD_SINGLE_ ## itemName(n0, t0) ADD_ITEM36(itemName, n1, t1, n2, t2, n3, t3, n4, t4, n5, t5, n6, t6, n7, t7, n8, t8, n9, t9, n10, t10, n11, t11, n12, t12, n13, t13, n14, t14, n15, t15, n16, t16, n17, t17, n18, t18)
#define ADD_ITEM40(itemName, n0, t0, n1, t1, n2, t2, n3, t3, n4, t4, n5, t5, n6, t6, n7, t7, n8, t8, n9, t9, n10, t10, n11, t11, n12, t12, n13, t13, n14, t14, n15, t15, n16, t16, n17, t17, n18, t18, n19, t19) ADD_SINGLE_ ## itemName(n0, t0) ADD_ITEM38(itemName, n1, t1, n2, t2, n3, t3, n4, t4, n5, t5, n6, t6, n7, t7, n8, t8, n9, t9, n10, t10, n11, t11, n12, t12, n13, t13, n14, t14, n15, t15, n16, t16, n17, t17, n18, t18, n19, t19)
#define ADD_ITEM42(itemName, n0, t0, n1, t1, n2, t2, n3, t3, n4, t4, n5, t5, n6, t6, n7, t7, n8, t8, n9, t9, n10, t10, n11, t11, n12, t12, n13, t13, n14, t14, n15, t15, n16, t16, n17, t17, n18, t18, n19, t19, n20, t20) ADD_SINGLE_ ## itemName(n0, t0) ADD_ITEM40(itemName, n1, t1, n2, t2, n3, t3, n4, t4, n5, t5, n6, t6, n7, t7, n8, t8, n9, t9, n10, t10, n11, t11, n12, t12, n13, t13, n14, t14, n15, t15, n16, t16, n17, t17, n18, t18, n19, t19, n20, t20)
#define ADD_ITEM44(itemName, n0, t0, n1, t1, n2, t2, n3, t3, n4, t4, n5, t5, n6, t6, n7, t7, n8, t8, n9, t9, n10, t10, n11, t11, n12, t12, n13, t13, n14, t14, n15, t15, n16, t16, n17, t17, n18, t18, n19, t19, n20, t20, n21, t21) ADD_SINGLE_ ## itemName(n0, t0) ADD_ITEM42(itemName, n1, t1, n2, t2, n3, t3, n4, t4, n5, t5, n6, t6, n7, t7, n8, t8, n9, t9, n10, t10, n11, t11, n12, t12, n13, t13, n14, t14, n15, t15, n16, t16, n17, t17, n18, t18, n19, t19, n20, t20, n21, t21)
#define ADD_ITEM46(itemName, n0, t0, n1, t1, n2, t2, n3, t3, n4, t4, n5, t5, n6, t6, n7, t7, n8, t8, n9, t9, n10, t10, n11, t11, n12, t12, n13, t13, n14, t14, n15, t15, n16, t16, n17, t17, n18, t18, n19, t19, n20, t20, n21, t21, n22, t22) ADD_SINGLE_ ## itemName(n0, t0) ADD_ITEM44(itemName, n1, t1, n2, t2, n3, t3, n4, t4, n5, t5, n6, t6, n7, t7, n8, t8, n9, t9, n10, t10, n11, t11, n12, t12, n13, t13, n14, t14, n15, t15, n16, t16, n17, t17, n18, t18, n19, t19, n20, t20, n21, t21, n22, t22)
#define ADD_ITEM48(itemName, n0, t0, n1, t1, n2, t2, n3, t3, n4, t4, n5, t5, n6, t6, n7, t7, n8, t8, n9, t9, n10, t10, n11, t11, n12, t12, n13, t13, n14, t14, n15, t15, n16, t16, n17, t17, n18, t18, n19, t19, n20, t20, n21, t21, n22, t22, n23, t23) ADD_SINGLE_ ## itemName(n0, t0) ADD_ITEM46(itemName, n1, t1, n2, t2, n3, t3, n4, t4, n5, t5, n6, t6, n7, t7, n8, t8, n9, t9, n10, t10, n11, t11, n12, t12, n13, t13, n14, t14, n15, t15, n16, t16, n17, t17, n18, t18, n19, t19, n20, t20, n21, t21, n22, t22, n23, t23)
#define ADD_ITEM50(itemName, n0, t0, n1, t1, n2, t2, n3, t3, n4, t4, n5, t5, n6, t6, n7, t7, n8, t8, n9, t9, n10, t10, n11, t11, n12, t12, n13, t13, n14, t14, n15, t15, n16, t16, n17, t17, n18, t18, n19, t19, n20, t20, n21, t21, n22, t22, n23, t23, n24, t24) ADD_SINGLE_ ## itemName(n0, t0) ADD_ITEM48(itemName, n1, t1, n2, t2, n3, t3, n4, t4, n5, t5, n6, t6, n7, t7, n8, t8, n9, t9, n10, t10, n11, t11, n12, t12, n13, t13, n14, t14, n15, t15, n16, t16, n17, t17, n18, t18, n19, t19, n20, t20, n21, t21, n22, t22, n23, t23, n24, t24)
#define ADD_ITEM52(itemName, n0, t0, n1, t1, n2, t2, n3, t3, n4, t4, n5, t5, n6, t6, n7, t7, n8, t8, n9, t9, n10, t10, n11, t11, n12, t12, n13, t13, n14, t14, n15, t15, n16, t16, n17, t17, n18, t18, n19, t19, n20, t20, n21, t21, n22, t22, n23, t23, n24, t24, n25, t25) ADD_SINGLE_ ## itemName(n0, t0) ADD_ITEM50(itemName, n1, t1, n2, t2, n3, t3, n4, t4, n5, t5, n6, t6, n7, t7, n8, t8, n9, t9, n10, t10, n11, t11, n12, t12, n13, t13, n14, t14, n15, t15, n16, t16, n17, t17, n18, t18, n19, t19, n20, t20, n21, t21, n22, t22, n23, t23, n24, t24, n25, t25)
#define ADD_ITEM54(itemName, n0, t0, n1, t1, n2, t2, n3, t3, n4, t4, n5, t5, n6, t6, n7, t7, n8, t8, n9, t9, n10, t10, n11, t11, n12, t12, n13, t13, n14, t14, n15, t15, n16, t16, n17, t17, n18, t18, n19, t19, n20, t20, n21, t21, n22, t22, n23, t23, n24, t24, n25, t25, n26, t26) ADD_SINGLE_ ## itemName(n0, t0) ADD_ITEM52(itemName, n1, t1, n2, t2, n3, t3, n4, t4, n5, t5, n6, t6, n7, t7, n8, t8, n9, t9, n10, t10, n11, t11, n12, t12, n13, t13, n14, t14, n15, t15, n16, t16, n17, t17, n18, t18, n19, t19, n20, t20, n21, t21, n22, t22, n23, t23, n24, t24, n25, t25, n26, t26)
#define ADD_ITEM56(itemName, n0, t0, n1, t1, n2, t2, n3, t3, n4, t4, n5, t5, n6, t6, n7, t7, n8, t8, n9, t9, n10, t10, n11, t11, n12, t12, n13, t13, n14, t14, n15, t15, n16, t16, n17, t17, n18, t18, n19, t19, n20, t20, n21, t21, n22, t22, n23, t23, n24, t24, n25, t25, n26, t26, n27, t27) ADD_SINGLE_ ## itemName(n0, t0) ADD_ITEM54(itemName, n1, t1, n2, t2, n3, t3, n4, t4, n5, t5, n6, t6, n7, t7, n8, t8, n9, t9, n10, t10, n11, t11, n12, t12, n13, t13, n14, t14, n15, t15, n16, t16, n17, t17, n18, t18, n19, t19, n20, t20, n21, t21, n22, t22, n23, t23, n24, t24, n25, t25, n26, t26, n27, t27)
#define ADD_ITEM58(itemName, n0, t0, n1, t1, n2, t2, n3, t3, n4, t4, n5, t5, n6, t6, n7, t7, n8, t8, n9, t9, n10, t10, n11, t11, n12, t12, n13, t13, n14, t14, n15, t15, n16, t16, n17, t17, n18, t18, n19, t19, n20, t20, n21, t21, n22, t22, n23, t23, n24, t24, n25, t25, n26, t26, n27, t27, n28, t28) ADD_SINGLE_ ## itemName(n0, t0) ADD_ITEM56(itemName, n1, t1, n2, t2, n3, t3, n4, t4, n5, t5, n6, t6, n7, t7, n8, t8, n9, t9, n10, t10, n11, t11, n12, t12, n13, t13, n14, t14, n15, t15, n16, t16, n17, t17, n18, t18, n19, t19, n20, t20, n21, t21, n22, t22, n23, t23, n24, t24, n25, t25, n26, t26, n27, t27, n28, t28)
#define ADD_ITEM60(itemName, n0, t0, n1, t1, n2, t2, n3, t3, n4, t4, n5, t5, n6, t6, n7, t7, n8, t8, n9, t9, n10, t10, n11, t11, n12, t12, n13, t13, n14, t14, n15, t15, n16, t16, n17, t17, n18, t18, n19, t19, n20, t20, n21, t21, n22, t22, n23, t23, n24, t24, n25, t25, n26, t26, n27, t27, n28, t28, n29, t29) ADD_SINGLE_ ## itemName(n0, t0) ADD_ITEM58(itemName, n1, t1, n2, t2, n3, t3, n4, t4, n5, t5, n6, t6, n7, t7, n8, t8, n9, t9, n10, t10, n11, t11, n12, t12, n13, t13, n14, t14, n15, t15, n16, t16, n17, t17, n18, t18, n19, t19, n20, t20, n21, t21, n22, t22, n23, t23, n24, t24, n25, t25, n26, t26, n27, t27, n28, t28, n29, t29)
#define ADD_ITEM62(itemName, n0, t0, n1, t1, n2, t2, n3, t3, n4, t4, n5, t5, n6, t6, n7, t7, n8, t8, n9, t9, n10, t10, n11, t11, n12, t12, n13, t13, n14, t14, n15, t15, n16, t16, n17, t17, n18, t18, n19, t19, n20, t20, n21, t21, n22, t22, n23, t23, n24, t24, n25, t25, n26, t26, n27, t27, n28, t28, n29, t29, n30, t30) ADD_SINGLE_ ## itemName(n0, t0) ADD_ITEM60(itemName, n1, t1, n2, t2, n3, t3, n4, t4, n5, t5, n6, t6, n7, t7, n8, t8, n9, t9, n10, t10, n11, t11, n12, t12, n13, t13, n14, t14, n15, t15, n16, t16, n17, t17, n18, t18, n19, t19, n20, t20, n21, t21, n22, t22, n23, t23, n24, t24, n25, t25, n26, t26, n27, t27, n28, t28, n29, t29, n30, t30)
#define ADD_ITEM64(itemName, n0, t0, n1, t1, n2, t2, n3, t3, n4, t4, n5, t5, n6, t6, n7, t7, n8, t8, n9, t9, n10, t10, n11, t11, n12, t12, n13, t13, n14, t14, n15, t15, n16, t16, n17, t17, n18, t18, n19, t19, n20, t20, n21, t21, n22, t22, n23, t23, n24, t24, n25, t25, n26, t26, n27, t27, n28, t28, n29, t29, n30, t30, n31, t31) ADD_SINGLE_ ## itemName(n0, t0) ADD_ITEM62(itemName, n1, t1, n2, t2, n3, t3, n4, t4, n5, t5, n6, t6, n7, t7, n8, t8, n9, t9, n10, t10, n11, t11, n12, t12, n13, t13, n14, t14, n15, t15, n16, t16, n17, t17, n18, t18, n19, t19, n20, t20, n21, t21, n22, t22, n23, t23, n24, t24, n25, t25, n26, t26, n27, t27, n28, t28, n29, t29, n30, t30, n31, t31)

#define ADD_ITEM_HELPER2(count) ADD_ITEM##count
#define ADD_ITEM_HELPER1(count) ADD_ITEM_HELPER2(count)
#define ADD_ITEM_HELPER(count) ADD_ITEM_HELPER1(count)

#endif