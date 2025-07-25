#include<cjson/cJSON.h>
#include<time.h>


void save_chain_to_json(markBlock *chain, int length, const char *filename) {
    // Check if file exists and read last line
    FILE *fprev = fopen(filename, "r");
    if (fprev) {
        char line[8192];
        char last_line[8192] = {0};

        while (fgets(line, sizeof(line), fprev)) {
            if (strlen(line) > 1)
                strcpy(last_line, line);
        }
        fclose(fprev);

        if (strlen(last_line) > 0) {
            cJSON *last_block = cJSON_Parse(last_line);
            if (last_block) {
                cJSON *last_hash = cJSON_GetObjectItem(last_block, "hash");
                if (last_hash && cJSON_IsString(last_hash)) {
                    // Update only if the first block has prev_hash as "0"
                    if (strcmp(chain[0].prev_hash, "0") == 0)
                        strncpy(chain[0].prev_hash, last_hash->valuestring, sizeof(chain[0].prev_hash) - 1);
                }
                cJSON_Delete(last_block);
            }
        }
    }

    // Now append blocks as before
    FILE *fp = fopen(filename, "a");
    if (!fp) {
        perror("fopen");
        return;
    }

    for (int i = 0; i < length; i++) {
        cJSON *jblock = cJSON_CreateObject();
        cJSON_AddStringToObject(jblock, "student_id", chain[i].student_id);
        cJSON_AddNumberToObject(jblock, "semester", chain[i].semester);
        cJSON_AddStringToObject(jblock, "timestamp", chain[i].timestamp_str);
        cJSON_AddStringToObject(jblock, "prev_hash", chain[i].prev_hash);
        cJSON_AddStringToObject(jblock, "hash", chain[i].hash);

        // Signature
        unsigned char encoded_sig[2048] = {0};
        int encoded_len = EVP_EncodeBlock(encoded_sig, (unsigned char *)chain[i].signature, chain[i].sig_len);
        cJSON_AddStringToObject(jblock, "signature", (char *)encoded_sig);

        // Subjects
        cJSON *jsubjects = cJSON_CreateArray();
        for (int j = 0; j < chain[i].subject_count; j++) {
            cJSON *jsub = cJSON_CreateObject();
            cJSON_AddStringToObject(jsub, "subject", chain[i].subjects[j].subject);
            cJSON_AddNumberToObject(jsub, "mark", chain[i].subjects[j].mark);
            cJSON_AddItemToArray(jsubjects, jsub);
        }
        cJSON_AddItemToObject(jblock, "subjects", jsubjects);

        // Print and write
        char *string = cJSON_PrintUnformatted(jblock);
        fprintf(fp, "%s\n", string);

        free(string);
        cJSON_Delete(jblock);
    }

    fclose(fp);
    printf("☑️ Blocks appended to %s\n", filename);
}


void load_chain_from_json(markBlock *loaded_chain, int *loaded_len, const char *filename) {
    FILE *f = fopen(filename, "r");
    if (!f) {
        perror("Couldn't open JSON file");
        return;
    }

    char line[8192]; // adjust as needed
    int i = 0;

    while (fgets(line, sizeof(line), f) && i < MAX_BLOCKS) {
        cJSON *block_json = cJSON_Parse(line);
        if (!block_json) {
            fprintf(stderr, "❌ Error parsing JSON line %d\n", i + 1);
            continue;
        }

        markBlock *b = &loaded_chain[i];
        memset(b, 0, sizeof(markBlock));

        strncpy(b->student_id, cJSON_GetObjectItem(block_json, "student_id")->valuestring, MAX_STR - 1);
        b->semester = cJSON_GetObjectItem(block_json, "semester")->valueint;



        strncpy(b->timestamp_str, cJSON_GetObjectItem(block_json, "timestamp")->valuestring, sizeof(b->timestamp_str)-1);
        // strncpy(b->timestamp_str, cJSON_GetObjectItem(block_json, "timestamp")->valuestring, 63);

        strncpy(b->prev_hash, cJSON_GetObjectItem(block_json, "prev_hash")->valuestring, 64);
        strncpy(b->hash, cJSON_GetObjectItem(block_json, "hash")->valuestring, 64);

        const char *sig_base64 = cJSON_GetObjectItem(block_json, "signature")->valuestring;
        b->sig_len = EVP_DecodeBlock(b->signature, (const unsigned char *)sig_base64, strlen(sig_base64));

        cJSON *subjects = cJSON_GetObjectItem(block_json, "subjects");
        b->subject_count = cJSON_GetArraySize(subjects);
        for (int j = 0; j < b->subject_count && j < MAX_SUBJECTS; j++) {
            cJSON *subj = cJSON_GetArrayItem(subjects, j);
            strncpy(b->subjects[j].subject, cJSON_GetObjectItem(subj, "subject")->valuestring, MAX_STR - 1);
            b->subjects[j].mark = cJSON_GetObjectItem(subj, "mark")->valueint;
        }

        cJSON_Delete(block_json);
        i++;
    }

    fclose(f);
    *loaded_len = i;
    printf("✅ Loaded %d blocks from %s\n", *loaded_len, filename);
}

void verify_chain_from_json(markBlock *loaded_chain,int loaded_len){
  verifyChain(loaded_chain,loaded_len);
}

void display_chain_from_json(markBlock *loaded_chain,int loaded_len){
  for(int i=0;i<loaded_len;i++){
    printf("\n------------------------\n");
    print_block(&loaded_chain[i]);
    printf("\n------------------------\n");
  }
}
