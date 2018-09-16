#include <stdio.h>
#include <errno.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
	char magic_number[4]; //different between little and big endian?
	uint32_t unk0; //1
	uint32_t unk1; //1
	uint32_t unk2; //1
	uint32_t unk3; //54h
	uint32_t ptr_eof;
	uint32_t unk4; //6
	uint32_t unk5; //2ch
	uint32_t unk6; //1
	uint32_t unk7; //10h
	uint32_t unk8; //2
	uint32_t unk9; //24h
	uint32_t num_stateEntry;
	uint32_t unk11; //1ch
	uint32_t num_nextEntry;
	uint32_t unk13; //10h
	uint32_t num_functionEntry;
	uint32_t unk15; //8
	uint32_t num_functionParamEntry;
	uint32_t ptr_transitionEntry;
	uint32_t num_transitionEntry;
	uint32_t ptr_end_of_bs0;
	uint32_t pad0;
	uint32_t ptr_end_of_bs1; //same as ptr_end_of_bs0
	uint32_t pad1;
	uint32_t ptr_end_of_bs2; //same as ptr_end_of_bs0
	uint32_t pad2;
	uint32_t unk18; //1, 0 in console versions?
	char unk19[16]; //useless?
} header;

typedef struct {
	uint32_t pointer0; //2ch not sure
	uint32_t entriesToRead0; //not sure
	uint32_t pointer1; //ffffffff not sure
	uint32_t entriesToRead1; //not sure
	char pad2[12];
} lolHeader1;

typedef struct {
	uint32_t pointer0; //4ch, points to stateEntry section
	uint32_t entriesToRead0;
	uint32_t pointer1; //4ch, same as unk0
	uint32_t entriesToRead1; //idk what this is for
	uint32_t pointer2; //points to stateEntry section
	uint32_t entriesToRead2;
	uint32_t pointer3; //same as unk4
} lolHeader2;

typedef struct {
	uint32_t index; //correlates with the state number shown in debug
	uint32_t pointer0; //can be ffffffff, points to transition section
	uint32_t numEntriesToRead0;
	uint32_t pointer1; //points to function section
	uint32_t numEntriesToRead1;
	uint32_t pointer2; //can be ffffffff, points to function section
	uint32_t numEntriesToRead2;
	uint32_t pad0; //ffffffff, pointer? never points to anything
	uint32_t pad1; //0, bytes to read?
} stateEntry;

//maybe "subState" would be appropriate here
typedef struct {
	uint32_t pointer0; //points to stateEntry, can be ffffffff
	uint32_t pointer1; //can be ffffffff, points to transition section
	uint32_t bytesToRead1;
	uint32_t pointer2; //can be ffffffff, points to transition section
	uint32_t bytesToRead2;
	uint32_t pointer3; //points to BS section
	uint32_t bytesToRead3;
} nextEntry;

typedef struct {
	uint32_t unk0; //1
	uint32_t unk1; //function id
	uint32_t pointer0; //can be ffffffff, points at functionParam
	uint32_t numEntriesToRead; //number of parameters
} functionEntry;
			  //72, 78, 79, 7B, 7C, 7D, 84, 87, 8B, 91, 92, 93 have no args

typedef struct {
	uint32_t pointer0; //points into BS section
	uint32_t bytesToRead;
} functionParamEntry;

typedef struct {
	uint32_t pointer0; //points to nextEntry section
} transitionEntry;

typedef struct {
	char garbo[];
} BSEntry;

extern int recursiveBULLSHIT(FILE *, FILE *, int);
extern int insertEventMessage(const int, const int);