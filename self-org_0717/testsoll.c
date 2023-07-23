#include "soll.h"
#include "specific.h"

#define LISTSTRLEN 1000

int main(void)
{
   char str[LISTSTRLEN];
   long cnt = 0;
   printf("Test Self-Org LL (%s) Start ... ", SOLLTYPE);

   assert(soll_free(NULL));
   assert(soll_size(NULL) == 0);
   soll_add(NULL, "1one");

   soll* s = soll_init(none);
   assert(soll_size(s) == 0);
   soll_add(s, "1one");
   assert(soll_isin(s, "1one", &cnt));
   assert(soll_freq(s, "1one")==2);
   assert(soll_size(s) == 1);
   soll_add(s, "2two");
   assert(soll_freq(s, "2two")==1);
   assert(soll_size(s) == 2);
   assert(soll_isin(s, "2two", &cnt));
   assert(soll_freq(s, "2two")==2);
   soll_add(s, "3three");
   assert(soll_size(s) == 3);
   soll_tostring(s, str);  
   assert(!strcmp("1one(2)|2two(2)|3three(1)", str));
   assert(soll_remove(s, "2two"));
   assert(soll_size(s) == 2);
   assert(soll_remove(s, "3three"));
   assert(soll_size(s) == 1);
   assert(soll_remove(s, "1one"));
   assert(soll_size(s) == 0);
   assert(soll_free(s));

   s = soll_init(mtf);
   assert(soll_size(s) == 0);
   soll_add(s, "1one");
   assert(soll_size(s) == 1);
   soll_add(s, "2two");
   assert(soll_size(s) == 2);


   assert(soll_isin(s, "1one", &cnt));
   assert(soll_isin(s, "2two", &cnt));
   soll_add(s, "3three");
   assert(soll_size(s) == 3);
   soll_tostring(s, str);
   assert(!strcmp("2two(2)|1one(2)|3three(1)", str));
   assert(soll_remove(s, "2two"));
   assert(soll_size(s) == 2);
   assert(soll_remove(s, "3three"));
   assert(soll_size(s) == 1);
   assert(soll_remove(s, "1one"));
   assert(soll_size(s) == 0);
   assert(soll_free(s));

   s = soll_init(transpose);
   assert(soll_size(s) == 0);
   soll_add(s, "1one");
   assert(soll_size(s) == 1);
   soll_add(s, "2two");
   assert(soll_size(s) == 2);
   assert(soll_isin(s, "1one", &cnt));
   assert(soll_isin(s, "2two", &cnt));

   soll_tostring(s, str);
   soll_add(s, "3three");
   soll_tostring(s, str);
   assert(soll_isin(s, "3three", &cnt));
   soll_tostring(s, str);
   assert(!strcmp("2two(2)|3three(2)|1one(2)", str));
   assert(soll_remove(s, "2two"));
   assert(soll_size(s) == 2);
   assert(soll_remove(s, "3three"));
   assert(soll_size(s) == 1);
   assert(soll_remove(s, "1one"));
   assert(soll_size(s) == 0);
   assert(soll_free(s));


   printf("End\n");



// ------------------------------- Additional Assertions included ---------------------------------------------

   //Organisation type: None
   soll* testcases = soll_init(none);
   long count = 0;
   char teststr[LISTSTRLEN];

   //Accessing node which do not exist in the list
   assert(soll_size(testcases) == 0);
   assert(!soll_isin(testcases, "1", &count));
   assert(count == 0);
   assert(soll_freq(testcases,"1") == 0);
   assert(!soll_remove(testcases, "1"));

   soll_add(testcases, "1");
   soll_add(testcases, "2");
   soll_add(testcases, "3");
   soll_add(testcases, "4");

   assert(!soll_isin(testcases,"5", &count));
   assert(soll_freq(testcases,"5") == 0);

   //Check if node it exists   
   assert(soll_isin(testcases, "4", &count));
   assert(count == 3);

   //check the pointer-chasing to reach node in the list which matches
   assert(count == 3);

   //Check the frequency of the node accessed
   assert(soll_freq(testcases,"4") == 2);
   soll_tostring(testcases, teststr);

   //Check if the list is re-arranged, no change in "none" 
   assert(strcmp(teststr, "4(2)|1(1)|2(1)|3(1)") != 0);
   assert(strcmp(teststr, "1(1)|2(1)|4(2)|3(1)") != 0);
   assert(strcmp(teststr, "1(1)|2(1)|3(1)|4(2)") == 0);

   //check the pointer chasing for node that is not in the list
   assert(!soll_isin(testcases,"5", &count));
   assert(count == 0);

   //Check if the removal works - non-edge cases
   assert(soll_remove(testcases, "3"));
   assert(soll_freq(testcases, "3") == 0);
   assert(!soll_isin(testcases, "3", &count));
   soll_tostring(testcases, teststr);
   assert(strcmp(teststr, "1(1)|2(1)|4(2)") == 0);
   assert(soll_size(testcases) == 3);

   //Check if the removal works - edge cases (removal of last node)
   soll_add(testcases, "5");
   soll_tostring(testcases, teststr);
   assert(strcmp(teststr, "1(1)|2(1)|4(2)|5(1)") == 0);

   assert(soll_remove(testcases, "5"));
   soll_tostring(testcases, teststr);
   assert(strcmp(teststr, "1(1)|2(1)|4(2)") == 0);

   //Check if the removal works - edge cases (removal of first node)
   assert(soll_remove(testcases, "1"));
   soll_tostring(testcases, teststr);
   assert(strcmp(teststr, "2(1)|4(2)") == 0);
   assert(soll_size(testcases) == 2);

   //Check if removal works if the node doesnt exist in the list
   assert(!soll_remove(testcases, "1"));
   assert(soll_size(testcases) == 2);
   soll_tostring(testcases, str);
   assert(strcmp(teststr, "2(1)|4(2)") == 0);
   assert(soll_size(testcases) == 2);


   //Check for non-string values entered
   soll_add(testcases, "");
   assert(soll_size(testcases)==2);
   soll_tostring(testcases, teststr);

   //Check if soll_free works
   assert(soll_free(testcases));




   //Organisation type: Move-to-Front
   testcases = soll_init(mtf);

   //Accessing elements which do not exist in the list
   assert(soll_size(testcases) == 0);
   assert(soll_isin(testcases, "1",&count) == 0);
   assert(soll_freq(testcases,"1") == 0);
   assert(!soll_remove(testcases, "1"));

   soll_add(testcases, "1");
   soll_add(testcases, "2");
   soll_add(testcases, "3");
   soll_add(testcases, "4");

   soll_tostring(testcases, teststr);
   assert(strcmp(teststr, "1(1)|2(1)|3(1)|4(1)") == 0);


   //Check if node exist
   assert(soll_isin(testcases, "3", &count));
   
   soll_tostring(testcases, teststr);
   //Check if node has moved to the front of list
   assert(strcmp(teststr, "3(2)|1(1)|2(1)|4(1)") == 0);
   
   //check if Node->count has increased after soll_isin is used
   assert(soll_freq(testcases, "3") == 2);
   assert(soll_isin(testcases, "3", &count));
   assert(count == 1);

   soll_tostring(testcases, teststr);
   assert(strcmp(teststr, "3(3)|1(1)|2(1)|4(1)") == 0);

   assert(soll_isin(testcases, "2", &count));
   assert(count == 2);
   assert(soll_freq(testcases, "2") == 2);

   soll_tostring(testcases, teststr);
   assert(strcmp(teststr, "2(2)|3(3)|1(1)|4(1)") == 0);

   //check if removal works, non-edge cases
   assert(soll_remove(testcases, "3"));

   soll_tostring(testcases, teststr);
   assert(strcmp(teststr, "2(2)|1(1)|4(1)") == 0);

   //check if removal works, edge cases (removal last node)
   assert(soll_remove(testcases, "2"));
   soll_tostring(testcases, teststr);
   assert(strcmp(teststr, "1(1)|4(1)") == 0);
   assert(soll_size(testcases) == 2);

   //check if removal works, edge cases (removal first node)
   assert(soll_remove(testcases, "4"));
   soll_tostring(testcases, teststr);
   assert(strcmp(teststr, "1(1)") == 0);
   assert(soll_size(testcases) == 1);

   //Check if memory is freed
   assert(soll_free(testcases));





   //Organisation type: Transpose
   testcases = soll_init(transpose);

   //Accessing elements which do not exist in the list
   assert(soll_size(testcases) == 0);
   assert(soll_isin(testcases, "1",&count) == 0);
   assert(soll_freq(testcases,"1") == 0);
   assert(!soll_remove(testcases, "1"));

   soll_add(testcases, "1");
   soll_add(testcases, "2");
   soll_add(testcases, "3");
   soll_add(testcases, "4");

   soll_tostring(testcases, teststr);
   assert(strcmp(teststr, "1(1)|2(1)|3(1)|4(1)") == 0);


   //Check if node exist
   assert(soll_isin(testcases, "3", &count));

   soll_tostring(testcases, teststr);
   //Check if node has moved to the front of list
   assert(count == 2);


   assert(strcmp(teststr, "1(1)|3(2)|2(1)|4(1)") == 0);
   //check if Node->count has increased after soll_isin is used
   assert(soll_freq(testcases, "3") == 2);
   assert(soll_isin(testcases, "3", &count));

   //Check if node has moved to the one place infront of list
   soll_tostring(testcases, teststr);
   assert(strcmp(teststr, "3(3)|1(1)|2(1)|4(1)") == 0 );
   assert(soll_freq(testcases, "3") == 3);
   assert(count == 1);


   assert(soll_isin(testcases, "2", &count));
   assert(count == 2);
   soll_tostring(testcases, teststr);
   assert(strcmp(teststr, "3(3)|2(2)|1(1)|4(1)") == 0 );
   assert(soll_freq(testcases, "2") == 2);


   //check if removal works, non-edge cases
   assert(soll_remove(testcases, "3"));

   soll_tostring(testcases, teststr);
   assert(strcmp(teststr, "2(2)|1(1)|4(1)") == 0);

   //check if removal works, edge cases (removal last node)
   assert(soll_remove(testcases, "2"));
   soll_tostring(testcases, teststr);
   assert(strcmp(teststr, "1(1)|4(1)") == 0);
   assert(soll_size(testcases) == 2);

   //check if removal works, edge cases (removal first node)
   assert(soll_remove(testcases, "4"));
   soll_tostring(testcases, teststr);
   assert(strcmp(teststr, "1(1)") == 0);
   assert(soll_size(testcases) == 1);

   //Check if memory is freed
   assert(soll_free(testcases));

   return 0;
}
