#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct hchar{
    char name[32];
    char line[296];
    int cid;
    int nvert;
    int lhp;
    int rhp;
    int cp[144][2];
};

struct hfont {
    char name[16];
    struct hchar hchars[96];
    int num_hchars;
};

void print_hchar_info(struct hchar hc) {
    printf("Hershey character %d has %d vertices:\n", hc.cid, hc.nvert);
    printf("\t(lhp, rhp): (%d, %d)\n", hc.lhp, hc.rhp);
    for(int j=0; j<hc.nvert; j++) {
        printf("\t%d: (%d, %d)\n", j+1, hc.cp[j][0], hc.cp[j][1]);
    }
}

void get_hfont(struct hfont* hf, char* font_name) {

    char fname[128];
    sprintf(fname, "fonts/%s.hf", font_name);

    strcpy(hf->name, font_name);
    hf->num_hchars = 0;

    FILE* fid = fopen(fname, "r");
    if(fid==NULL) {
        printf("Error opening Hershey font %s\n", font_name);
    }
    int i, j;
    char c;
    char cid[5];
    char nvert[3];
    char line[296];

    while(fgets(line, sizeof(line), fid) != NULL) {
        line[strlen(line)-1] = '\0'; // remove newline character
        strcpy(hf->hchars[hf->num_hchars].line, line);
        hf->num_hchars++;
    }
    fclose(fid);

//    printf("Hershey font %s has %d characters\n", hf->name, hf->num_hchars);

    for(i=0; i<hf->num_hchars; i++) {
        strncpy(cid, hf->hchars[i].line, 5);
        strncpy(nvert, &(hf->hchars[i].line[5]), 3);
        hf->hchars[i].cid = atoi(cid);
        hf->hchars[i].nvert = atoi(nvert)-1;
        hf->hchars[i].lhp = hf->hchars[i].line[8] - 'R';
        hf->hchars[i].rhp = hf->hchars[i].line[9] - 'R';
        for(j=0; j<hf->hchars[i].nvert; j++) {
            hf->hchars[i].cp[j][0] = hf->hchars[i].line[10+2*j] - 'R';
            hf->hchars[i].cp[j][1] = hf->hchars[i].line[10+2*j+1] - 'R';
        }
    }
}

void get_hstring_extent(struct hfont* hf, double hsize, char* string, int* width, int* height) {

    int nchars = strlen(string);

    *width = 0;

    int h_idx;
    int min_y = 0;
    int max_y = 0;
    for(int i=0; i<nchars; i++) {
        h_idx = (int)string[i] - 32; // assuming ascii
        *width += hsize*(hf->hchars[h_idx].rhp - hf->hchars[h_idx].lhp);
    }
    *height = max_y - min_y;
}
