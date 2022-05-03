#include "pyClient.h"

void createPod() {
    if (fork() == 0) {
        int argc = 0;
        PyClient client;
        wchar_t * argv[1];
        char* script = "createPod";
        char* extension = ".py";
        char fileSpec[strlen(script)+strlen(extension)+5];

        snprintf( fileSpec, sizeof( fileSpec ), "src/%s%s", script, extension );
        initPyClient(&client, script);

        executeScript(fileSpec, argc, argv);

        closePyClient(&client);
        _exit(0);
    }
}

void deletePods() {
    if (fork() == 0) {
        int argc = 0;
        wchar_t * argv[1];
        PyClient client;
        char* script = "deletePods";
        char* extension = ".py";
        char fileSpec[strlen(script)+strlen(extension)+5];

        snprintf( fileSpec, sizeof( fileSpec ), "src/%s%s", script, extension );
        initPyClient(&client, script);

        executeScript(fileSpec, argc, argv);
        closePyClient(&client);

        _exit(0);
    }
}

void applyManifest(char* filename) {
    if (fork() == 0) {
        wchar_t* argv[1];
        PyClient client;
        char* script = "applyFile";
        char* extension = ".py";
        char fileSpec[strlen(script)+strlen(extension)+5];

        snprintf( fileSpec, sizeof( fileSpec ), "src/%s%s", script, extension );
        initPyClient(&client, script);

        wchar_t* arg = Py_DecodeLocale(filename, NULL); 
        wchar_t* wargv[1];
        wargv[0]=arg;

        executeScript(fileSpec, 1, wargv);
        closePyClient(&client);

        _exit(0);
    }
}

int initPyClient(PyClient* client, char* script) {
    client->out = open("std-out.log", O_RDWR|O_CREAT|O_APPEND, 0600);
    if (-1 == client->out) { perror("opening std-out.log"); return 255; }

    client->err = open("std-err.log", O_RDWR|O_CREAT|O_APPEND, 0600);
    if (-1 == client->err) { perror("opening std-err.log"); return 255; }

    client->save_out = dup(fileno(stdout));
    client->save_err = dup(fileno(stderr));

    if (-1 == dup2(client->out, fileno(stdout))) { perror("cannot redirect stdout"); return 255; }
    if (-1 == dup2(client->err, fileno(stderr))) { perror("cannot redirect stderr"); return 255; }

    client->program = Py_DecodeLocale(script, NULL);
    if (client->program == NULL) {
        fprintf(stderr, "Fatal error: cannot decode argv[0]\n");
        exit(1);
    }
    Py_SetProgramName(client->program); 
    Py_Initialize();
    return 0;
}

void closePyClient(PyClient* client) {
    PyMem_RawFree(client->program);
    
    fflush(stdout); close(client->out);
    fflush(stderr); close(client->err);

    dup2(client->save_out, fileno(stdout));
    dup2(client->save_err, fileno(stderr));

    close(client->save_out);
    close(client->save_err);
}

int executeScript(char* fileSpec, int argc, wchar_t** argv) {
    FILE *fp = fopen(fileSpec,"rb");
    PySys_SetArgv(argc, argv);
    PyRun_SimpleFile(fp,fileSpec);   

    if (Py_FinalizeEx() < 0) {
        exit(120);
    }

    return 0;
}