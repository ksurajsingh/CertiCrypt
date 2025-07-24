#include<cjson/cJSON.h>

void save_chain_to_json(markBlock *chain,int length, const char *filename){
  cJSON *jchain = cJSON_CreateArray();

  for(int i=0;i<length;i++){
    cJSON *jblock=cJSON_CreateObject();
    cJSON_AddStringToObject(jblock,"student_id",chain[i].student_id);
    cJSON_AddNumberToObject(jblock,"semester",chain[i].semester);
    char timestamp_str[64];
    strftime(timestamp_str,sizeof(timestamp_str),"%Y-%m-%d %H:%M:%S",localtime(&chain[i].timestamp));
    cJSON_AddStringToObject(jblock,"timestamp",timestamp_str);
    cJSON_AddStringToObject(jblock,"prev_hash",chain[i].prev_hash);
    cJSON_AddStringToObject(jblock,"hash",chain[i].hash);

    // saving signature
    char sig_hex[513]={0};
    for(size_t j=0;j<chain[i].sig_len;j++){
      snprintf(&sig_hex[j*2],3,"%02x",chain[i].signature[j]);
    }
    cJSON_AddStringToObject(jblock,"signature",sig_hex);

    // saving subjects 
    cJSON *jsubjects = cJSON_CreateArray();
    for(int j=0;j<chain[i].subject_count;j++){
      cJSON *jsub=cJSON_CreateObject();
      cJSON_AddStringToObject(jsub,"subject",chain[i].subjects[j].subject);
      cJSON_AddNumberToObject(jsub,"mark",chain[i].subjects[j].mark);
      cJSON_AddItemToArray(jsubjects,jsub);
    }
    cJSON_AddItemToObject(jblock,"subjects",jsubjects);
    cJSON_AddItemToArray(jchain,jblock);
  }

  char *string=cJSON_Print(jchain);
  FILE *fp=fopen(filename,"w");
  if (fp){
    fputs(string,fp);
    fclose(fp);
    printf("☑️ JSON saved to %s\n",filename);
  }
  cJSON_Delete(jchain);
  free(string);

}
