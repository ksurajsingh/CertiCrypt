#include<ncurses.h>
#include<stdlib.h>
#include "markblock.c"
#include "toJson.c"

int show_menu(){
  initscr();
  noecho();
  cbreak();
  keypad(stdscr,TRUE);

  char *choices[]={
    "1. Add a new block",
    "2. Display the chain",
    "3. Verify Chain [ also verifies Block ]",
    "4. Verify Block [ signature ]",
    "5. Save to Json",      
    "6. Exit"
  };

  int choice,highlight=0;

  while(1){
    clear();
    for(int i=0;i<6;i++){
      if(i==highlight)
        attron(A_REVERSE);
      mvprintw(i+1,2,choices[i]);
      attroff(A_REVERSE);
    }

    int c = getch();
    switch(c){
      case KEY_UP:
        highlight=(highlight==0)?5:highlight-1;
        break;
      case KEY_DOWN:
        highlight=(highlight==5)?0:highlight+1;
        break;
      case 10: // enter key
        choice=highlight;
        endwin();
        return choice;
    }
  }
}

int main(){
  markBlock chain[100];
  int chain_len=0;

  while(1){
    int choice=show_menu();
    switch(choice){
      case 0:{
               char student_id[32];
               int semester,subject_count;

               echo();
               printf("\nEnter student ID: ");
               scanf("%s",student_id);
               printf("\nEnter Semester: ");
               scanf("%d",&semester);
               printf("\nEnter number of subjects: ");
               scanf("%d",&subject_count);

               subjectMark subjects[subject_count];
               for(int i=0;i<subject_count;++i){
                 printf("\nSubject %d name: ",i+1);
                 scanf("%s",&subjects[i].subject);
                 printf("\nSubect %d marks: ",i+1);
                 scanf("%d",&subjects[i].mark);
               }

               const char *prev_hash=(chain_len==0)?"0":chain[chain_len-1].hash;
               chain[chain_len]=create_block(student_id,semester,subjects,subject_count,prev_hash);

               chain_len++;
               getchar(); //pause
               break;

             }
      case 1:
          for(int i=0;i<chain_len;i++){
            printf("\n------------------------\n");
            print_block(&chain[i]);
            printf("\n------------------------\n");
          }
          getchar();
          break;
      case 2:
          verifyChain(chain,chain_len);
          getchar();
          break;
      case 3:
          for(int i=0;i<chain_len;i++){
            printf("Verifying %d block",i+1);
            verifyBlock(chain,i);
          }
          getchar();
          break;
      case 4:
          char filename[50];
          printf("\nEnter the filename to save in: ");
          scanf("%s",filename);
          save_chain_to_json(chain,chain_len,filename);
      case 5:
          printf("\n 👋 Bye . . . .\n");
          exit(0);

    }
    printf("\n\n**** Press enter to menu 📑 ****");
    getchar();
  }

  //
  // const subjectMark sem1[]={{"Math",88},{"DSA",91}};
  // const markBlock block1 = create_block("2023CS001",1,sem1,2,"0");
  //
  // const subjectMark sem2[]={{"OS",89},{"Crypto",93}};
  // const markBlock block2 = create_block("2023CS001",2,sem2,2,block1.hash);
  //
  // markBlock chain[] ={block1,block2};
  //
  // print_block(&chain[0]);
  // printf("\n\n");
  // print_block(&chain[1]);


  // tamper test
  // printf("before tampering: %s",chain[0].hash);
  // chain[0].subjects[1].mark=95;
  // printf("\nAfter tampering: %s\n",chain[0].hash);
  // tamper test
  // print_block(&chain[0]);
  // printf("\n\n");
  // print_block(&chain[1]);
  // verifyChain(chain,2); // give the length as ```total blocks - 1``` since last block doesn't need to be verified [ dummy ]

  return 0;
}
