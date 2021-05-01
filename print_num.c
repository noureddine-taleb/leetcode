#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <assert.h>

char *num_str[] = {
    "Zero ",
    "One ",
    "Two ",
    "Three ",
    "Four ",
    "Five ",
    "Six ",
    "Seven ",
    "Eight ",
    "Nine "
};

char *num_th_str[] = {
    "",
    "Ten ",
    "Twenty ",
    "Thirty ",
    "Fourty ",
    "Fifty ",
    "Sixty ",
    "Seventy ",
    "Eighty ",
    "Ninety ",
};

char *num_10_19_str[] = {
    "Ten ",
    "Eleven ",
    "Twelve ",
    "Thirteen ",
    "Fourteen ",
    "Fifteen ",
    "Sixteen ",
    "Seventeen ",
    "Eighteen ",
    "Nineteen ",
};

char *orders[] = {
    "Hundred ",
    "Thousand ",
    "Million ",
    "Billion ",
    "Trillion ",
    "Quadrillion ",
    "Quintillion ",
    "Sextillion ",
    "Septillion "
};


/**
 * struct section
 * begin 
 **/
struct section {
    int num1;
    int num2;
    int num3;

    int order;

    struct section *next;

    char *(*to_string)(struct section *self);
};

void init_section(struct section *self, int num1, int num2, int num3, int order, char *(*to_string)(struct section *self))
{
    memset(self, sizeof(*self), 0);
    self->num1 = num1;
    self->num2 = num2;
    self->num3 = num3;
    self->order = order;

    self->to_string = to_string;
}
char *section_to_string(struct section *self)
{
    if (self->num1 + self->num2 + self->num3  == 0)
        return NULL;

    char *buf = malloc(100);
    buf[0] = 0;

    if (self->num1) {
        strcat(buf , num_str[self->num1]);
        strcat(buf , orders[0]);
    }

    if (self->num2*10 + self->num3 > 10 && self->num2*10 + self->num3 < 20) {
        strcat(buf , num_10_19_str[self->num3]);
    } else {
        strcat(buf , num_th_str[self->num2]);

        if (self->num3) {
            strcat(buf , num_str[self->num3]);
        }
    }

    if (self->order > 1)
        strcat(buf , orders[self->order-1]);

    return buf;
}
/**
 * struct section
 * end 
 **/

/**
 * struct number
 * begin 
 **/
struct number {
    struct section *list; // points to the big endian

    void (*push)(struct number *self, struct section *sec);
    char *(*to_string)(struct number *self);
};

void number_push(struct number *self, struct section *sec)
{
    if (!self->list) {
        self->list = sec;
        return;
    }

    struct section *prev = self->list;
    self->list = sec;
    sec->next = prev;
}
void init_number(struct number *self, void (*push)(struct number *self, struct section *sec), char *(*to_string)(struct number *self))
{
    memset(self, sizeof(*self), 0);
    self->push = push;
    self->to_string = to_string;
}
char *number_to_string(struct number *self)
{
    struct section *cur = self->list;
    if (!cur)
        return NULL;

    char *buf = malloc(255);
    buf[0] = 0;

    while(cur) {
        assert(((long)cur & 0x7) == 0);
        char *section_string = cur->to_string(cur);
        if (section_string) {
            strcat(buf, section_string);
            free(section_string);
        }
        cur = cur->next;
    }

    buf[strlen(buf)-1] = 0;
    return buf;
}
/**
 * struct section
 * end 
 **/

struct section *popr_section(int *num)
{
    static int sorder;
    struct section *s = aligned_alloc(8, sizeof(*s));
    assert(((long)s & 0x7) == 0); // leet code bug
    int val = *num % 1000;
    *num /= 1000;

    init_section(s, val/100, (val % 100)/10, (val % 100) % 10, ++sorder, &section_to_string);

    return s;
}

char *numberToWords(int num)
{
    struct number *n = malloc(sizeof(*n));

    init_number(n, &number_push, &number_to_string);

    while (num) {
        n->push(n, popr_section(&num));
    }

    return n->to_string(n);
}


int main(int argc, char **argv)
{
    int num = atoi(argv[1]);

    printf("%d: '%s'\n", num, numberToWords(num));
    
    return 0;
}