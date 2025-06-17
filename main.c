#include "markblock.c"

int main(){
  const subjectMark sem1[]={{"Math",88},{"DSA",91}};
  const markBlock block1 = create_block("2023CS001",1,sem1,2,"0");

  const subjectMark sem2[]={{"OS",89},{"Crypto",93}};
  const markBlock block2 = create_block("2023CS001",2,sem2,2,block1.hash);

  markBlock chain[] ={block1,block2};

  print_block(&chain[0]);
  printf("\n\n");
  print_block(&chain[1]);


  // tamper test
  printf("before tampering: %s",chain[0].hash);
  // chain[0].subjects[1].mark=95;
  printf("\nAfter tampering: %s\n",chain[0].hash);
  // tamper test
  print_block(&chain[0]);
  printf("\n\n");
  print_block(&chain[1]);
  verifyChain(chain,2); // give the length as ```total blocks - 1``` since last block doesn't need to be verified [ dummy ]

  return 0;
}
