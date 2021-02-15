/*
 * bigint.c
 *
 * This file defines the functions for working with bigints.
 */
#include <string.h>

#include "bigint.h"

/* Intermediate API */
bigint _bi_neg(bigint bi);
bigint _bi_add_signed(bigint bi1, bigint bi2);
bigint _bi_mul(bigint bi1, bigint bi2);
bigint _bi_div(bigint bin, bigint bid);
int _bi_eq(bigint bi1, bigint bi2);
int _bi_lt(bigint bi1, bigint bi2);

/*
 ******************************* Public API ***********************************
 *
 * The functions defined here are the public functions exported from this
 * file. These are what users of bigint should call. These functions build a
 * usable API out of those provided in the base API. These are all declared in
 * bigint.h.
 *
 ******************************************************************************
 */

/*
 * --------------------------------------- bigint arithmetic
 */

/* Negate a bigint: returns -bi */
bigint bigint_neg(bigint bi)
{
    return _bi_neg(bi);
}

/* Add two bigints: return bi1 + bi2 */
bigint bigint_add(bigint bi1, bigint bi2)
{
    return _bi_add_signed(bi1, bi2);
}

/* Subtract two bigints: bi1 - bi2 */
bigint bigint_sub(bigint bi1, bigint bi2)
{
    return _bi_add_signed(bi1, _bi_neg(bi2));
}

/* Multiply bigint by a bigint: bi1 * bi2 */
bigint bigint_mul(bigint bi1, bigint bi2)
{
    return _bi_mul(bi1, bi2);
}

/* Divide bigints: bi1 / bi2 */
bigint bigint_div(bigint bi1, bigint bi2)
{
    return _bi_div(bi1, bi2);
}

/*
 * --------------------------------- Relations on bigints
 */

/* Equal: bi1 == bi2 ? */
int bigint_eq(bigint bi1, bigint bi2) {
    return _bi_eq(bi1, bi2);
}

/* Not equal: bi1 != bi2 ? */
int bigint_ne(bigint bi1, bigint bi2) {
    return ! _bi_eq(bi1, bi2);
}

/* Less or equal: bi1 <= bi2 ? */
int bigint_le(bigint bi1, bigint bi2) {
    return _bi_lt(bi1, bi2) || _bi_eq(bi1, bi2);
}

/* Less than: bi1 < bi2 ? */
int bigint_lt(bigint bi1, bigint bi2) {
    return _bi_lt(bi1, bi2);
}

/* Greater or equal: bi1 >= bi2 ? */
int bigint_ge(bigint bi1, bigint bi2) {
    return ! _bi_lt(bi1, bi2);
}

/* Greater than: bi1 > bi2 ? */
int bigint_gt(bigint bi1, bigint bi2) {
    return ! _bi_lt(bi1, bi2) && ! _bi_eq(bi1, bi2);
}

/*
 *******************************Intermediate API******************************
 *
 * The functions defined here are all private (not exported by bigint.h). The
 * idea is that these are the only functions that directly manipulate the
 * members of the bigint struct. These functions form a minimal API that is
 * sufficient for implementing everything else. The public API is built out of
 * these using functions that don't directly manipulate the struct members.
 * This provides a level of separation in the code meaning that if we want to
 * change the struct layout then only the functions here would need to be
 * modified: everything else should just work. Likewise we can rearrange the
 * code in the public API without needing to worry about the low-level details
 * covered here.
 *
 ******************************************************************************
 */

/* Low-level API */
bigint _bi_zero();
int _bi_isneg(bigint bi);
int _bi_abs_lt(bigint bi1, bigint bi2);
int _bi_ndigits(bigint bi);
int _bi_nth(bigint bi, unsigned char n);
bigint _bi_neg(bigint bi);
bigint _bi_sub_abs_lt(bigint bi1, bigint bi2);
bigint _bi_add_abs(bigint bi1, bigint bi2);
bigint _bi_mul_digit_abs(bigint bi, unsigned char d);
bigint _bi_shift(bigint bi, unsigned char shift);
bigint _bi_normalise(bigint bi);
bigint _bi_div_abs(bigint bin, bigint bid);

/* Return bi with the same sign as bi_cmp */
bigint _bi_copysign(bigint bi, bigint bi_cmp)
{
    if(_bi_isneg(bi) != _bi_isneg(bi_cmp)) {
        bi = _bi_neg(bi);
    }
    return bi;
}

/* Add two bigints, taking account of sign: bi1 + bi2 */
bigint _bi_add_signed(bigint bi1, bigint bi2)
{
    bigint total;

    /* Opposite signs here. */
    if(_bi_isneg(bi1) ^ _bi_isneg(bi2)) {
        /* |bi1| < |bi2| */
        if(_bi_abs_lt(bi1, bi2)) {
            total = _bi_sub_abs_lt(bi2, bi1);
            total = _bi_copysign(total, bi2);
        }
        /* |bi1| >= |bi2| */
        else {
            total = _bi_sub_abs_lt(bi1, bi2);
            total = _bi_copysign(total, bi1);
        }
    }
    else {
        /* bi1 * bi2 >= 0 */
        total = _bi_add_abs(bi1, bi2);
        total = _bi_copysign(total, bi1);
    }
    return _bi_normalise(total);
}

/* Multiply bigint by a bigint: bi1 * bi2 */
bigint _bi_mul(bigint bi1, bigint bi2)
{
    /* Long multiplication */
    bigint total = _bi_zero();
    bigint digit_product;
    for(int i=0; i<_bi_ndigits(bi2); i++) {
        digit_product = _bi_mul_digit_abs(bi1, _bi_nth(bi2, i));
        total = bigint_add(total, _bi_shift(digit_product, i));
    }

    /* Set the sign */
    if(_bi_isneg(bi1) ^ _bi_isneg(bi2)) {
        total = _bi_neg(total);
    }

    /* Fix negatives zeros */
    return _bi_normalise(total);
}

/* Divide bigint by bigint: bin / bid */
bigint _bi_div(bigint bin, bigint bid)
{
    /* Divide by zero ... */
    if(_bi_eq(bid, _bi_zero())) {
        return _bi_zero();
    }

    bigint quotient = _bi_div_abs(bin, bid);

    /* Set the sign */
    if(_bi_isneg(bin) ^ _bi_isneg(bid)) {
        quotient = _bi_neg(quotient);
    }

    return _bi_normalise(quotient);
}

/* Equal to: bi1 == bi2 ? */
int _bi_eq(bigint bi1, bigint bi2)
{
    if(bi1.s != bi2.s) {
        return 0;
    }
    else if(bi1.n != bi2.n) {
        return 0;
    }
    else{
        for(int i=0; i<bi1.n; i++) {
            if(bi1.d[i] != bi2.d[i]) {
                return 0;
            }
        }
        /* Finally return true */
        return 1;
    }
}

/* Less than: bi1 < bi2 ? */
int _bi_lt(bigint bi1, bigint bi2)
{
    /* Handle the negative cases ... */
    int result;
    if(_bi_isneg(bi1) && !_bi_isneg(bi2)) {
        result = 1;
    }
    else if(!_bi_isneg(bi1) && _bi_isneg(bi2)) {
        result = 0;
    }
    else if(_bi_isneg(bi1) && _bi_isneg(bi2)) {
        result = _bi_lt(_bi_neg(bi2), _bi_neg(bi1));
    }
    /* Now we know they're both positive... */
    else {
        result = _bi_abs_lt(bi1, bi2);
    }
    return result;
}


/*
 ************************** Low level API *************************************
 *
 * These are the functions that directly manipulate the members of the bigint
 * struct.
 *
 ******************************************************************************
 */

/* Create a zero bigint: 0 */
bigint _bi_zero()
{
    bigint zero = {0, {0}, 0};
    return zero;
}

/* Negate a bigint: -bi */
bigint _bi_neg(bigint bi)
{
    bigint neg = bi;
    if(neg.n) {
        neg.s ^= 1;
    }
    return neg;
}

/* Is bi negative */
int _bi_isneg(bigint bi)
{
    return bi.s;
}

/* number of digits */
int _bi_ndigits(bigint bi)
{
    return bi.n;
}

/* nth digit */
int _bi_nth(bigint bi, unsigned char n)
{
    return bi.d[n];
}

/* Put into canonical form - strip leading zeros, no negative zeros */
bigint _bi_normalise(bigint bi)
{
    /* Remove leading zeros */
    unsigned char n = bi.n;
    while(n > 0 && bi.d[n-1] == 0) {
        n--;
    }
    bi.n = n;

    /* No negative zeros */
    if(bi.n == 0) {
        bi.s = 0;
    }

    return bi;
}

/* Get a bigint from a decimal string representation */
bigint bigint_from_string_d(const char* str)
{
    bigint bi = {0, {0}, 0};
    /* First extract the sign */
    int index = 0;
    if(str[index] == '-') {
        bi.s = 1;
        index++;
    }
    /* Now loop through the remaining characters */
    int len = strlen(str);
    for(int i=index; str[i]!='\0'; i++) {

        /* Validate ASCII digit */
        if( ! (('0' <=  str[i]) && (str[i] <= '9')) ) {
            fprintf(stderr, "WARNING: invalid string for bigint: \"%s\"\n", str);
            bi.n = 0;
            return bi;
        }
        /* Convert to byte value and store */
        bi.d[len-1-i] = (unsigned char)(str[i] - '0');
    }
    bi.n = len - index;

    return _bi_normalise(bi);
}

/* Add two bigints having the same sign */
bigint _bi_add_abs(bigint bi1, bigint bi2)
{
    /* Allocate the number to return (take sign of bi1) */
    bigint total = bi1;

    /* Need at least as many digits as max(bi1,n, bi2.n) */
    int n = bi1.n;
    if(bi2.n > n) {
        n = bi2.n;
    }

    /* Long addition... */
    unsigned char sum_digit = 0;
    unsigned char carry = 0;
    unsigned char bd1, bd2;
    for(int i=0; i<n; i++) {
        /* Get digits (zero if beyond length) */
        bd1 = bd2 = 0;
        if(i<bi1.n) {
            bd1 = bi1.d[i];
        }
        if(i<bi2.n) {
            bd2 = bi2.d[i];
        }
        sum_digit = bd1 + bd2 + carry;
        total.d[i] = sum_digit % 10;
        carry = sum_digit / 10;
    }

    /* Handle the carry */
    if(carry) {
        /* The only place that overflow can happen with add. */
        if(n == BIGINT_MAX_DIGITS) {
            fprintf(stderr, "WARNING: overflow in add\n");
            bigint_print_d(bi1, stderr);
            fprintf(stderr, " + ");
            bigint_print_d(bi2, stderr);
            fprintf(stderr, "\n");
            total.n = 0;
            return total;
        }

        total.d[n] = carry;
        n++;
    }

    total.n = n;
    return total;
}

/* bi1 - bi2 when bi1 >= bi2 >= 0 */
bigint _bi_sub_abs_lt(bigint bi1, bigint bi2)
{
    /* Allocate the number to return */
    bigint diff = bi1;

    /* Need to work through all digits */
    int n = bi1.n;

    /* Long subtraction */
    unsigned char carry = 0;
    for(int i=0; i<n; i++) {
        if(i < bi2.n) {
            carry += bi2.d[i];
        }
        if(bi1.d[i] >= carry) {
            diff.d[i] = bi1.d[i] - carry;
            carry = 0;
        }
        else {
            diff.d[i] = (10 + bi1.d[i]) - carry;
            carry = 1;
        }
    }
    return diff;
}

/* Less than: abs(bi1) < abs(bi2) (ignores signs...) */
int _bi_abs_lt(bigint bi1, bigint bi2)
{
    /* First check the number of digits */
    if (bi1.n < bi2.n) {
        return 1;
    }
    else if (bi1.n > bi2.n) {
        return 0;
    }
    /* Now they are positive and have the same number of digits */
    /* Start with most significant digit. */
    for(int i=bi1.n-1; i>=0; i--) {
        if(bi1.d[i] < bi2.d[i]) {
            return 1; /* True */
        }
        else if(bi1.d[i] > bi2.d[i]) {
            return 0; /* False */
        }
    }
    return 0; /* All digits equal */
}


/* Print a bigint to output stream ofile. */
void bigint_print_d(bigint bi, FILE* ofile)
{
    /* Handle zero */
    if(bi.n == 0) {
        fputc('0', ofile);
    }
    else {
        /* Print the sign bit */
        if(bi.s) {
            fputc('-', ofile);
        }
        /* Loop from most to least significant digit */
        for(int i = bi.n-1; i>=0; i--) {
            /* Convert to ASCII code and write */
            char c = '0' + (unsigned char)bi.d[i];
            fputc(c, ofile);
        }
    }
}

/* Multiply bigint by a digit */
bigint _bi_mul_digit_abs(bigint bi, unsigned char d)
{
    bigint product = {0, {0}, 0};

    unsigned char mini_product;
    unsigned char carry = 0;

    /* Least to most significant digit */
    for(int i=0; i<bi.n; i++) {
        mini_product = bi.d[i] * d + carry;
        product.d[i] = mini_product % 10;
        carry = mini_product / 10;
    }

    product.n = bi.n;
    if(carry) {
        product.d[product.n] = carry;
        product.n++;
    }

    return product;
}

/* Multiply bigint by 10**n */
bigint _bi_shift(bigint bi, unsigned char shift)
{
    bigint shifted;

    for(int i=0; i<shift; i++) {
        shifted.d[i] = 0;
    }
    for(int i=0; i<bi.n; i++) {
        shifted.d[shift+i] = bi.d[i];
    }
    shifted.n = bi.n + shift;
    shifted.s = bi.s;

    /* Strip leading zeros */
    while(shifted.n && shifted.d[shifted.n-1] == 0) {
        shifted.n--;
    }
    /* Always one zero digit though... */
    if(shifted.n == 0) {
        shifted.n++;
        shifted.d[0] = 0;
        shifted.s = 0;
    }
    return shifted;
}

/* Divide two bigints, ignoring sign */
bigint _bi_div_abs(bigint bin, bigint bid)
{
    /* Zero just from number of digits */
    if(bid.n > bin.n) {
        return _bi_zero();
    }
    /* Max number of digits needed */
    unsigned char n = bin.n - bid.n;
    bigint rem = bin;

    bigint quotient = _bi_zero();
    quotient.n = n + 1;
    for(int i=n; i>=0; i--) {
        bigint bid_shift = _bi_shift(bid, i);
        quotient.d[i] = 0;

        while(!_bi_abs_lt(rem, bid_shift)) {
            rem = _bi_sub_abs_lt(rem, bid_shift);
            rem = _bi_normalise(rem);
            quotient.d[i]++;
        }
    }
    return quotient;
}
