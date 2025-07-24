#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include<openssl/sha.h>

#include "digitalsignature.c"

#define MAX_SUBJECTS 20
#define MAX_STR 256

typedef struct{
  char user_id[32];
  EVP_PKEY *private_key;
  EVP_PKEY *public_key;
} User;

typedef struct{
  char subject[MAX_STR];
  int mark;
} subjectMark;

typedef struct{
  char student_id[MAX_STR];
  int semester;
  subjectMark subjects[MAX_SUBJECTS];
  int subject_count;
  time_t timestamp;
  char prev_hash[65]; // SHA-256 = 64 hex chars + null 
  char hash[65];
  char signed_by[32];
  unsigned char signature[512]; // 4096 bit rsa
  size_t sig_len;
} markBlock;

void to_hex_string(unsigned char *hash,char *output){
  for(int i=0;i<SHA256_DIGEST_LENGTH;i++){
    sprintf(output+(i*2),"%02x",hash[i]);
  }
  output[64]='\0';
}

void compute_hash(markBlock *block){
  char data[2048];
  memset(data,0,sizeof(data));
  char temp[256];

  snprintf(data,sizeof(data),"%s%d%ld%s",block->student_id,block->semester,block->timestamp,block->prev_hash);

  for (int i=0;i<block->subject_count;i++){
    snprintf(temp,sizeof(temp),"%s%d",block->subjects[i].subject,block->subjects[i].mark);
    strncat(data,temp,sizeof(data)-strlen(data)-1);
  }
  unsigned char hash_bin[SHA256_DIGEST_LENGTH];
  SHA256((unsigned char *)data,strlen(data),hash_bin);
  to_hex_string(hash_bin,block->hash);
}

markBlock create_block(const char *student_id,const int semester,const subjectMark *subjects,const int subject_count,const char *prev_hash){
  markBlock block;
  memset(&block,0,sizeof(markBlock));
  strncpy(block.student_id,student_id,MAX_STR-1);
  block.semester=semester;
  block.subject_count=subject_count;
  for(int i=0;i<subject_count && i < MAX_SUBJECTS; i++){
    strncpy(block.subjects[i].subject,subjects[i].subject,MAX_STR-1);
    block.subjects[i].mark=subjects[i].mark;
  }
  block.timestamp=time(NULL);
  if(prev_hash){
    strncpy(block.prev_hash,prev_hash,64);
    block.prev_hash[64]='\0';
  }
  else 
    strcpy(block.prev_hash,"0");
  compute_hash(&block);

  if(!sign_block(block.hash,strlen(block.hash),block.signature,&block.sig_len))
    fprintf(stderr,"\n❌ Failed to sign the block!\n");
  else {
    printf("\n✔️Signed!\n");
  }

  return block;
}

void get_hash(markBlock *block,char *out_hash){
  unsigned char temp_hash[SHA256_DIGEST_LENGTH];

  // we consider that the hacker can change the hash of the block aswell and so 
  // we compute the has while verification
  char data[200];
  char temp[200];
  snprintf(data,sizeof(data), "%s%d%ld%s",block->student_id,block->semester,block->timestamp,block->prev_hash);
  for(int i=0;i<block->subject_count;i++){
    snprintf(
        temp,sizeof(temp), "%s%d",block->subjects[i].subject,block->subjects[i].mark);
    strncat(data,temp,sizeof(data)-strlen(data)-1);
  }
  SHA256((unsigned char*)data,strlen(data),temp_hash);
  to_hex_string(temp_hash,out_hash);
}


void verifyBlock(markBlock *block,int i){
    if(!verify_block(block->hash,strlen(block->hash),block->signature,&block->sig_len))
      fprintf(stderr,"\n❌Verification failed.\nThe user who signed block no: %d\nIs not Authentic[registered]!\n",i);
    else {
      printf("\nBlock %d verified.\n✔️ Valid user!\n",i);
    }
  }


int verifyChain(markBlock *chain,int length){
  for(int i=1;i<=length;i++){
    printf("\nVerifying block %d signature. . . \n",i-1);
    verifyBlock(&chain[i-1],i-1);
    if (strncmp(chain[i-1].hash,chain[i].prev_hash,64)!=0){ // if the hacker tampers the data AND THE HASH - we check the hash stored in the `prev_hash` of the next block
                                                          //This is efficient since its bypass would make the hacker to change the hash of al the blocks after the tampered block!
      printf("❎ Block %d is tampered! \nHash mismatch",i-1);
      return 0;
    }
    char expected_hash[65]; // actual hash
    get_hash(&chain[i-1],expected_hash);
    if(strncmp(expected_hash,chain[i-1].hash,64)!=0){
      // if the hacker tampers the data and leaves the hash as it is [ original hash ] so that the block could impersonate authenticity.
      // This makes sure to check the stored [ present ] hash with the actual hash [ computed for the present data ]
      printf("❎ Block %d is tampered! \nData mismatch!",i-1);
      return 0;
    }
  }
  printf("✅Chain is valid!");
  return 1;
}


void print_block(markBlock *block){
  printf("Student ID: %s\n",block->student_id);
  printf("Semester: %d\n",block->semester);
  printf("Timestamp: %ld\n",block->timestamp);
  printf("Subjects: \n");
  for(int i=0;i<block->subject_count;i++){
    printf("%s: %d\n",block->subjects[i].subject,block->subjects[i].mark);
  }
  printf("Previous Hash: %s\n",block->prev_hash);
  printf("Block Hash: %s\n",block->hash);
}
