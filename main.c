#include "markblock.c"

int main(){
  subjectMark sem1[]={{"Math",88},{"DSA",91}};
  markBlock block1 = create_block("2023CS001",1,sem1,2,"0");

  subjectMark sem2[]={{"OS",89},{"Crypto",93}};
  markBlock block2 = create_block("2023CS001",2,sem2,2,block1.hash);

  markBlock chain[] ={block1,block2};

  print_block(&chain[0]);
  printf("\n\n");
  print_block(&chain[1]);
  chain[1].subjects[0].mark=100;
  compute_hash(&chain[1]);
  verifyChain(chain,2);

  return 0;
}
