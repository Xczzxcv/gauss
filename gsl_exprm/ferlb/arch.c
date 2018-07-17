/* Apr 2015. demo file of using the fermat archive.
   If you wish, see the several fer*.h files for the definitions of various types and variables.  Or, simply use as a black box.  */

#include <stddef.h>
#include <sys/types.h>
#include <limits.h>
#include <setjmp.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "p2cLibraries/Builtins.h"
#include "p2cLibraries/PSets.h"
#include <signal.h>

#include <time.h>
#include <stdint.h>

#include "ferm120.h"
#include "fer320.h"
#include "fer220.h"
#include "fer221.h"
#include "fer222a.h"
#include "stash.h"

int main()

{
  uint32_t timer;
  int32_t j;
  quoly q1;
  quoly q2;
  quoly q3;
  quoly nf;
  quolyptr qp;
  nameptr flag;
  Boolean quit;

  tokentipe tok;
  int32_t dummy;   // keeps track of how many characters displayed per line.  of no concern to the user.
  polypt pt;

  adjust_polyname_ptrs();
  fermat_init();    // very important to do before any use of Fermat.
  ustrcpy(tok.name, ""); // set tok.name to empty string.
  tok.address = -1;

  number_polys = 0;

  // attach two poly vars, first y, then x.  The names 'x' and 'y' have to be stored in two places.
  tok.name[0] = 'y';
  newpt(&pt);
  pt->depth = 2;
  pt->next = NULL;
  polynames = pt;   // polynames is defined in fer120.h
  pt->let = tok;
  ustrcpy(pt->let.name, "y");
  ustrcpy(polyvar[1], "y");   // array of polynomial variable names.
  number_polys = 1;

  tok.name[0] = 'x';
  newpt(&pt);
  pt->next = polynames;
  pt->depth = 1;
  polynames = pt;
  pt->let = tok;
  ustrcpy(pt->let.name, "x");
  ustrcpy(polyvar[0], "x");   // polyvar must be sorted alphabetically.  'x' < 'y'
  number_polys = 2;

  // This procedure displays the system syatus.
  global_dump();
  fprintf(stdout, "\n");

  q1 = NULL;    // initialize the quolies; they are pointers.
  q2 = NULL;
  q3 = NULL;

  timer = clock();    // clock reports millionths of a second.
  fprintf(stdout, "time: %li\n", timer);

  base_quoly(&q1, 1);     // q1 = x.  1 means top level.
  base_quoly(&q2, 2);     // q2 = y.  2 means second level.
  quoly_set(1, &q3);      // q3 = 1
  best_quoly_add(&q1, q2);   // q1 is x + y;     q2 is undefined, its storage has been reallocated.
  best_quoly_add(&q1, q3);   // q1 is x + y + 1; q3 is undefined, its storage has been reallocated.

  q2 = NULL;    // q2 was left a dangling pointer.
  quoly_mult(q1, q1, &q2);
  q3 = NULL;    // q3 was left a dangling pointer.
  quoly_set(1, &q3);   // q3 = 1
  better_quoly_add(q3, q2, &q3);     // q3 is q1^2 + 1;  q2 is unchanged.  note better vs. best

  fprintf(stdout, "q1:\n");
  dummy = 0;
  quoly_display(&Output, true, true, true, q1, &dummy, &semicolon, polynames, false, true, false);
  dummy = 0;
  fprintf(stdout, "q2:\n");
  quoly_display(&Output, true, true, true, q2, &dummy, &semicolon, polynames, false, true, false);

  quoly_mult(q2, q2, &q2);
  quoly_mult(q2, q2, &q2);

  quoly_quot(q2, q3, &q2, false);  // true would tell it that we know there is no common gcd -- which is true, but sort of cheating.
                                   // Of coure in a real application, you might indeed know that.
  dummy = 0;
  fprintf(stdout, "q2 now:\n");
  quoly_display(&Output, true, true, true, q2, &dummy, &semicolon, polynames, false, true, false);

  quoly_mult(q2, q2, &q2);
  dummy = 0;
  fprintf(stdout, "\n");
  quoly_display(&Output, true, true, true, q2, &dummy, &semicolon, polynames, false, true, false);

  // just as a demo, keep squaring q2.
  for (j = 1; j <= 3; ++j) {
    quoly_mult(q2, q2, &q2);
    fprintf(stdout, "in q2 loop %li, %li\n", j, clock());
  }

  fprintf(stdout, "final time. number of terms of q2, clock, elapsed: %li, %li, %li\n", numterms(q2), clock(), clock() - timer);

  fprintf(stdout, "\n");
  fprintf(stdout, "about to read file rstuff, %li\n",  clock());
  fprintf(stdout, "\n");

  // read stuff from a file called 'rstuff'. quit does not have to be initialized.
  // rstuff contains Fermat language commands.  The procedure 'load' here calls the Fermat interpreter.
  ustrcpy(input_file_name,  "rstuff");
  inpuf = fopen((char *)input_file_name, "r");
  nf = NULL;
  flag = NULL;
  load(NULL, &nf, &flag, input_file_name, &quit);

  // This is how to gain access to variables created in the data file (rstuff).
  // The name 'vx' must be known here.

  ustrcpy(tok.name,  "vx");
  nf = NULL;
  retrieve(&tok, &qp, &nf, NULL, NULL);   // qp points to vx, or is an 'alias' of vx.

  better_quoly_sub(q3, *qp, &q3);   // analagous to adding.

  fprintf(stdout, "\n");
  dummy = 0;
  quoly_display(&Output, true, true, true, *qp, &dummy, &semicolon, polynames, false, true, false);
  fprintf(stdout, "\n");
  dummy = 0;
  quoly_display(&Output, true, true, true, q3, &dummy, &semicolon, polynames, false, true, false);


  return 0;

}

