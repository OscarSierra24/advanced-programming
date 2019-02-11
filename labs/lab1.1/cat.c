#include<sys/types.h>
#include<sys/stat.h>
#include <fcntl.h>  
#include <stdio.h>
#include <unistd.h>

/* filecopy:  copy file ifp to file ofp */
void filecopy(size_t ifile, size_t ofile){
    int size = 10000;
    int b = 0;
    char data[size];
    while((b = read(ifile, data, size)) > 0){
        write(1, data, b);
    }
}

int main(int argc, char *argv[])
{
    size_t file;
    char *prog = argv[0];

    if (argc == 1){  /* no args; copy standard input */
        filecopy(0, 1);
    }else{
        while (--argc > 0)
	        if ((file = open(*++argv, O_RDONLY)) < 0) {
                fprintf(stderr, "%s: can′t open %s\n",
			prog, *argv);
                return 1;
            } else {
                filecopy(file, 1);
                close(file);
            }
    }
    if (ferror(stdout)) {
        fprintf(stderr, "%s: error writing stdout\n", prog);
        return 2;
    }
    
    return 0;
}