#include "esd_rebuild.h"

int pos_output;
stateEntry * states_list1;
stateEntry * states_list2;
lolHeader1 subheader1;
lolHeader2 subheader2;
header header1;
nextEntry * next_list;
functionEntry * function_list;
functionParamEntry * functionParam_list;
transitionEntry * trans_list;
unsigned char * bs_list;
int num_states1 = 0;
int num_states2 = 0;
int offset_functions = 0;
int offset_functionParams = 0;

//stupid offset
const int FILE_OFFSET = 0x6c;
//function ids
const int ID_SEND_EVENT_MESSAGE = 0x6c;
const int ID_PLAY_ANIMATION_1 = 0x3;

int main(int argc, char* argv[]) {
	int err;
	char err_buff[256];
	FILE * esd;
	FILE * output;

	if (argc == 1) {
		printf("Usage: ESDShit <path_to_file>");
		exit(0);
	}

	if ((err = fopen_s(&esd, argv[1], "r+b")) != 0) {
		strerror_s(err_buff, 256, err);
		fprintf(stderr, err_buff);
		exit(0);
	}
	else {
		
		fread(&header1, sizeof(header), 1, esd);
		fread(&subheader1, sizeof(subheader1), 1, esd);
		fread(&subheader2, sizeof(subheader2), 1, esd);

		num_states1 = subheader2.entriesToRead0;
		num_states2 = subheader2.entriesToRead2;

		states_list1 = malloc(sizeof(stateEntry)*(num_states1+1));
		states_list2 = malloc(sizeof(stateEntry)*(num_states2+1));

		printf("num passive states: %d, num active states: %d\n", num_states1, num_states2);

		fread(states_list1, sizeof(stateEntry), num_states1+1, esd);
		fread(states_list2, sizeof(stateEntry), num_states2+1, esd);

		next_list = malloc(sizeof(nextEntry)*header1.num_nextEntry);

		fread(next_list, sizeof(nextEntry), header1.num_nextEntry, esd);

		function_list = malloc(sizeof(functionEntry)*header1.num_functionEntry);
		functionParam_list = malloc(sizeof(functionParamEntry)*header1.num_functionParamEntry);
		offset_functions = ftell(esd);

		fread(function_list, sizeof(functionEntry), header1.num_functionEntry, esd);
		offset_functionParams = ftell(esd);
		fread(functionParam_list, sizeof(functionParamEntry), header1.num_functionParamEntry, esd);

		trans_list = malloc(sizeof(transitionEntry)*header1.num_transitionEntry);

		fread(trans_list, sizeof(transitionEntry), header1.num_transitionEntry, esd);

		bs_list = malloc(header1.ptr_end_of_bs0 - next_list[0].pointer3);

		fread(bs_list, 1, header1.ptr_end_of_bs0 - next_list[0].pointer3, esd);
		
		rewind(esd);
		fclose(esd);

		//create file listing all functions from function_list + the code associated with them

		/*FILE * functions;
		
		if ((err = fopen_s(&functions, "functions.txt", "w")) != 0) {
			strerror_s(err_buff, 256, err);
			fprintf(stderr, err_buff);
			exit(0);
		}
		else {
			for (unsigned i = 0; i < header1.num_functionEntry; i++) {
				if (function_list[i].pointer0 < 0xffffffff) {
					for (unsigned j = 0; j < function_list[i].numEntriesToRead; j++) {
						fseek(esd, function_list[i].pointer0 + (j * 8) + 0x6c, SEEK_SET);
						//printf("pointer %X ", function_list[i].pointer0);
						int pos = 0;
						int bytes = 0;
						fread(&pos, 4, 1, esd);
						fread(&bytes, 4, 1, esd);
						fseek(esd, pos + 0x6c, SEEK_SET);
						//printf("bytes %d ", bytes);
						unsigned char * shit = malloc(bytes + 1);
						fread(shit, bytes, 1, esd);
						for (int k = 0; k < bytes; k++) {
							printf("%X", shit[k]);
						}
						printf("|");
						free(shit);
					}
				}
				printf("\n");
			}
		}*/
		
		//printing out dot graph

		/*if ((err = fopen_s(&output, "kek.dot", "w")) != 0) {
			strerror_s(err_buff, 256, err);
			fprintf(stderr, err_buff);
			exit(0);
		}
		else {
			fwrite("digraph G {\n", 1, 12, output);
			pos_output = 12;
			for (int i = 0; i < num_states2; i++) {
				i = 46;
				for (unsigned j = 0; j < states_list2[i].numEntriesToRead0; j++) {
					fseek(esd, states_list2[i].pointer0 + (j * 4) + 0x6c, SEEK_SET);
					int pos = 0;
					int pos2 = 0;
					fread(&pos, 4, 1, esd);
					
					fseek(esd, pos + 0x6c, SEEK_SET);
					fread(&pos2, 4, 1, esd);
					if (pos2 > -1) {
						//printf("%d\n", pos2 + 0x6c);
						fseek(esd, pos2 + 0x6c, SEEK_SET);
						int idx = 0;
						fread(&idx, 4, 1, esd);
						fseek(output, pos_output, SEEK_SET);
						//fwrite(&states_list1[i].index, 4, 1, output);
						fprintf(output, "%d", states_list2[i].index);
						fwrite(" -> ", 1, 4, output);
						//printf("idx %d\n", idx);
						fprintf(output, "%d", idx);
						//fwrite(&idx, 4, 1, output);
						//printf("%d, ", idx);
						fwrite("\n", 1, 1, output);
						pos_output = ftell(output);
					}
					else {
						recursiveBULLSHIT(esd, output, i);
					}
				}
				break;
			}
			fseek(output, pos_output, SEEK_SET);
			fwrite("}", 1, 1, output);
			fclose(output);
		}*/

		insertEventMessage(46, 3000);
		insertEventMessage(48, 3001);
		insertEventMessage(49, 3002);
		
	}
	
	if ((err = fopen_s(&output, "lol.hex", "w+b")) != 0) {
		strerror_s(err_buff, 256, err);
		fprintf(stderr, err_buff);
		exit(0);
	}
	else {
		
		fwrite(&header1, sizeof(header), 1, output);
		fwrite(&subheader1, sizeof(subheader1), 1, output);
		fwrite(&subheader2, sizeof(subheader2), 1, output);
		fwrite(states_list1, sizeof(stateEntry), num_states1+1, output);
		fwrite(states_list2, sizeof(stateEntry), num_states2+1, output);
		fwrite(next_list, sizeof(nextEntry), header1.num_nextEntry, output);
		fwrite(function_list, sizeof(functionEntry), header1.num_functionEntry, output);
		fwrite(functionParam_list, sizeof(functionParamEntry), header1.num_functionParamEntry, output);
		fwrite(trans_list, sizeof(transitionEntry), header1.num_transitionEntry, output);
		fwrite(bs_list, 1, header1.ptr_end_of_bs0 - next_list[0].pointer3, output);
		fclose(output);
	}

	
	
	free(states_list1);
	free(states_list2);
	free(next_list);
	free(function_list);
	free(functionParam_list);
	free(trans_list);
	free(bs_list);
	return 0;
}

int insertEventMessage(const int state, const int message_id) {
	uint32_t state_pointer = states_list2[state].pointer1;
	functionEntry * temp_list = malloc(sizeof(functionEntry)*(header1.num_functionEntry + 1));
	unsigned idx = (states_list2[state].pointer1 + FILE_OFFSET - offset_functions + states_list2[state].numEntriesToRead1 * sizeof(functionEntry)) / sizeof(functionEntry);
	states_list2[state].numEntriesToRead1 += 1;
	for (unsigned i = 0; i < header1.num_functionEntry + 1; i++) {
		if (i < idx) {
			temp_list[i] = function_list[i];
			if (temp_list[i].pointer0 < 0xffffffff)
				temp_list[i].pointer0 += 16;
		}
		else if (i == idx) {
			functionEntry temp_entry;
			temp_entry.unk0 = 1;
			temp_entry.pointer0 = temp_list[i - 1].pointer0 + sizeof(functionParamEntry)*temp_list[i - 1].numEntriesToRead;
			temp_entry.numEntriesToRead = 1;
			temp_entry.unk1 = ID_SEND_EVENT_MESSAGE;
			temp_list[i] = temp_entry;
		}
		else {
			temp_list[i] = function_list[i - 1];
			if (temp_list[i].pointer0 < 0xffffffff)
				temp_list[i].pointer0 += 24;
		}

	}
	offset_functionParams += 16;
	header1.num_functionEntry += 1;
	free(function_list);
	function_list = temp_list;

	functionParamEntry * temp_list2 = malloc(sizeof(functionParamEntry)*(header1.num_functionParamEntry + 1));
	unsigned idx2 = (function_list[idx - 1].pointer0 + FILE_OFFSET - offset_functionParams + function_list[idx - 1].numEntriesToRead * sizeof(functionParamEntry)) / sizeof(functionParamEntry);
	for (unsigned i = 0; i < header1.num_functionParamEntry + 1; i++) {
		if (i < idx2) {
			temp_list2[i] = functionParam_list[i];
			if (temp_list2[i].pointer0 < 0xffffffff)
				temp_list2[i].pointer0 += 24;
		}
		else if (i == idx2) {
			functionParamEntry temp_entry;
			temp_entry.pointer0 = temp_list2[i - 1].pointer0 + temp_list2[i - 1].bytesToRead;
			temp_entry.bytesToRead = 6;
			temp_list2[i] = temp_entry;
		}
		else {
			temp_list2[i] = functionParam_list[i - 1];
			if (temp_list2[i].pointer0 < 0xffffffff)
				temp_list2[i].pointer0 += 30;
		}

	}
	header1.num_functionParamEntry += 1;
	free(functionParam_list);
	functionParam_list = temp_list2;

	next_list[0].pointer3 += 24;
	header1.ptr_end_of_bs0 += 30;

	unsigned char * temp_list3 = malloc(header1.ptr_end_of_bs0 - next_list[0].pointer3 + 6);

	unsigned idx3 = functionParam_list[idx2 - 1].pointer0 + FILE_OFFSET - (next_list[0].pointer3 + FILE_OFFSET) + functionParam_list[idx2 - 1].bytesToRead;
	for (unsigned i = 0; i < header1.ptr_end_of_bs0 - next_list[0].pointer3 - 5; i++) {
		if (i < idx3) {
			temp_list3[i] = bs_list[i];
		}
		else if (i == idx3) {
			//82 00 00 27 10 A1
			temp_list3[i] = 0x82;
			memcpy(&temp_list3[i + 1], &message_id, 4);
			/*temp_list3[i + 1] = 0xb8;
			temp_list3[i + 2] = 0x0b;
			temp_list3[i + 3] = 0x00;
			temp_list3[i + 4] = 0x00;*/
			temp_list3[i + 5] = 0xa1;
		}
		else {
			temp_list3[i + 5] = bs_list[i - 1];
		}
	}

	free(bs_list);
	bs_list = temp_list3;

	//update pointers
	header1.ptr_transitionEntry += 24;
	header1.ptr_end_of_bs1 += 30;
	header1.ptr_end_of_bs2 += 30;

	for (int i = 0; i < num_states1 + 1; i++) {
		states_list1[i].pointer0 += 24;
		if (states_list1[i].pointer1 > state_pointer && states_list1[i].pointer1 < 0xffffffff) {
			states_list1[i].pointer1 += 16;
		}
		if (states_list1[i].pointer2 > state_pointer && states_list1[i].pointer2 < 0xffffffff) {
			states_list1[i].pointer2 += 16;
		}
	}
	for (int i = 0; i < num_states2 + 1; i++) {
		states_list2[i].pointer0 += 24;
		if (states_list2[i].pointer1 > state_pointer && states_list2[i].pointer1 < 0xffffffff) {
			states_list2[i].pointer1 += 16;
		}
		if (states_list2[i].pointer2 > state_pointer && states_list2[i].pointer2 < 0xffffffff) {
			states_list2[i].pointer2 += 16;
		}
	}
	for (unsigned i = 0; i < header1.num_nextEntry; i++) {
		if (next_list[i].pointer1 < 0xffffffff)
			next_list[i].pointer1 += 24;
		if (next_list[i].pointer2 < 0xffffffff)
			next_list[i].pointer2 += 24;
		if (i != 0 && next_list[i].pointer3 < 0xffffffff) {
			if (functionParam_list[idx2].pointer0 < next_list[i].pointer3) {
				next_list[i].pointer3 += 30;
			}
			else {
				next_list[i].pointer3 += 24;
			}
		}
	}
	return 0;
}

//function for printing dot graph
int recursiveBULLSHIT(FILE * esd, FILE * output, int index) {
	int transition1;
	int transition2;
	int entries1;
	int entries2;
	fread(&transition1, 4, 1, esd);
	fread(&entries1, 4, 1, esd);
	fread(&transition2, 4, 1, esd);
	fread(&entries2, 4, 1, esd);
	if (transition1 > -1) {
		int last_idx = -1;
		for (int k = 0; k < entries1; k++) {
			fseek(esd, transition1 + (k * 4) + FILE_OFFSET, SEEK_SET);
			int pos3 = 0;
			fread(&pos3, 4, 1, esd);
			fseek(esd, pos3 + FILE_OFFSET, SEEK_SET);
			int pos4 = 0;
			fread(&pos4, 4, 1, esd);
			if (pos4 > -1) {
				//printf("%d\n", pos4 + 0x6c);
				fseek(esd, pos4 + FILE_OFFSET, SEEK_SET);
				int idx = 0;
				fread(&idx, 4, 1, esd);
				if (idx != last_idx) {
					last_idx = idx;
					fseek(output, pos_output, SEEK_SET);
					fprintf(output, "%d", states_list2[index].index);
					fwrite(" -> ", 1, 4, output);
					fprintf(output, "%d", idx);
					fwrite("\n", 1, 1, output);
					pos_output = ftell(output);
				}
			}
			else {
				recursiveBULLSHIT(esd, output, index);
			}
		}
	}
	else if (transition2 > -1) {
		int last_idx = -1;
		for (int k = 0; k < entries2; k++) {
			fseek(esd, transition2 + (k * 4) + FILE_OFFSET, SEEK_SET);
			int pos3 = 0;
			fread(&pos3, 4, 1, esd);
			fseek(esd, pos3 + FILE_OFFSET, SEEK_SET);
			int pos4 = 0;
			fread(&pos4, 4, 1, esd);
			if (pos4 > -1) {
				//printf("%d\n", pos4 + 0x6c);
				fseek(esd, pos4 + FILE_OFFSET, SEEK_SET);
				int idx = -1;
				fread(&idx, 4, 1, esd);
				if (idx != last_idx) {
					last_idx = idx;
					fseek(output, pos_output, SEEK_SET);
					fprintf(output, "%d", states_list2[index].index);
					fwrite(" -> ", 1, 4, output);
					fprintf(output, "%d", idx);
					fwrite("\n", 1, 1, output);
					pos_output = ftell(output);
				}
			}
			else {
				recursiveBULLSHIT(esd, output, index);
			}
		}
	}
	return 0;
}