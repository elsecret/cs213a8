#include <stdlib.h>
#include <stdio.h>
#include "list.h"

/*
	Function for Step 2

	out is a pointer to an int pointer
	in is a char* (one of the argv strings actually)
*/
void string_to_number(element_t* out, element_t in) {
	/*
		The function signature explained:

		In general:

			[out]
			a pointer to an element_t
			element_t is itself a pointer to data

			[in]
			a pointer that directly points to data


		For this specific function:

		We are returning an int, so:

			[out]
			a pointer to a pointer to an int.
			int** -> int* -> int

				Let's say we already have a malloc'ed int ready to go, call its address int* x
				To make this result persist, we set the value of out, to that address
				One way to do this would be *out = malloc(sizeof(int));
				Note that *out refers to the int* in the above diagram
				Now, to write an actual value onto this (and of course have it persist), say
				the value is 5. Do this:
				**out = 5;

			[in]
			a pointer that directly points to a string
			i.e. it is of type char* (we have to cast it)
	*/

	/*
		[The strtol function]:

		outputs an int no matter what. If the string wasn't a number,
		then it returns 0. This is how we check to see if the string is a number

		Its 3 inputs:
			1. char*, the string
			2. just pass in NULL (ignore it)
			3. the "radix" -- radix = base (e.g. base 2, base 16, base 10). For example if we have "11", we want
				it to be interpreted in base 10, so that the int we get is 11
				if we set the radix to 2, we'd get 11 interpreted as 3.
				if we set the radix to 16, we'dd get 11 interpreted as 17.
		The return:
			It returns a long int but there are literally no words that are long enough for this to matter,
			so it's fine to just cast it to an int.
			Note: it returns 0 if the string is not a number. We need to check for this.
	*/

	/*
		Notes about out:
		out CANNOT be NULL. It is passed by reference. It is a memory address where our function must place
		out result. And out result must itself be a pointer, since element_t is a pointer.
		If out is NULL, then our function simply cannot do its job. There will be no way to pass the
		result back to the caller (and we cannot use [return] because the signature must return void).

		However, what is AT the memory address that out points to (i.e. *out) could be NULL.
		If it's NULL, we must malloc the appropriate data type and overwrite the NULL with its pointer.
		In this specific case, it will be an int pointer since we're returning an int.

		On the other hand, if *out is not NULL, we can assume memory has been appropriately allocated for the
		int, so we just go to the pointer and overwrite its value with the correct one without needing malloc.
	*/
	if (*out == NULL) {
		*out = malloc(sizeof(int));
	}

	/*
		Implementation
	*/

	// Step 1: call the strtol function with radix = 10
	// record its return value.
	int num = strtol(in, NULL, 10);

	// Step 2: check for [num == 0]
	// if it's 0 then set it to -1 as per assignment instruction
	if (num == 0) {
		num = -1;
	}

	// Step 3: [*out] is the int pointer, so we save [num] as the value it points to
	// Basically **out = num, but we have to cast it.
	// Reason: can't dereference a [void*], and [*out] is a [void*]
	// In test.c, the functions declare a local variable with the appropraite type
	// before the malloc but the order doesn't matter.
	// There are 2 equivalent ways to do this.
	// You can cast out to an int**
	// In that case, it now knows that *out is an int*
	// Thus, **out is the final value.
	// Alternatively, you can cast *out to an int*
	// It's the same thing.

	// Version 1a.
	int* int_pointer = (int*)*out;
	*int_pointer = num;

	// Version 1b:
	*(int*)*out = num;

	// Version 2a:
	int** int_pointer_pointer = (int**)out;
	**int_pointer_pointer = num;

	// Version 2b:
	**(int**)out = num;
	// All 4 of these are equivalent and do the same thing.

	// However, the following syntax is illegal:
	// (int) **out == num;
	// The reason for this is *out cannot be deferenced because it's a void*, so **out cannot be reached.
	// Hence, it cannot be cast to an int.
	// **out == num is illegal for the same reason.

	/*
		Alternative Implementation (this is the entire function other than malloc)
	*/
	*(int*)*out = strtol(in, NULL, 10) == 0 ? -1 : strtol(in, NULL, 10);
}

/*
	Function for Step 3

	[num_in] is a pointer to an int;
	[str_in] is a pointer to a char -- in this sense, it's just a string
	[out] is the address where we must save the pointer to a char
	([out] is passed by reference)
*/
void string_to_string(element_t* out, element_t num_in, element_t str_in) {
	/*
		[out]
		Unlike in int_to_string, we are going to put the address of a char as the value of out
		*out = some char*
		[num_in] is a pointer, and here we expect it to be an int pointer.
		If that value it points to is -1, then we set *out to str_in.
		If that value it points to is not, then we set *out to...?

		We are told to set it a "NULL value".
		We must treat it like string_to_number:
		The end result should be this:

		[out] points to a char*
		that char* must point to some byte in memory with the value of 0x00 i.e. NULL

		In memory, physically, as in, literally:
		out is a parameter, so it was placed on the stack by the caller prologue.
		Let's say the stack bottom is at 0x1000, and the exact address of out is 0x1020
		What is stored there, is a pointer, a memory address. This pointer is itself a pointer.
		So, on the stack, at address 0x1020, is the value of a pointer.
		This pointer could be on the stack (at a more positive number), in the caller's
		activation frame, if it was a local variable, or it could be on the heap, i.e.
		already a part of some list's ->data field.
		Let's say it's a local variable. So it would be slightly lower on the stack.
		Let's say it's at 0x1040.
		So [out] has address 0x1020 and value 0x1040.
		At address 0x1040 is a pointer.
		At address 0x1040, the value there i.e. the bits that are physically there,
		is the address of where the actual information is.

		Brief recap:
		&out is 0x1020	This is the address of the [out] argument, passed by reference
		out is 0x1040	This is the value there.
		&*out is 0x1040	This is also the value there. &* cancel each other out.
		*out is ????	This is the value at the pointer stored at out.

		Our job is to go to 0x1040 and put there the POINTER to either the start of a string or a null character.
		In other case it is of type char*
	*/

	if (*(int*)num_in == -1) {
		*out = str_in;
		// Don't need to malloc, we are going to just point to the original arg from the
		// user input because there's no need to make an extra copy of the string
		// This literally sets it to the address of the first character of str_in
		// When this character is read as the start of a string, it will keep going until it reaches a NULL
		// character ('\0')
	}
	else {
		// We DO need to malloc if we want it to be a string with first character NULL or '\0' (same thing).
		// (if it's NULL)
		/*if (*out == NULL){
			*out = malloc(sizeof(char));
		}
		*(char*) *out = '\0';*/

		// Just kidding.
		// Avoid a malloc and sneak past Valgrind with this simple trick:
		char* str_end = str_in;
		while (*str_end != '\0') {
			str_end++;
		}
		*out = str_end;
		// We got the pointer to str_in's NULL termination character, then saved it to *out.

		// The main thing is... do not do this:
		// *out = NULL;

		// Why?
		// If we do this and ask element_t for its string, it will say: my string is at address 0 JeBaited
		// Segfault.
	}

}

/*
	Function for Step 4
*/
int not_negative(element_t num) {
	return (*(int*)num >= 0);
}

/*
	Function for Step 5
*/
int not_null_string(element_t str) {
	return (*(char*)str != '\0');
	// Return whether the first character is not NULL.
}

/*
	Function for Step 6
*/
void truncate(element_t* out, element_t num_in, element_t str_in) {
	int len = *(int*)num_in;

	// Malloc the necessary space for the substring.
	*out = malloc(sizeof(char) * (len + 1));

	char* str_out = *out;
	char* str_to_truncate = str_in;

	// Now everything is familiar and easy to work with with array syntax

	for (int i = 0; i < len; i++) {
		str_out[i] = str_to_truncate[i];
	}
	str_out[len] = '\0'; // Terminate the string with a NULL
}

/*
	Function for Step 7
*/
void print_string(element_t str) {
	printf((char*)str);
	printf("\n");
}

/*
	Function for Step 8 (calculating size for string);

	The final int includes the spaces and the null termination
*/
void accumulate_size(element_t* out, element_t ignore, element_t num_in) {
	*(int*)out += (*(int*)num_in) + 1;
}

/*
	Function for Step 8 (concatenating the string)
*/
void concatenate_string(element_t* out, element_t ignore, element_t in) {
	char* out_string = *out;
	char* in_string = in;

	// Find the start index to copy characters to
	int out_index = 0;
	while (out_string[out_index] != '\0') {
		out_index++;
	}

	// copy the string
	int in_index = 0;
	while (in_string[in_index] != '\0') {
		out_string[out_index] = in_string[in_index];
		out_index++;
		in_index++;
	}
	// add white space and '\0'
	out_string[out_index] = ' ';
	out_string[out_index + 1] = '\0';
}

/*
	Function for Step 9 (finding the max):
*/
void get_max(element_t* out, element_t ignore, element_t num_in) {
	if (*(int*)num_in > *(int*)out) {
		*(int*)out = *(int*)num_in;
	}
}

int main(int argc, char* argv[]) {
	// Step 1

	// Create list holding all arg strings
	struct list * arg_list = list_create();
	// Each element is a pointer to the start of an arg string
	for (size_t i = 1; i < argc; i++)
	{
		list_append(arg_list, argv[i]);
	}

	// Dev initialization of arg_list:
	//char* inputs[] = { "5", "Patriots", "4", "2", "LeBron", "Mayweather" };
	//for (int i = 0; i < 6; i++) {
	//	list_append(arg_list, inputs[i]);
	//}

	// Step 2: produce a list of numbers from initial_list

	// Create list for numbers with -1
	struct list * initial_number_list = list_create();
	list_map1(string_to_number, initial_number_list, arg_list);

	// Step 3: produce a list of strings from initial_number_list and arg_list

	// Create list for the strings with NULLs
	struct list * initial_string_list = list_create();
	list_map2(string_to_string, initial_string_list, initial_number_list, arg_list);

	// Step 4: Filter the number list to remove -1s
	struct list * filtered_number_list = list_create();
	list_filter(not_negative, filtered_number_list, initial_number_list);

	// Step 5: Filter the string list to remove '\0's
	struct list * filtered_string_list = list_create();
	list_filter(not_null_string, filtered_string_list, initial_string_list);

	// Step 6: Truncate the strings based on the numbers
	struct list * truncated_string_list = list_create();
	list_map2(truncate, truncated_string_list, filtered_number_list, filtered_string_list);

	// Step 7: Print the truncated string list one per line
	list_foreach(print_string, truncated_string_list);

	// Step 8: Join this list to a single array
	// Get the final size of the string (just put it on the stack to avoid malloc)
	// Strategy: get a list 
	int size = 0;
	list_foldl(accumulate_size, (void*) &size, filtered_number_list);

	char* string = malloc(sizeof(char) * size + 1);

	// Set the initial char to NULL so concatenate_string knows where to start
	string[0] = '\0';

	list_foldl(concatenate_string, (void*) &string, truncated_string_list);

	// Currently, the string is not null terminated, and string[size] = ' '
	// So, just change it to '\0'
	string[size] = '\0';
	// Note that string ends with 2 NULLs but it doesn't matter because free
	// doesn't care; no memory issues.

	// Print it as per instruction
	print_string(string);

	// Step 9: Compute the maximum value in the numbers list and print it
	int max = 0;
	list_foldl(get_max, (void*) &max, filtered_number_list);
	printf("%d\n", max);

	// Step 10: Free things
	free(string);

	list_foreach(free, truncated_string_list);
	list_foreach(free, initial_number_list);
	// Don't need to free arg_list (compiler does it)
	// Don't need to free initial_string_list (since it points to arg_list elements)
	// Don't need to free filtered_number_list (since it points to initial_number_list elements)
	// Don't need to free filtered_string_list (since it points to initial_string_list elements)
	list_destroy(arg_list);
	list_destroy(initial_number_list);
	list_destroy(initial_string_list);
	list_destroy(filtered_number_list);
	list_destroy(filtered_string_list);
	list_destroy(truncated_string_list);
}
