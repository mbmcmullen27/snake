#include "pyClient.h"

int createPod() {
    if (fork() == 0) {
        int out = open("std-out.log", O_RDWR|O_CREAT|O_APPEND, 0600);
        if (-1 == out) { perror("opening std-out.log"); return 255; }

        int err = open("std-err.log", O_RDWR|O_CREAT|O_APPEND, 0600);
        if (-1 == err) { perror("opening std-err.log"); return 255; }

        int save_out = dup(fileno(stdout));
        int save_err = dup(fileno(stderr));

        if (-1 == dup2(out, fileno(stdout))) { perror("cannot redirect stdout"); return 255; }
        if (-1 == dup2(err, fileno(stderr))) { perror("cannot redirect stderr"); return 255; }

        wchar_t *program = Py_DecodeLocale("create-pod", NULL);
        if (program == NULL) {
            fprintf(stderr, "Fatal error: cannot decode argv[0]\n");
            exit(1);
        }
        Py_SetProgramName(program); 
        Py_Initialize();

        FILE *fp = fopen("src/createPod.py","rb");
        PyRun_SimpleFile(fp,"createPod.py");   

        if (Py_FinalizeEx() < 0) {
            exit(120);
        }
        PyMem_RawFree(program);

        fflush(stdout); close(out);
        fflush(stderr); close(err);

        dup2(save_out, fileno(stdout));
        dup2(save_err, fileno(stderr));

        close(save_out);
        close(save_err);
        _exit(0);
    }

    return 0;
}