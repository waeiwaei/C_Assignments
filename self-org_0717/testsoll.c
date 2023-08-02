#include "soll.h"
#include "specific.h"

#define LISTSTRLEN 1000
#define ZERO 0
#define ONE 1


void test();
void test_white_box();
void test_white_box_none();
void test_white_box_mtf();
void test_white_box_tr();
void test_white_box_frequency();
void test_black_box();
void test_black_box_transpose();
void test_black_box_mtf();
void test_black_box_none();
void test_black_box_frequency();
void test_unit_test();
void test_soll_init();
void test_soll_add();  
void test_soll_remove();
void test_soll_tostring();
void test_soll_freq();
void test_soll_size();
void test_soll_isin();
void test_soll_isin_mtf();
void test_soll_isin_transpose();
void test_soll_isin_frequency();
void test_mtf_reorg();
void test_transpose_reorg();
void test_frequency_reorg();


int main(void)
{

   test();

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
   soll_add(s, "3three");
   assert(soll_size(s) == 3);
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
   return 0;
}



void test(){

   test_white_box();
   test_black_box();
   test_unit_test();
}



void test_white_box(){

   test_white_box_none();
   test_white_box_mtf();
   test_black_box_transpose();
   test_white_box_frequency();

}


void test_white_box_none(){

   //Organisation type: None
   soll* testcases = soll_init(none);
   long count = ZERO;
   char teststr[LISTSTRLEN];
   
   //Checking the list should be empty
   soll_tostring(testcases, teststr);
   assert(strcmp(teststr,"") == ZERO);

   //Accessing node which do not exist in the list
   assert(soll_size(testcases) == ZERO);
   assert(!soll_isin(testcases, "1", &count));
   assert(count == ZERO);
   assert(soll_freq(testcases,"1") == ZERO);
   assert(!soll_remove(testcases, "1"));

   soll_add(testcases, "1");
   soll_add(testcases, "2");
   soll_add(testcases, "3");
   soll_add(testcases, "4");
   assert(soll_size(testcases) == 4);
   soll_tostring(testcases, teststr);
   assert(strcmp(teststr, "1(1)|2(1)|3(1)|4(1)") == ZERO);
   

   //Check for node that is not in the list - should go through entire list
   assert(!soll_isin(testcases,"5", &count));
   //
   assert(soll_freq(testcases,"5") == 0);
   //Counter for pointer chasing should itterate through entire list (4 nodes)
   assert(count == 4);


   //Check if node it exists in the list - should return true 
   //Should also increase the frequency of the searched node 
   assert(soll_isin(testcases, "4", &count));
   
   //Counter for pointer chasing should be incremented by 3, 1->2->3
   assert(count == 7);

   //Check the frequency of the node accessed
   assert(soll_freq(testcases,"4") == 2);
   soll_tostring(testcases, teststr);

   //Check if the list is re-arranged, should be no change in "none" orgtype
   assert(strcmp(teststr, "4(2)|1(1)|2(1)|3(1)") != ZERO);
   assert(strcmp(teststr, "1(1)|2(1)|4(2)|3(1)") != ZERO);
   assert(strcmp(teststr, "1(1)|2(1)|3(1)|4(2)") == ZERO);

   //check the pointer chasing for node that is not in the list
   assert(!soll_isin(testcases,"5", &count));
   assert(count == 11);

   //Check if the removal works - node middle of the list
   assert(soll_remove(testcases, "3"));
   assert(soll_freq(testcases, "3") == ZERO);
   assert(!soll_isin(testcases, "3", &count));
   soll_tostring(testcases, teststr);
   assert(strcmp(teststr, "1(1)|2(1)|4(2)") == ZERO);
   assert(soll_size(testcases) == 3);

   //Check if the removal works - edge cases (removal of last node)
   soll_add(testcases, "5");
   soll_tostring(testcases, teststr);
   assert(strcmp(teststr, "1(1)|2(1)|4(2)|5(1)") == ZERO);

   assert(soll_remove(testcases, "5"));
   soll_tostring(testcases, teststr);
   assert(strcmp(teststr, "1(1)|2(1)|4(2)") == ZERO);

   //Check if the removal works - edge cases (removal of first node)
   assert(soll_remove(testcases, "1"));
   soll_tostring(testcases, teststr);
   assert(strcmp(teststr, "2(1)|4(2)") == ZERO);
   assert(soll_size(testcases) == 2);

   //Trying to remove node from list which does not already exist
   assert(!soll_remove(testcases, "1"));
   assert(soll_size(testcases) == 2);
   
   soll_tostring(testcases, teststr);
   assert(strcmp(teststr, "2(1)|4(2)") == ZERO);
   assert(soll_size(testcases) == 2);


   //Check for empty string added to the list - should not be able to add
   soll_add(testcases, "");
   assert(soll_size(testcases) == 2);
   soll_tostring(testcases, teststr);
   assert(strcmp(teststr, "2(1)|4(2)") == ZERO);

   //adding duplicates
   soll_add(testcases, "2");
   soll_tostring(testcases, teststr);
   assert(strcmp(teststr, "2(1)|4(2)|2(1)") == ZERO);
   assert(soll_size(testcases) == 3);

   assert(soll_remove(testcases, "2"));
   assert(soll_size(testcases) == 2);
   soll_tostring(testcases, teststr);
   assert(strcmp(teststr, "4(2)|2(1)") == ZERO);

   assert(soll_remove(testcases, "2"));
   assert(soll_size(testcases) == 1);
   soll_tostring(testcases, teststr);
   assert(strcmp(teststr, "4(2)") == ZERO);

   assert(soll_remove(testcases, "4"));
   assert(soll_size(testcases) == 0);

   //Check if soll_free works
   assert(soll_free(testcases));

}



void test_white_box_mtf(){

   //Organisation type: MTF
   soll* testcases = soll_init(mtf);
   long count = ZERO;
   char teststr[LISTSTRLEN];


   //Checking the list should be empty
   soll_tostring(testcases, teststr);
   assert(strcmp(teststr,"") == ZERO);

   //Accessing elements which do not exist in the list
   assert(soll_size(testcases) == ZERO);
   assert(!soll_isin(testcases, "1",&count));
   assert(soll_freq(testcases,"1") == ZERO);
   assert(!soll_remove(testcases, "1"));

   soll_add(testcases, "1");
   soll_add(testcases, "2");
   soll_add(testcases, "3");
   soll_add(testcases, "4");
   assert(soll_size(testcases) == 4);
   soll_tostring(testcases, teststr);
   assert(strcmp(teststr, "1(1)|2(1)|3(1)|4(1)") == ZERO);


   //Check if node exist
   assert(soll_isin(testcases, "3", &count));
   assert(count == 2);
   //check if frequency count increased
   assert(soll_freq(testcases, "3") == 2);
   
   soll_tostring(testcases, teststr);
   //Check if node has moved to the front of list
   assert(strcmp(teststr, "1(1)|2(1)|3(2)|4(1)") != ZERO);
   assert(strcmp(teststr, "1(1)|3(2)|2(1)|4(1)") != ZERO);
   assert(strcmp(teststr, "3(2)|1(1)|2(1)|4(1)") == ZERO);
   
   //Frequency count of node should be incremented
   assert(soll_isin(testcases, "3", &count));
   assert(soll_freq(testcases, "3") == 3);
   
   //Pointer chasing should be the same, as there was no pointer chasing required
   assert(count == 2);

   soll_tostring(testcases, teststr);
   assert(strcmp(teststr, "3(3)|1(1)|2(1)|4(1)") == ZERO);


   assert(soll_isin(testcases, "2", &count));
   assert(count == 4);
   assert(soll_freq(testcases, "2") == 2);

   soll_tostring(testcases, teststr);
   assert(strcmp(teststr, "2(2)|3(3)|1(1)|4(1)") == ZERO);

   //check if removal works, non-edge cases - node middle of the list
   assert(soll_remove(testcases, "3"));

   soll_tostring(testcases, teststr);
   assert(strcmp(teststr, "2(2)|1(1)|4(1)") == ZERO);
   assert(soll_size(testcases) == 3);


   //check if removal works - edge cases (removal first node)
   assert(soll_remove(testcases, "2"));
   soll_tostring(testcases, teststr);
   assert(strcmp(teststr, "1(1)|4(1)") == ZERO);
   assert(soll_size(testcases) == 2);
   
   

   //check if removal works, edge cases (removal last node)
   assert(soll_remove(testcases, "4"));
   assert(soll_size(testcases) == ONE);
   
   soll_tostring(testcases, teststr);
   assert(strcmp(teststr, "1(1)") == ZERO);


   assert(soll_remove(testcases, "1"));
   assert(soll_size(testcases) == ZERO);
   
   assert(!soll_isin(testcases, "1", &count));
   assert(count == 4);
   assert(!soll_isin(testcases, "1", &count));
   assert(count == 4);

   //Adding an empty string to the list - should not be allowed
   soll_add(testcases, "");
   assert(soll_size(testcases) == ZERO);

   //adding duplicates & removing duplicates
   soll_add(testcases, "1");
   soll_add(testcases, "5");
   soll_add(testcases, "5");
   soll_tostring(testcases, teststr);
   assert(strcmp(teststr, "1(1)|5(1)|5(1)") == ZERO);
   assert(soll_size(testcases) == 3);

   assert(soll_remove(testcases, "1"));
   assert(soll_size(testcases) == 2);
   soll_tostring(testcases, teststr);
   assert(strcmp(teststr, "5(1)|5(1)") == ZERO);

   assert(soll_remove(testcases, "5"));
   assert(soll_size(testcases) == 1);
   soll_tostring(testcases, teststr);
   assert(strcmp(teststr, "5(1)") == ZERO);

   assert(soll_remove(testcases, "5"));
   assert(soll_size(testcases) == 0);


   //Check if memory is freed
   assert(soll_free(testcases));

}



void test_white_box_tr(){

   //Organisation type: Transpose
   soll* testcases = soll_init(transpose);
   long count = ZERO;
   char teststr[LISTSTRLEN];


   //Organisation type: Transpose
   soll_tostring(testcases, teststr);
   assert(strcmp(teststr,"") == ZERO);

   //Accessing elements which do not exist in the list
   assert(soll_size(testcases) == ZERO);
   assert(soll_isin(testcases, "1",&count) == ZERO);
   assert(soll_freq(testcases,"1") == ZERO);
   assert(!soll_remove(testcases, "1"));

   soll_add(testcases, "1");
   soll_add(testcases, "2");
   soll_add(testcases, "3");
   soll_add(testcases, "4");
   assert(soll_size(testcases) == 4);  
   soll_tostring(testcases, teststr);
   assert(strcmp(teststr, "1(1)|2(1)|3(1)|4(1)") == ZERO);


   //Check if node exist
   assert(soll_isin(testcases, "3", &count));
   assert(count == 2);

   //Check if node has moved one position to the front of list
   soll_tostring(testcases, teststr);
   assert(strcmp(teststr, "1(1)|3(2)|2(1)|4(1)") == ZERO);
   assert(count == 2);

   //check if Node->count has increased after soll_isin is called
   assert(soll_freq(testcases, "3") == 2);
   
   //Check if node has moved to the one place infront of list
   assert(soll_isin(testcases, "3", &count));
   soll_tostring(testcases, teststr);
   assert(strcmp(teststr, "3(3)|1(1)|2(1)|4(1)") == ZERO);
   assert(soll_freq(testcases, "3") == 3);
   assert(count == 3);

   //Check if node has moved to the one place infront of list
   assert(soll_isin(testcases, "2", &count));
   assert(count == 5);
   soll_tostring(testcases, teststr);
   assert(strcmp(teststr, "3(3)|2(2)|1(1)|4(1)") == ZERO);
   assert(soll_freq(testcases, "2") == 2);


   //check if removal works, edge cases - first node
   assert(soll_remove(testcases, "3"));
   soll_tostring(testcases, teststr);
   assert(strcmp(teststr, "2(2)|1(1)|4(1)") == ZERO);
   assert(soll_size(testcases) == 3);


   //check if removal works, non edge cases - (middle node)
   assert(soll_remove(testcases, "1"));
   soll_tostring(testcases, teststr);
   assert(strcmp(teststr, "2(2)|4(1)") == ZERO);
   assert(soll_size(testcases) == 2);


   //check if removal works, edge cases - (removal last node)
   assert(soll_remove(testcases, "4"));
   soll_tostring(testcases, teststr);
   assert(strcmp(teststr, "2(2)") == ZERO);
   assert(soll_size(testcases) == ONE);

   assert(soll_remove(testcases, "2"));
   assert(soll_size(testcases) == ZERO);

   assert(!soll_isin(testcases, "2", &count));
   assert(count == 5);

   //Adding an empty string to the list - should not be allowed
   soll_add(testcases, "");
   assert(soll_size(testcases) == ZERO);

   //adding duplicates & removing duplicates
   soll_add(testcases, "1");
   soll_add(testcases, "5");
   soll_add(testcases, "5");
   soll_tostring(testcases, teststr);
   assert(strcmp(teststr, "1(1)|5(1)|5(1)") == ZERO);
   assert(soll_size(testcases) == 3);

   assert(soll_remove(testcases, "1"));
   assert(soll_size(testcases) == 2);
   soll_tostring(testcases, teststr);
   assert(strcmp(teststr, "5(1)|5(1)") == ZERO);

   assert(soll_remove(testcases, "5"));
   assert(soll_size(testcases) == 1);
   soll_tostring(testcases, teststr);
   assert(strcmp(teststr, "5(1)") == ZERO);

   assert(soll_remove(testcases, "5"));
   assert(soll_size(testcases) == 0);


   //Check if memory is freed
   assert(soll_free(testcases));

}




void test_white_box_frequency(){

   //Organisation type: Frequency
   soll* testcases = soll_init(frequency);
   long count = ZERO;
   char teststr[LISTSTRLEN];
   
   //test for string result - empty list
   soll_tostring(testcases, teststr);
   assert(strcmp(teststr,"") == ZERO);
   

   //Accessing node which does not exist in the list
   assert(soll_size(testcases) == ZERO);
   assert(!soll_isin(testcases, "1", &count));
   assert(count == ZERO);
   assert(soll_freq(testcases,"1") == ZERO);
   assert(!soll_remove(testcases, "1"));


   soll_add(testcases, "1");
   soll_add(testcases, "2");
   soll_add(testcases, "3");
   soll_add(testcases, "4");
   assert(soll_size(testcases) == 4);
   soll_tostring(testcases, teststr);
   assert(strcmp(teststr, "1(1)|2(1)|3(1)|4(1)") == ZERO);


   assert(!soll_isin(testcases,"5", &count));
   assert(soll_freq(testcases,"5") == ZERO);
   assert(count == 4);


   //Check if node it exists   
   assert(soll_isin(testcases, "4", &count));
   assert(count == 7);

   //Check the frequency of the node accessed
   assert(soll_freq(testcases,"4") == 2);
   soll_tostring(testcases, teststr);

   //Check if the list is re-arranged, no change in "none" 
   assert(strcmp(teststr, "1(1)|2(1)|4(2)|3(1)") != ZERO);
   assert(strcmp(teststr, "1(1)|2(1)|3(1)|4(2)") != ZERO);
   assert(strcmp(teststr, "4(2)|1(1)|2(1)|3(1)") == ZERO);


   //check the pointer chasing for node that is not in the list
   assert(!soll_isin(testcases,"5", &count));
   assert(count == 11);

   //Check if the removal works - non-edge cases
   assert(soll_remove(testcases, "3"));
   assert(soll_freq(testcases, "3") == ZERO);
   assert(!soll_isin(testcases, "3", &count));
   soll_tostring(testcases, teststr);
   assert(strcmp(teststr, "4(2)|1(1)|2(1)") == ZERO);
   assert(soll_size(testcases) == 3);

   //Check if the removal works - edge cases (removal of last node)
   soll_add(testcases, "5");
   soll_tostring(testcases, teststr);
   assert(strcmp(teststr, "4(2)|1(1)|2(1)|5(1)") == ZERO);
   
   
   assert(soll_isin(testcases, "5", &count));
   assert(soll_freq(testcases, "5") == 2);
   
   soll_tostring(testcases, teststr);
   assert(strcmp(teststr,"5(2)|4(2)|1(1)|2(1)") == ZERO);
   
   assert(soll_isin(testcases, "4", &count));
   assert(soll_freq(testcases, "4") == 3);

   soll_tostring(testcases, teststr);
   assert(strcmp(teststr,"4(3)|5(2)|1(1)|2(1)") == ZERO);

   assert(soll_remove(testcases, "5"));
   soll_tostring(testcases, teststr);
   assert(strcmp(teststr, "4(3)|1(1)|2(1)") == ZERO);

   //Check if the removal works - edge cases (removal of first node)
   assert(soll_remove(testcases, "1"));
   soll_tostring(testcases, teststr);
   assert(strcmp(teststr, "4(3)|2(1)") == ZERO);
   assert(soll_size(testcases) == 2);

   //Check if removal works if the node doesnt exist in the list
   assert(!soll_remove(testcases, "1"));
   assert(soll_size(testcases) == 2);
   soll_tostring(testcases, teststr);
   assert(strcmp(teststr, "4(3)|2(1)") == ZERO);
   assert(soll_size(testcases) == 2);


   //Check for non-string values entered
   soll_add(testcases, "");
   assert(soll_size(testcases) == 2);
   soll_tostring(testcases, teststr);

   //Removing all values
   assert(soll_remove(testcases, "4"));
   assert(soll_freq(testcases, "4") == ZERO);
   
   assert(soll_size(testcases) == ONE);
   
   assert(soll_isin(testcases, "2", &count));
   assert(soll_remove(testcases, "2"));
   assert(!soll_isin(testcases, "2", &count));
   assert(soll_freq(testcases, "2") == ZERO);
   assert(soll_size(testcases) == ZERO);


   //adding duplicates & removing duplicates
   soll_add(testcases, "1");
   soll_add(testcases, "5");
   soll_add(testcases, "5");
   soll_tostring(testcases, teststr);
   assert(strcmp(teststr, "1(1)|5(1)|5(1)") == ZERO);
   assert(soll_size(testcases) == 3);

   assert(soll_remove(testcases, "1"));
   assert(soll_size(testcases) == 2);
   soll_tostring(testcases, teststr);
   assert(strcmp(teststr, "5(1)|5(1)") == ZERO);

   assert(soll_remove(testcases, "5"));
   assert(soll_size(testcases) == ONE);
   soll_tostring(testcases, teststr);
   assert(strcmp(teststr, "5(1)") == ZERO);

   assert(soll_remove(testcases, "5"));
   assert(soll_size(testcases) == ZERO);


   //Check if soll_free works
   assert(soll_free(testcases));

}



void test_black_box(){

   test_black_box_none();
   test_black_box_mtf();
   test_black_box_transpose();
   test_black_box_frequency();

}


void test_black_box_transpose(){

   soll* solltr = soll_init(transpose);
   char teststr[LISTSTRLEN];
   long solltrcount = ZERO;
   
   //test for string result - empty list
   soll_tostring(solltr, teststr);
   assert(strcmp(teststr,"") == ZERO);
   

   soll_add(solltr, "one");
   soll_add(solltr, "two");
   soll_add(solltr, "three");
   soll_add(solltr, "four");
   soll_add(solltr, "five");
   soll_add(solltr, "six");
   assert(soll_size(solltr) == 6);
   soll_tostring(solltr, teststr);
   assert(strcmp(teststr, "one(1)|two(1)|three(1)|four(1)|five(1)|six(1)") == ZERO);


   //Accessing an element from the list - transpose
   assert(soll_isin(solltr, "five", &solltrcount));
   assert(solltrcount == 4);
   soll_tostring(solltr, teststr);
   assert(strcmp(teststr, "one(1)|two(1)|three(1)|five(2)|four(1)|six(1)") == ZERO);
   assert(soll_freq(solltr, "five") == 2);


   //Accessing an element from the list - transpose
   assert(soll_isin(solltr, "five", &solltrcount));
   assert(solltrcount == 7);
   soll_tostring(solltr, teststr);
   assert(strcmp(teststr, "one(1)|two(1)|five(3)|three(1)|four(1)|six(1)") == ZERO);
   assert(soll_freq(solltr, "five") == 3);


   assert(soll_isin(solltr, "one", &solltrcount));
   assert(solltrcount == 7);
   soll_tostring(solltr, teststr);
   assert(strcmp(teststr, "one(2)|two(1)|five(3)|three(1)|four(1)|six(1)") == ZERO);
   assert(soll_freq(solltr, "one") == 2);


   assert(soll_isin(solltr, "four", &solltrcount));
   assert(solltrcount == 11);
   soll_tostring(solltr, teststr);
   assert(strcmp(teststr, "one(2)|two(1)|five(3)|four(2)|three(1)|six(1)") == ZERO);
   assert(soll_freq(solltr, "four") == 2);


   //Accessing an element which does not exist -> current implementation will traverse the list and continue to increment the value of cnt
   assert(soll_remove(solltr, "four"));
   assert(soll_size(solltr) == 5);
   soll_tostring(solltr, teststr);
   assert(strcmp(teststr, "one(2)|two(1)|five(3)|three(1)|six(1)") == ZERO);

   assert(!soll_isin(solltr, "four", &solltrcount));
   assert(solltrcount == 16);
   
   soll_add(solltr, "four");
   soll_tostring(solltr, teststr);
   assert(strcmp(teststr, "one(2)|two(1)|five(3)|three(1)|six(1)|four(1)") == ZERO);

   assert(soll_remove(solltr, "one"));
   assert(soll_size(solltr) == 5);

   //return false if the node to remove does not exist
   assert(!soll_remove(solltr, "one"));
   assert(soll_size(solltr) == 5);

   assert(soll_remove(solltr, "two"));   
   assert(soll_size(solltr) == 4);

   assert(soll_remove(solltr, "three"));   
   assert(soll_size(solltr) == 3);

   assert(soll_remove(solltr, "six"));   
   assert(soll_size(solltr) == 2);

   assert(soll_remove(solltr, "five"));   
   assert(soll_size(solltr) == ONE);

   assert(soll_remove(solltr, "four"));   
   assert(soll_size(solltr) == ZERO);

   //Adding an empty string to the list - should not be allowed
   soll_add(solltr, "");
   assert(soll_size(solltr) == ZERO);


   //adding duplicates & removing duplicates
   soll_add(solltr, "1");
   soll_add(solltr, "5");
   soll_add(solltr, "5");
   soll_tostring(solltr, teststr);
   assert(strcmp(teststr, "1(1)|5(1)|5(1)") == ZERO);
   assert(soll_size(solltr) == 3);

   assert(soll_remove(solltr, "1"));
   assert(soll_size(solltr) == 2);
   soll_tostring(solltr, teststr);
   assert(strcmp(teststr, "5(1)|5(1)") == ZERO);

   assert(soll_remove(solltr, "5"));
   assert(soll_size(solltr) == ONE);
   soll_tostring(solltr, teststr);
   assert(strcmp(teststr, "5(1)") == ZERO);

   assert(soll_remove(solltr, "5"));
   assert(soll_size(solltr) == ZERO);




   //Free memory allocated
   assert(soll_free(solltr));

}

void test_black_box_mtf(){

   soll* sollmtf = soll_init(mtf);
   char teststr[LISTSTRLEN];
   long sollmtfcount = ZERO;
   
   //test for string result - empty list
   soll_tostring(sollmtf, teststr);
   assert(strcmp(teststr,"") == ZERO);

   soll_add(sollmtf, "one");
   soll_add(sollmtf, "two");
   soll_add(sollmtf, "three");
   soll_add(sollmtf, "four");
   soll_add(sollmtf, "five");
   soll_add(sollmtf, "six");
   assert(soll_size(sollmtf) == 6);
   soll_tostring(sollmtf, teststr);
   assert(strcmp(teststr, "one(1)|two(1)|three(1)|four(1)|five(1)|six(1)") == ZERO);


   //Accessing an element from the list - mtf
   assert(soll_isin(sollmtf, "five", &sollmtfcount));
   soll_tostring(sollmtf, teststr);
   assert(strcmp(teststr, "five(2)|one(1)|two(1)|three(1)|four(1)|six(1)") == ZERO);
   assert(soll_freq(sollmtf, "five") == 2);


   //Accessing an element from the list - mtf
   assert(soll_isin(sollmtf, "five", &sollmtfcount));
   assert(sollmtfcount == 4);
   soll_tostring(sollmtf, teststr);
   assert(strcmp(teststr, "five(3)|one(1)|two(1)|three(1)|four(1)|six(1)") == ZERO);
   assert(soll_freq(sollmtf, "five") == 3);

   assert(soll_isin(sollmtf, "one", &sollmtfcount));
   assert(sollmtfcount == 5);
   soll_tostring(sollmtf, teststr);
   assert(strcmp(teststr, "one(2)|five(3)|two(1)|three(1)|four(1)|six(1)") == ZERO);
   assert(soll_freq(sollmtf, "one") == 2);


   //Accessing an element which does not exist -> current implementation will traverse the list and continue to increment the value of cnt
   assert(soll_remove(sollmtf, "four"));
   assert(soll_size(sollmtf) == 5);
   soll_tostring(sollmtf, teststr);
   assert(strcmp(teststr, "one(2)|five(3)|two(1)|three(1)|six(1)") == ZERO);

   assert(!soll_isin(sollmtf, "four", &sollmtfcount));
   assert(sollmtfcount == 10);
   
   soll_add(sollmtf, "four");
   soll_tostring(sollmtf, teststr);
   assert(strcmp(teststr, "one(2)|five(3)|two(1)|three(1)|six(1)|four(1)") == ZERO);
   assert(soll_size(sollmtf) == 6);

   assert(soll_remove(sollmtf, "one"));
   assert(soll_size(sollmtf) == 5);

   //return false if the node to remove does not exist
   assert(!soll_remove(sollmtf, "one"));
   assert(soll_size(sollmtf) == 5);

   assert(soll_remove(sollmtf, "two"));   
   assert(soll_size(sollmtf) == 4);

   assert(soll_remove(sollmtf, "three"));   
   assert(soll_size(sollmtf) == 3);

   assert(soll_remove(sollmtf, "six"));   
   assert(soll_size(sollmtf) == 2);

   assert(soll_remove(sollmtf, "five"));   
   assert(soll_size(sollmtf) == ONE);

   assert(soll_remove(sollmtf, "four"));   
   assert(soll_size(sollmtf) == ZERO);

   assert(!soll_isin(sollmtf, "one", &sollmtfcount));
   assert(sollmtfcount == 10);

   //adding duplicates & removing duplicates
   soll_add(sollmtf, "1");
   soll_add(sollmtf, "5");
   soll_add(sollmtf, "5");
   soll_tostring(sollmtf, teststr);
   assert(strcmp(teststr, "1(1)|5(1)|5(1)") == ZERO);
   assert(soll_size(sollmtf) == 3);

   assert(soll_remove(sollmtf, "1"));
   assert(soll_size(sollmtf) == 2);
   soll_tostring(sollmtf, teststr);
   assert(strcmp(teststr, "5(1)|5(1)") == ZERO);

   assert(soll_remove(sollmtf, "5"));
   assert(soll_size(sollmtf) == ONE);
   soll_tostring(sollmtf, teststr);
   assert(strcmp(teststr, "5(1)") == ZERO);

   assert(soll_remove(sollmtf, "5"));
   assert(soll_size(sollmtf) == ZERO);




   //Free memory allocated
   assert(soll_free(sollmtf));

}

void test_black_box_none(){

   soll* solln = soll_init(none);
   char teststr[LISTSTRLEN];
   long sollncount = ZERO;

   //test for string result - empty list
   soll_tostring(solln, teststr);
   assert(strcmp(teststr,"") == ZERO);


   soll_add(solln, "one");
   soll_add(solln, "two");
   soll_add(solln, "three");
   soll_add(solln, "four");
   soll_add(solln, "five");
   soll_add(solln, "six");
   assert(soll_size(solln) == 6);
   soll_tostring(solln, teststr);
   assert(strcmp(teststr, "one(1)|two(1)|three(1)|four(1)|five(1)|six(1)") == ZERO);


   //Accessing an element from the list - none
   assert(soll_isin(solln, "five", &sollncount));
   soll_tostring(solln, teststr);
   assert(strcmp(teststr, "one(1)|two(1)|three(1)|four(1)|five(2)|six(1)") == ZERO);
   assert(soll_freq(solln, "five") == 2);
   assert(sollncount == 4);

   //Accessing the same element and comparing the difference between orgtypes in retrieving the element
   //Accessing an element from the list - none
   assert(soll_isin(solln, "five", &sollncount));
   soll_tostring(solln, teststr);
   assert(strcmp(teststr, "one(1)|two(1)|three(1)|four(1)|five(3)|six(1)") == ZERO);
   assert(soll_freq(solln, "five") == 3);
   assert(sollncount == 8);

   assert(soll_isin(solln, "one", &sollncount));
   soll_tostring(solln, teststr);
   assert(strcmp(teststr, "one(2)|two(1)|three(1)|four(1)|five(3)|six(1)") == ZERO);
   assert(soll_freq(solln, "one") == 2);
   assert(sollncount == 8);


   //Accessing an element which does not exist -> current implementation will traverse the list and continue to increment the value of cnt
   assert(soll_remove(solln, "four"));
   assert(soll_size(solln) == 5);
   soll_tostring(solln, teststr);
   assert(strcmp(teststr, "one(2)|two(1)|three(1)|five(3)|six(1)") == ZERO);

   assert(!soll_isin(solln, "four", &sollncount));
   assert(sollncount == 13);

   soll_add(solln, "four");
   assert(soll_size(solln) == 6);
   soll_tostring(solln, teststr);
   assert(strcmp(teststr, "one(2)|two(1)|three(1)|five(3)|six(1)|four(1)") == ZERO);

   assert(soll_remove(solln, "one"));
   assert(soll_size(solln) == 5);

   //return false if the node to remove does not exist
   assert(!soll_remove(solln, "one"));
   assert(soll_size(solln) == 5);

   assert(soll_remove(solln, "two"));   
   assert(soll_size(solln) == 4);

   assert(soll_remove(solln, "three"));   
   assert(soll_size(solln) == 3);

   assert(soll_remove(solln, "six"));   
   assert(soll_size(solln) == 2);

   assert(soll_remove(solln, "five"));   
   assert(soll_size(solln) == ONE);
   
   assert(soll_remove(solln, "four"));   
   assert(soll_size(solln) == ZERO);

   assert(!soll_isin(solln, "one", &sollncount));
   assert(sollncount == 13);

   //adding duplicates & removing duplicates
   soll_add(solln, "1");
   soll_add(solln, "5");
   soll_add(solln, "5");
   soll_tostring(solln, teststr);
   assert(strcmp(teststr, "1(1)|5(1)|5(1)") == ZERO);
   assert(soll_size(solln) == 3);

   assert(soll_remove(solln, "1"));
   assert(soll_size(solln) == 2);
   soll_tostring(solln, teststr);
   assert(strcmp(teststr, "5(1)|5(1)") == ZERO);

   assert(soll_remove(solln, "5"));
   assert(soll_size(solln) == ONE);
   soll_tostring(solln, teststr);
   assert(strcmp(teststr, "5(1)") == ZERO);

   assert(soll_remove(solln, "5"));
   assert(soll_size(solln) == ZERO);



   //Free memory allocated
   assert(soll_free(solln));

}



void test_black_box_frequency(){

   soll* sollfreq = soll_init(frequency);
   char teststr[LISTSTRLEN];
   long sollfreqcount = ZERO;
   
   //test for string result - empty list
   soll_tostring(sollfreq, teststr);
   assert(strcmp(teststr,"") == ZERO);


   soll_add(sollfreq, "one");
   soll_add(sollfreq, "two");
   soll_add(sollfreq, "three");
   soll_add(sollfreq, "four");
   soll_add(sollfreq, "five");
   soll_add(sollfreq, "six");
   assert(soll_size(sollfreq) == 6);
   soll_tostring(sollfreq, teststr);
   assert(strcmp(teststr, "one(1)|two(1)|three(1)|four(1)|five(1)|six(1)") == ZERO);


   assert(soll_isin(sollfreq, "five", &sollfreqcount));
   soll_tostring(sollfreq, teststr);
   assert(strcmp(teststr, "five(2)|one(1)|two(1)|three(1)|four(1)|six(1)") == ZERO);
   assert(soll_freq(sollfreq, "five") == 2);

   assert(soll_isin(sollfreq, "five", &sollfreqcount));

   assert(sollfreqcount == 4);
   soll_tostring(sollfreq, teststr);
   assert(strcmp(teststr, "five(3)|one(1)|two(1)|three(1)|four(1)|six(1)") == ZERO);
   assert(soll_freq(sollfreq, "five") == 3);

   assert(soll_isin(sollfreq, "one", &sollfreqcount));
   assert(sollfreqcount == 5);
   soll_tostring(sollfreq, teststr);
   assert(strcmp(teststr, "five(3)|one(2)|two(1)|three(1)|four(1)|six(1)") == 0);
   assert(soll_freq(sollfreq, "one") == 2);


   assert(soll_isin(sollfreq, "two", &sollfreqcount));
   assert(sollfreqcount == 7);
   soll_tostring(sollfreq, teststr);
   assert(strcmp(teststr, "five(3)|one(2)|two(2)|three(1)|four(1)|six(1)") == ZERO);
   assert(soll_freq(sollfreq, "two") == 2);

   assert(soll_isin(sollfreq, "six", &sollfreqcount));
   assert(sollfreqcount == 12);
   soll_tostring(sollfreq, teststr);
   assert(strcmp(teststr, "five(3)|six(2)|one(2)|two(2)|three(1)|four(1)") == ZERO);
   assert(soll_freq(sollfreq, "six") == 2);


   //Accessing an element which does not exist -> current implementation will traverse the list and continue to increment the value of cnt
   assert(soll_remove(sollfreq, "four"));
   assert(soll_size(sollfreq) == 5);
   soll_tostring(sollfreq, teststr);
   assert(strcmp(teststr, "five(3)|six(2)|one(2)|two(2)|three(1)") == ZERO);
   assert(!soll_isin(sollfreq, "four", &sollfreqcount));
   assert(sollfreqcount == 17);
   
   soll_add(sollfreq, "four");
   soll_tostring(sollfreq, teststr);
   assert(strcmp(teststr, "five(3)|six(2)|one(2)|two(2)|three(1)|four(1)") == ZERO);

   assert(soll_remove(sollfreq, "one"));
   assert(soll_size(sollfreq) == 5);

   //return false if the node to remove does not exist
   assert(!soll_remove(sollfreq, "one"));
   assert(soll_size(sollfreq) == 5);

   assert(soll_remove(sollfreq, "two"));   
   assert(soll_size(sollfreq) == 4);

   assert(soll_remove(sollfreq, "three"));   
   assert(soll_size(sollfreq) == 3);

   assert(soll_remove(sollfreq, "six"));   
   assert(soll_size(sollfreq) == 2);

   assert(soll_remove(sollfreq, "five"));   
   assert(soll_size(sollfreq) == ONE);

   assert(soll_remove(sollfreq, "four"));   
   assert(soll_size(sollfreq) == ZERO);

   //adding duplicates & removing duplicates
   soll_add(sollfreq, "1");
   soll_add(sollfreq, "5");
   soll_add(sollfreq, "5");
   soll_tostring(sollfreq, teststr);
   assert(strcmp(teststr, "1(1)|5(1)|5(1)") == ZERO);
   assert(soll_size(sollfreq) == 3);

   assert(soll_remove(sollfreq, "1"));
   assert(soll_size(sollfreq) == 2);
   soll_tostring(sollfreq, teststr);
   assert(strcmp(teststr, "5(1)|5(1)") == ZERO);

   assert(soll_remove(sollfreq, "5"));
   assert(soll_size(sollfreq) == ONE);
   soll_tostring(sollfreq, teststr);
   assert(strcmp(teststr, "5(1)") == ZERO);

   assert(soll_remove(sollfreq, "5"));
   assert(soll_size(sollfreq) == ZERO);




   //Free memory allocated
   assert(soll_free(sollfreq));

}

void test_unit_test(){
   test_soll_init();
   test_soll_add();
   test_soll_remove();
   test_soll_tostring();
   test_soll_size();
   test_soll_freq();
   test_soll_isin();
}


// Test case for soll_init
void test_soll_init() {

   for (int method = none; method <= frequency; method++) {

      soll* s = soll_init((orgtype)method);
      assert(s != NULL);
      assert(s->head == NULL);
      assert(s->tail == NULL);
      assert(s->type == (orgtype) method);
      assert(s->count == ZERO);

      soll_free(s);

   }

}



// Test case for soll_add
void test_soll_add() {

   for (int method = none; method <= frequency; method++) {
   
      soll* s = soll_init((orgtype)method);
      long count = ZERO;
      soll_add(s, "apple");
      soll_add(s, "banana");
      soll_add(s, "");
      soll_add(s, NULL);

      assert(s->tail != NULL);
      assert(strcmp(s->tail->data, "") != ZERO);
      assert(strcmp(s->tail->data,"banana") == ZERO);
      assert(soll_size(s) == 2); // Check the size of the list
      assert(soll_isin(s, "apple", &count) == true); // Check if "apple" is in the list
      assert(soll_isin(s, "orange", &count) == false); // Check if "orange" is not in the list
      assert(soll_freq(s, "apple") == 2); // Check the frequency of "apple" (should be 2)
      assert(soll_freq(s, "banana") == ONE); // Check the frequency of "banana" (should be 1)
      assert(soll_freq(s, "orange") == ZERO); // Check the frequency of "orange" (should be 0)

      soll_free(s);

   }

   //testing deep copy of soll_add
   long testdeepcpycount = ZERO;
   soll* soll_testdeepcpy = soll_init(transpose);
   char* mystring = ncalloc(10 + ONE, sizeof(char));
   strcpy(mystring, "car");
   soll_add(soll_testdeepcpy, mystring);

   mystring[ZERO] = 'f'; // mystring -> far

   // assert it must be a deep copy
   assert(soll_isin(soll_testdeepcpy, "car", &testdeepcpycount));
   assert(!soll_isin(soll_testdeepcpy, "far", &testdeepcpycount));

   assert(soll_free(soll_testdeepcpy));
   free(mystring);

}





// Test case for soll_remove
void test_soll_remove() {

   for (int method = none; method <= frequency; method++) {
      soll* s = soll_init((orgtype)method);
      long count = ZERO;
      char teststr[LISTSTRLEN];
      soll_add(s, "apple");
      soll_add(s, "banana");
      soll_add(s, "orange");


      assert(soll_size(s) == 3); // Check the size of the list before removal

      assert(soll_isin(s, "orange", &count));
      assert(soll_remove(s, "banana") == true); // Remove "banana"
      assert(soll_size(s) == 2); // Check the size of the list after removal
      assert(!soll_isin(s, "banana", &count)); // Check if "banana" is not in the list after removal
      assert(soll_remove(s, "banana") == false); // Try to remove "orange" (not in the list)
      assert(soll_size(s) == 2); // Size should remain unchanged
      assert(soll_remove(s, "apple") == true); // Try to remove "orange" (not in the list)
      assert(soll_size(s) == ONE); // Size should remain unchanged
      assert(soll_remove(s, "orange") == true); // Try to remove "orange" (not in the list)
      assert(soll_size(s) == ZERO); // Size should remain unchanged

      //test for string result - empty list
      soll_tostring(s,teststr);
      assert(strcmp(teststr,"") == ZERO);

      assert(soll_free(s));

   }

}




// Test case for soll_tostring
void test_soll_tostring() {

   //Org Type: None
   soll* s = soll_init(none);
   long count = ZERO;
   soll_add(s, "apple");
   soll_add(s, "banana");
   soll_add(s, "orange");

   char str[LISTSTRLEN];
   soll_tostring(s, str);

   // The resulting string should be in the format: "apple(1)|banana(1)|orange(1)"
   assert(strcmp(str, "apple(1)|banana(1)|orange(1)") == 0);

   soll_isin(s, "banana", &count);

   soll_tostring(s, str);
   assert(strcmp(str, "apple(1)|banana(2)|orange(1)") == 0);

   assert(soll_free(s));




   //Org Type: MTF
   count = ZERO;
   s = soll_init(mtf);
   soll_add(s, "apple");
   soll_add(s, "banana");
   soll_add(s, "orange");

   soll_tostring(s, str);

   // The resulting string should be in the format: "apple(1)|banana(1)|orange(1)"
   assert(strcmp(str, "apple(1)|banana(1)|orange(1)") == 0);

   soll_isin(s, "banana", &count);

   soll_tostring(s, str);
   assert(strcmp(str, "banana(2)|apple(1)|orange(1)") == 0);

   assert(soll_free(s));





   //Org Type: Transpose
   count = ZERO;
   s = soll_init(transpose);
   soll_add(s, "apple");
   soll_add(s, "banana");
   soll_add(s, "orange");

   soll_tostring(s, str);

   // The resulting string should be in the format: "apple(1)|banana(1)|orange(1)"
   assert(strcmp(str, "apple(1)|banana(1)|orange(1)") == 0);

   soll_isin(s, "banana", &count);

   soll_tostring(s, str);
   assert(strcmp(str, "banana(2)|apple(1)|orange(1)") == 0);

   count = 0;

   soll_free(s);


   //Org Type: Transpose
   s = soll_init(frequency);
   soll_add(s, "apple");
   soll_add(s, "banana");
   soll_add(s, "orange");

   soll_tostring(s, str);

   // The resulting string should be in the format: "apple(1)|banana(1)|orange(1)"
   assert(strcmp(str, "apple(1)|banana(1)|orange(1)") == 0);

   soll_isin(s, "banana", &count);

   soll_tostring(s, str);
   assert(strcmp(str, "banana(2)|apple(1)|orange(1)") == 0);

   count = 0;

   assert(soll_free(s));

}


// Test case for soll_freq
void test_soll_freq() {

    // Test with each method (none, mtf, and transpose)
    for (int method = none; method <= frequency; method++) {
      soll* s = soll_init((orgtype)method);
      soll_add(s, "apple");
      soll_add(s, "banana");
      soll_add(s, "apple");
      soll_add(s, "orange");

      // Frequency of elements in the list: "apple" -> 2, "banana" -> 1, "orange" -> 1
      assert(soll_freq(s, "apple") == ONE);
      assert(soll_freq(s, "banana") == ONE);
      assert(soll_freq(s, "orange") == ONE);
      assert(soll_freq(s, "grape") == ZERO); // Test with an element not in the list

      assert(soll_free(s));

    }
}



// Test case for soll_size
void test_soll_size() {

    // Test with each method (none, mtf, and transpose)
    for (int method = none; method <= frequency; method++) {
      soll* s = soll_init((orgtype)method);
      assert(soll_size(s) == ZERO); // Empty list, size should be 0

      soll_add(s, "apple");
      soll_add(s, "banana");
      soll_add(s, "orange");

      assert(soll_size(s) == 3); // List contains 3 elements

      soll_remove(s, "apple");
      assert(soll_size(s) == 2); // After removing "apple", size should be 2

      soll_remove(s, "orange");
      assert(soll_size(s) == ONE); // After removing "orange", size should be 1

      soll_remove(s, "banana");
      assert(soll_size(s) == ZERO); // After removing "banana", size should be 0

      assert(soll_free(s));

    }
}


void test_soll_isin(){

   test_soll_isin_mtf();
   test_soll_isin_transpose();
   test_soll_isin_frequency();

   test_mtf_reorg();
   test_transpose_reorg();
   test_frequency_reorg();

}


void test_soll_isin_mtf() {

   soll* s = soll_init(mtf);
   long cnt = ZERO;
   char teststr[LISTSTRLEN];

   soll_add(s, "apple");
   soll_add(s, "banana");
   soll_add(s, "orange");
   soll_add(s, "pineapple");
   soll_add(s, "strawberry");
   soll_add(s, "peach");
   soll_add(s, "avocado");

   soll_tostring(s, teststr);
   assert(strcmp(teststr, "apple(1)|banana(1)|orange(1)|pineapple(1)|strawberry(1)|peach(1)|avocado(1)") == ZERO);


   // Test element presence
   assert(soll_isin(s, "banana", &cnt));
   assert(cnt == 1); // The element was found in the list
   assert(strcmp(s->head->data,"banana") == ZERO);

   soll_tostring(s, teststr);
   assert(strcmp(teststr, "banana(2)|apple(1)|orange(1)|pineapple(1)|strawberry(1)|peach(1)|avocado(1)") == ZERO);


   // Test element absence
   assert(!soll_isin(s, "grape", &cnt));
   assert(cnt == 8); // The cnt variable should be incremented since the element is not found
   assert(strcmp(s->head->data,"banana") == ZERO); // head of the list should still be "banana"


   soll_tostring(s, teststr);
   assert(strcmp(teststr, "banana(2)|apple(1)|orange(1)|pineapple(1)|strawberry(1)|peach(1)|avocado(1)") == ZERO);


   // Test element presence (after accessing an element)
   assert(soll_isin(s, "peach", &cnt));
   assert(cnt == 13); // The element was found in the list
   assert(strcmp(s->head->data,"peach") == ZERO); // head of the list should still be "banana"

   soll_tostring(s, teststr);
   assert(strcmp(teststr, "peach(2)|banana(2)|apple(1)|orange(1)|pineapple(1)|strawberry(1)|avocado(1)") == ZERO);



   // Test reorganization (move to front)
   assert(soll_isin(s, "pineapple", &cnt));
   assert(cnt == 17); // The element was found in the list, and mtf reorganization should occur
   assert(strcmp(s->head->data,"pineapple") == ZERO); 

   soll_tostring(s, teststr);
   assert(strcmp(teststr, "pineapple(2)|peach(2)|banana(2)|apple(1)|orange(1)|strawberry(1)|avocado(1)") == ZERO);


   soll_free(s);
}



void test_soll_isin_transpose() {

   soll* s = soll_init(transpose);
   long cnt = ZERO;
   char teststr[LISTSTRLEN];

   soll_add(s, "apple");
   soll_add(s, "banana");
   soll_add(s, "orange");
   soll_add(s, "pineapple");
   soll_add(s, "strawberry");
   soll_add(s, "peach");
   soll_add(s, "avocado");

   soll_tostring(s, teststr);
   assert(strcmp(teststr, "apple(1)|banana(1)|orange(1)|pineapple(1)|strawberry(1)|peach(1)|avocado(1)") == ZERO);


   // Test element presence
   assert(soll_isin(s, "banana", &cnt));
   assert(cnt == ONE); 
   assert(strcmp(s->head->data,"banana") == ZERO); // head of the list should be "banana"


   soll_tostring(s, teststr);
   assert(strcmp(teststr, "banana(2)|apple(1)|orange(1)|pineapple(1)|strawberry(1)|peach(1)|avocado(1)") == ZERO);



   // Test element presence (after accessing an element)
   assert(strcmp(s->head->next->next->next->next->data, "strawberry") == ZERO);   //Checking that "strawberry" is 5th Node in list
   assert(soll_isin(s, "strawberry", &cnt));
   assert(cnt == 5); // The element was found in the list
   assert(strcmp(s->head->next->next->next->data, "strawberry") == ZERO);  //Checking that "strawberry" is 4th Node in list, after calling soll_isin()

   // Test element presence (after accessing an last element)
   assert(strcmp(s->tail->data, "avocado") == ZERO);   
   assert(soll_isin(s, "avocado", &cnt));
   assert(cnt == 11); // The element was found in the list
   assert(strcmp(s->head->next->next->next->next->next->data, "avocado") == ZERO);  
   assert(strcmp(s->tail->data, "peach") == ZERO);   

   soll_tostring(s, teststr);
   assert(strcmp(teststr, "banana(2)|apple(1)|orange(1)|strawberry(2)|pineapple(1)|avocado(2)|peach(1)") == ZERO);

   //Test element presence (middle element in the list)
   assert(strcmp(s->head->next->next->next->next->data, "pineapple") == ZERO);
   assert(soll_isin(s, "pineapple", &cnt));
   assert(strcmp(s->head->next->next->next->data, "pineapple") == ZERO);


   soll_tostring(s, teststr);
   assert(strcmp(teststr, "banana(2)|apple(1)|orange(1)|pineapple(2)|strawberry(2)|avocado(2)|peach(1)") == ZERO);

   soll_free(s);
}



void test_soll_isin_frequency() {

   soll* s = soll_init(frequency);
   long cnt = ZERO;

   char teststr[LISTSTRLEN];

   soll_add(s, "apple");
   soll_add(s, "banana");
   soll_add(s, "orange");
   soll_add(s, "pineapple");
   soll_add(s, "strawberry");
   soll_add(s, "peach");
   soll_add(s, "avocado");

   soll_tostring(s, teststr);
   assert(strcmp(teststr, "apple(1)|banana(1)|orange(1)|pineapple(1)|strawberry(1)|peach(1)|avocado(1)") == ZERO);

   // Test element presence
   assert(soll_isin(s, "orange", &cnt));
   assert(cnt == 2); 
   assert(strcmp(s->head->data,"orange") == ZERO); 
   soll_tostring(s, teststr);
   assert(strcmp(teststr, "orange(2)|apple(1)|banana(1)|pineapple(1)|strawberry(1)|peach(1)|avocado(1)") == ZERO);


   // Test element presence (after accessing an element)
   assert(strcmp(s->head->next->next->next->next->data, "strawberry") == ZERO);   
   assert(soll_isin(s, "strawberry", &cnt));
   assert(soll_freq(s, "strawberry") == 2);
   assert(cnt == 6); 

   soll_tostring(s, teststr);
   assert(strcmp(teststr, "strawberry(2)|orange(2)|apple(1)|banana(1)|pineapple(1)|peach(1)|avocado(1)") == ZERO);
   assert(strcmp(s->head->data, "strawberry") == ZERO);  


   // Test element presence (after accessing an last element)
   assert(strcmp(s->tail->data, "avocado") == ZERO);   
   assert(soll_isin(s, "avocado", &cnt));
   assert(soll_freq(s, "avocado") == 2);
   assert(strcmp(s->tail->data, "peach") == ZERO);
   assert(strcmp(s->head->data, "avocado") == ZERO);  
   soll_tostring(s, teststr);

   assert(strcmp(teststr, "avocado(2)|strawberry(2)|orange(2)|apple(1)|banana(1)|pineapple(1)|peach(1)") == ZERO);

   assert(cnt == 12); 
   assert(soll_isin(s, "strawberry", &cnt));
   assert(soll_freq(s, "strawberry") == 3);
   assert(cnt == 13); 
   soll_tostring(s, teststr);
   assert(strcmp(teststr, "strawberry(3)|avocado(2)|orange(2)|apple(1)|banana(1)|pineapple(1)|peach(1)") == ZERO);

   soll_free(s);
}



void test_mtf_reorg() {

   soll* s = soll_init(mtf);

   soll_add(s, "apple");
   soll_add(s, "banana");
   soll_add(s, "orange");


   // Move "banana" to the front
   Node* current = s->head->next->next;
   Node* previous = s->head->next;
   mtf_reorg(s, current, previous);


   // Verify that "orange" is now at the front
   assert(strcmp(s->head->data, "orange") == ZERO);
   assert(strcmp(s->head->next->data, "apple") == ZERO);
   assert(strcmp(s->head->next->next->data, "banana") == ZERO);


   assert(soll_free(s));

}



void test_transpose_reorg() {
   soll* s = soll_init(transpose);

   soll_add(s, "apple");
   soll_add(s, "banana");
   soll_add(s, "orange");

   // Move "banana" one position closer to the front
   Node* current = s->head->next->next;
   Node* previous = s->head->next;
   transpose_reorg(s, current, previous);

   // Verify that "banana" is now at the front, and the order is updated
   assert(strcmp(s->head->data, "apple") == ZERO);
   assert(strcmp(s->head->next->data, "orange") == ZERO);
   assert(strcmp(s->head->next->next->data, "banana") == ZERO);

   soll_free(s);

   
}



void test_frequency_reorg() {
   soll* s = soll_init(frequency);

   soll_add(s, "apple");
   soll_add(s, "banana");
   soll_add(s, "orange");
   soll_add(s, "papaya");

   // Access "banana" and increase its count to 2
   Node* current = s->head->next->next;
   Node* previous = s->head->next;
   current->frequency = 2;
   frequency_reorg(s, current, previous);

   // Verify that "banana" is moved before "apple" based on frequency
   assert(strcmp(s->head->data, "orange") == ZERO);
   assert(strcmp(s->head->next->data, "apple") == ZERO);
   assert(strcmp(s->head->next->next->data, "banana") == ZERO);
   assert(strcmp(s->head->next->next->next->data, "papaya") == ZERO);


   soll_free(s);

}




