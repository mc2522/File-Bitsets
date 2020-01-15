/*
 * File: file-bitsets.c
 * @author: Mike Cao
 * @date: 4/17/19
 */

#include <stdio.h>    // size_t type
#include <stdlib.h>   
#include <string.h>   // C strings
#include <stdint.h>   // uint64_t type 
#define MAX_BIT_POSITION 63
#define MAX_STRING_LENGTH 256

// global string containing all of the valid characters to encode 
const char chars[MAX_BIT_POSITION + 2] = "ZYXWVUTSRQPONMLKJIHGFEDCBA,zyxwvutsrqponmlkjihgfedcba.9876543210"; 

/*
 * set_encode encodes a string
 * @param st string to encode
 * returns the resultant set which is of type uint64_t
 */
uint64_t set_encode(char * st) {
	// value to be returned
        uint64_t encoded_num = 0;
	// representation of 8 bits
        uint64_t shift = 1;
	// loop through and find valid characters
        for(size_t i = 0; i < strlen(st); i++) {
                for(size_t j = 0; j <= strlen(chars); j++) {
                        if(st[i] == chars[j]) {	
				// if matching, set the bit at the corresponding				// position
		        	encoded_num |= (shift << j);
				break;
			}
                }
        }
        return encoded_num;
}

/*
 * file_set_encode reads the file and encodes the contents
 * @param fp the file to read
 * returns the resultant set which is of type uint64_t
 */
uint64_t file_set_encode(FILE * fp) {
	// to be the buffer
	char * processed = (char*)malloc(sizeof(char) * MAX_STRING_LENGTH);
	// to be returned
	uint64_t encoded = 0;
	// read the file
	fread(processed, MAX_STRING_LENGTH, 1, fp);
	// encode the contents of the file
	encoded = set_encode(processed);
	// free the memory allocated to the string
	free(processed);
	return encoded;
}

/*
 * set_intersect calculates the intersection of set1 and set2
 * @param set1 the first uint64_t
 * @param set2 the second uint64_t
 * return the resultant set which is of type uint64_t
 */
uint64_t set_intersect(uint64_t set1, uint64_t set2) {
	return set1 & set2;
}

/*
 * set_union calculates the union of set1 and set2
 * @param set1 the first uint64_t
 * @param set2 the second uint64_t
 * return the resultant set which is of type uint64_t
 */
uint64_t set_union(uint64_t set1, uint64_t set2) {
	return set1 | set2;
}

/*
 * set_complement calculates the complement of set1
 * @param set1 the first uint64_t
 * returns the resultant set which is of type uint64_t
 */
uint64_t set_complement(uint64_t set1) {
	return ~set1;
}

/*
 * set_difference calculates the difference between set1 and set2
 * @param set1 the first uint64_t
 * @param set2 the second uint64_t
 * returns the resultant set which is of type uint64_t
 */
uint64_t set_difference(uint64_t set1, uint64_t set2) {
	return set1 & ~set2;
}

/*
 * set_symdifference calculates the symmetrical difference using bit
 * manipulation
 * @param set1 the first uint64_t
 * @param set2 the second uint64_t
 * returns the resultant set which is of type uint64_t
 */
uint64_t set_symdifference(uint64_t set1, uint64_t set2) {
	return set_union(set_difference(set1, set2), 
			set_difference(set2, set1));
}

/*
 * set_cardinality calculates the cardinality of the set
 * @set the uint64_t to calculate the cardinality of
 * returns the cardinality of the set
 */
size_t set_cardinality(uint64_t set) {
	// holds the result
	size_t cardinality = 0;
	// represents 8 bits
	uint64_t shift = 1;
	// iterate through the bits backwards
	for(int i = 0; i <= MAX_BIT_POSITION; i++) {
		// if the bits are set, increment cardinality
		if(set & (shift << i))
			cardinality++;
	}
	return cardinality;
}

/*
 * set_decode decodes the encoded uint64_t
 * @param set uint64_t to be decoded
 * return decoded string
 */
char * set_decode(uint64_t set) {
	// uint64_t or 8 bits to be shifted during bit manipulation
	uint64_t shift = 1;
	// string to be returned
	char * decoded = (char*)malloc(sizeof(char) * MAX_STRING_LENGTH);
	// index of the string to append character to
	size_t index = 0;
	// iterate through the bits backwards...
	for(int i = MAX_BIT_POSITION; i > 0; --i) {
		// check if the bits are set
		if(set & (shift << i)) {
			// append the character corresponding to i in chars
			decoded[index] = chars[i];
			// set the next index to the null terminator
			decoded[index + 1] = '\0';
			// increment index to append the next character to the
			// next index
			index++;
		}
	}		
	// return the string
	return decoded;
}

/*
 * fileExists checks if the file exists
 * @param filename the string to check if a file with the string as its name 
 * exists
 * returns 1 if file exists and 0 if file does not exist
 */
int fileExists(char * filename) {
	FILE *file;
	// open the file for reading and if successful...
	if((file = fopen(filename, "r")) != NULL) {
		// close the file and return 1 for success
		fclose(file);
		return 1;
	}
	// return 0 for failure
	return 0;
}

/*
 * setOperations calls the necessary functions to perform bit manipulation on
 * the two sets and print out the results
 * @param set1 the first uint64_t
 * @param set2 the second uint64_t
 */
void setOperations(uint64_t set1, uint64_t set2) {
	// temporary uint64_t to hold the values returned by each function
	uint64_t temp;
	// holds the number returned by the cardinality function
	size_t cardinality;
	// holds the string returned by the decode function
	char * decoded;
	// prints each set after they are encoded in hexadecimal
	printf("set1:\t%#018lx\n", set1);
	printf("set2:\t%#018lx\n\n", set2);
	// print the result of the set intersect between the two sets in 
	// hexadecimal
	temp = set_intersect(set1, set2);
	printf("set_intersect:\t%#018lx\n", temp);
	// print the result of the set union between the two sets in hexadecimal
	temp = set_union(set1, set2);
	printf("set_union:\t%#018lx\n\n", temp);
	// print the complement of set 1 in hexadecimal
	temp = set_complement(set1);
	printf("set1 set_complement:\t%#018lx\n", temp);
	// print the complement of set 2 in hexadecimal
	temp = set_complement(set2);
	printf("set2 set_complement:\t%#018lx\n\n", temp);
	// print the set difference in hexadecimal
	temp = set_difference(set1, set2);
	printf("set_difference:\t\t%#018lx\n", temp);
	// print the set symmetrical difference in hexadecimal
	temp = set_symdifference(set1, set2);
	printf("set_symdifference:\t%#018lx\n\n", temp);
	// print the cardinality of set 1 
	cardinality = set_cardinality(set1);
	printf("set1 set_cardinality:\t%ld\n", cardinality);
	// print the cardinality of set 2
	cardinality = set_cardinality(set2);
	printf("set2 set_cardinality:\t%ld\n\n", cardinality);
	// print the decoded set 1 which is a string
	decoded = set_decode(set1);
	printf("members of set1:\t'%s'\n", decoded);
	// free the string since it was allocated memory from the heap
	free(decoded);
	//print the decoded set 2 which is a string
	decoded = set_decode(set2);
	printf("members of set2:\t'%s'\n", decoded);
	// free the string since it was allocated memory from the heap
	free(decoded);
}

/*
 * main function processes the command line arguments and checks if there are
 * a proper amount of command line arguments
 * @param argc number of command line arguments
 * @param argv pointer to pointers of arguments
 */
int main(int argc, char **argv) {
	// check if there's the correct # of command line arguments
	if(argc != 3) { 
		// print usage message
		fprintf(stderr, "Usage: file-bitsets string1 string2\n");
		return 1;
	} else {
		uint64_t set1, set2 = 0;
		// print and encode string1
		printf("string1:\t%s\t", argv[1]);
		// check if the first input is a file
		if(!fileExists(argv[1])) {
			// if it is not a file, encode the argument as a string
			printf("Encoding the string:\t%s\n", argv[1]);
			set1 = set_encode(argv[1]);	
		} else {
			// if it is a file, encode it as a file
			printf("Encoding the file:\t%s\n", argv[1]);
			FILE *file1 = fopen(argv[1], "rb");
			set1 = file_set_encode(file1);
			// when finished, close the file
			fclose(file1);
		}

		// print and encode string2 
		printf("string2:\t%s\t", argv[2]);
		// check if the second input is a file
		if(!fileExists(argv[2])) {
			// if it is not a file, encode the argument as a string
			printf("Encoding the string:\t%s\n\n", argv[2]);
			set2 = set_encode(argv[2]);
		} else {
			// if it is a file, encode it as a file
			printf("Encoding the file:\t%s\n\n", argv[2]);
			FILE *file2 = fopen(argv[2], "rb");
                        set2 = file_set_encode(file2);
			// when finished, close the file
			fclose(file2);
		}
		// call the necessary operations or functions on the two sets
		setOperations(set1, set2);	
	}	
	return 0;
}
