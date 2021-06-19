#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fs.h"

FileObj * files;

struct file_nodes * root;


void fileCreateFromObject(FILE *outfile ,struct file_nodes *  file, char * content){
  fwrite (file, sizeof(struct file_nodes), 1, outfile);
  fwrite (content, file->length, 1, outfile);
}
void fileCreate(FILE *outfile, char name[128], char * content){

    struct file_nodes new_file;
    new_file.length = strlen(content);
    strcpy(new_file.name,name);
    new_file.permission =  333;

    fileCreateFromObject(outfile, &new_file, content);
}



FileObj find(FILE *infile, char name[128]){
    u32 current_offset = ftell(infile);
    struct file_nodes input;
    if (fseek(infile, SKIP_BLOCKS*BLOCK, SEEK_SET)){
        printf("Failed to move\n");
        FileObj empty_file;
        fseek(infile, current_offset, SEEK_SET);
        return empty_file;
    }
	FileObj file;
    // read file contents till end of file
    int i = 0;

    while (fread(&input, sizeof(struct file_nodes), 1, infile)){

        file.length = input.length;
        file.offset = ftell(infile);
        file.info = 0;
        file.currentOffset = 0;
        file.open = 1;
		strcpy(file.name, input.name);
      
        
        if (!strcmp(input.name, name)){
            fseek(infile, current_offset, SEEK_SET);
            return file;
        }
        //fseek(infile, ftell(infile)+input.length, SEEK_SET);
        memset(&file, 0, sizeof(FileObj));
      i++;
    }
    FileObj empty_file;
    fseek(infile, current_offset, SEEK_SET);
    return empty_file;
}


FileObj fileOpen(FILE *infile, char name[128]){
    FileObj file = find(infile,name);
    return file;
}

char * fileRead(FILE *infile, FileObj *file,u32 amount){
    u32 current_offset = ftell(infile);
    if (file->open){
        if (file->offset == 0){
            printf("not a file\n");
            fseek(infile, current_offset, SEEK_SET);
            return 0;
        }
        
        else{
            fseek(infile, file->offset+file->currentOffset,SEEK_SET);
            char  * content  = (char *)calloc(amount,sizeof(char));
            if (file->length < amount || file->length < amount+file->currentOffset){
                fread(content, file->length, 1, infile);
                file->currentOffset = file->length;
            }
            else{
                fread(content, amount, 1, infile);
                file->currentOffset = file->currentOffset+amount;
            }
            //fseek(infile, current_offset, SEEK_SET);
            return content;
        }
    }else{
        return 0;
    }
}



bool fileSetOffset(FileObj * file, u32 offset){
    file->currentOffset;
    return 1;
}

int main ()
{
    FILE *outfile;
      
    // open file for writing
    outfile = fopen ("fs.img", "w");
    if (ferror(outfile))
    {
        fprintf(stderr, "\nError when opening file\n");
        exit (1);
    }


      
    // write struct to file
    char first_block[512] = {0};
    fwrite (first_block ,sizeof(first_block), 1, outfile);

    
    fileCreate(outfile, "file.txt\0", "hello121hello1");

  
      
    if(ferror(outfile)) 
        printf("contents to file written successfully !\n");
    else 
        printf("error writing file !\n");
  
    // close file
    fclose (outfile);





    FILE *infile;
    struct file_nodes input;
      
    // Open person.dat for reading
    infile = fopen ("fs.img", "r");
    if (ferror(infile)){
        fprintf(stderr, "\nError when opening file\n");
    }

    // Skip all the blocks that are used for info and booting
    fseek(infile, SKIP_BLOCKS*BLOCK, SEEK_SET);
    printf("Finding file\n");
    FileObj filetxt = fileOpen(infile,"file.txt\0");
    printf("Reading file\n");
    
    char * content = fileRead(infile,&filetxt,3);
    char * content1 = fileRead(infile,&filetxt,8);
    printf("content amount = %li\n", strlen(content));
    printf("content = %s\n",content);
    printf("content = %s\n",content1);
	//fileRead(infile, "hello\0");
    // close file
    fclose (infile);


  
    return 1;
}