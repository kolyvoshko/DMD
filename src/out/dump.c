#include <stdio.h>
#include <stdlib.h>
#include "gd.h"
#include "log.h"
/**
   @file      dump.c
   @brief     Dump module.
   @author    Eugene Kolivoshko (ekolivoshko@gmail.com)
   @date      July 30 2015
   @version   1.0
   @copyright GNU Public License.
*/

#include "dump.h"
static void dump_file (t_pSys *pSys, t_opts *opts, t_dump *dump);
static void dump_image(t_pSys *pSys, t_opts *opts, t_dump *dump);
static void print     (t_pSys *pSys, t_opts *opts, t_dump *dump);

/** \brief Initialize the structure t_dump
 *
 * \param ini Dictionary to search
 * \return Initialized t_dump structure
 *
 */
t_dump dump_init (dictionary *ini){
    t_dump dump;
    char *s;

    /** [DumpFile] */
    dump.file.name = iniparser_getstring(ini, "DumpFile:Output", "dump");
    dump.file.snapInterv = iniparser_getint(ini, "DumpFile:Niter", 10000);
    if (dump.file.snapInterv < 1)
        ReportErrorMesage("ERROR: DumpFile:Output Niter must be natural number: src/out/dump.c dump_init");

    /** [DumpImage] */
    dump.img.name = iniparser_getstring(ini, "DumpImage:Output", "image");
    dump.img.snapInterv = iniparser_getint(ini, "DumpImage:Niter", 10000);
    if (dump.img.snapInterv < 1)
        ReportErrorMesage("ERROR: DumpImage:Output Niter must be natural number: src/out/dump.c dump_init");

    /** [DumpMovie] */
    dump.mov.name = iniparser_getstring(ini, "DumpMovie:Output", "movie");
    dump.mov.snapInterv = iniparser_getint(ini, "DumpMovie:Niter", 10000);
    if (dump.mov.snapInterv < 1)
        ReportErrorMesage("ERROR: DumpMovie:Output Niter must be natural number: src/out/dump.c dump_init");

    /** [out] */
    dump.out.snapInterv = iniparser_getint(ini, "Print:Niter", 1000);
    if (dump.out.snapInterv < 1)
        ReportErrorMesage("ERROR: Print:Niter must be natural number: src/out/dump.c dump_init");

    s = iniparser_getstring(ini, "Print:Step", "ON");
    dump.out.kIter = (strcmp(s, "ON") == 0) ? 1 : 0;

    s = iniparser_getstring(ini, "Print:Pe", "ON");
    dump.out.kPe = (strcmp(s, "ON") == 0) ? 1 : 0;

    s = iniparser_getstring(ini, "Print:Ke", "ON");
    dump.out.kKe = (strcmp(s, "ON") == 0) ? 1 : 0;

    s = iniparser_getstring(ini, "Print:E", "ON");
    dump.out.kE = (strcmp(s, "ON") == 0) ? 1 : 0;

    s = iniparser_getstring(ini, "Print:Temp", "ON");
    dump.out.kTemp = (strcmp(s, "ON") == 0) ? 1 : 0;

    return dump;
}

void dump_run (t_key *key, t_pSys *pSys, t_opts *opts, t_dump *dump){
    if (key->dump_file)     if(!(opts->thisIter % dump->file.snapInterv))    dump_file  (pSys, opts, dump);
    if (key->dump_image)    if(!(opts->thisIter % dump->img.snapInterv))     dump_image (pSys, opts, dump);
    //if (key->dump_movie)    if(!(opts->thisIter % dump->mov.snapInterv))     ;
    if (key->print)
        if(!(opts->thisIter % dump->out.snapInterv || opts->thisIter == 0))
            print (pSys, opts, dump);
}

/** \brief create text dump file */
static void dump_file(t_pSys *pSys, t_opts *opts, t_dump *dump){
    /*
    Create dump file every opts->thisIter step in dump/ directory
    */
    FILE *file;
    char fname[64];
    int i;
    sprintf(fname, "output/dump/dump%06d.dat", opts->thisIter);

    if ((file = fopen(fname,"w")) == NULL)
        ReportMessage("createDump(): File not opened.\n");
    else{
        const char *text = "# number, x, y, vx, vy, ax, ay";
        fprintf(file, "%s\n", text);
        for(i = 0; i < pSys->pCnt; i++){
           fprintf(file,"%d\t%f\t%f\t%f\t%f\t%f\t%f\n",
                   i,
                   pSys->x[i],  pSys->y[i],
                   pSys->vx[i], pSys->vy[i],
                   pSys->ax[i], pSys->ay[i]);
        }
    }
    fclose(file);
}


/** \brief create snapshot */
static void dump_image(t_pSys *pSys, t_opts *opts, t_dump *dump) {
    gdImagePtr im;
    FILE *pngsnap;
    int scale = 10,
        i,
        dkgreen, red, color;
    char fname[64];

    im = gdImageCreate(scale*pSys->lx, scale*pSys->ly);
    (void) gdImageColorAllocate(im, 223, 223, 223);
    dkgreen = gdImageColorAllocate(im, 0, 127, 0);
    red     = gdImageColorAllocate(im, 255, 0, 0);

    for(i = 0; i < pSys->pCnt; i++){
        if(pSys->atomType[i] == 0) color = dkgreen;
        if (pSys->atomType[i] == 1) color =red;

        gdImageFilledArc(im, scale*pSys->x[i], scale*pSys->y[i],
                          5, 5, 0, 360, color, gdArc);
    }
    sprintf(fname, "output/snapshot/snapshot%06d.jpg", opts->thisIter);
    pngsnap = fopen(fname, "wb");
    if(pngsnap == NULL){
        ReportMessage("ERROR: %s could not be opened!\n", fname);
    }
    //gdImagePng(im, pngsnap);
    gdImageJpeg(im, pngsnap,-1);

    fclose(pngsnap);
    gdImageDestroy(im);
}


void write_dump(t_pSys *pSys, t_opts *opts){
    // TODO: continues to development
    FILE *file;
    char fname[64];
    int dumpNumber=840;
    sprintf(fname, "output/dump/dump%06d.dat", dumpNumber);

    if ((file = fopen(fname,"r")) == NULL)
        ReportErrorMesage("ERROR: initFromDump(): File not opened.\n");
    else{
        int i;
        float x,y,z,vx,vy,vz,ax,ay,az;

        while (!feof(file)) {
            fscanf(file,"%d\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\n",
                &i,
                &x,  &y,  &z,
                &vx, &vy, &vz,
                &ax, &ay, &az );

            pSys->x[i]  = x;
            pSys->y[i]  = y;
            pSys->z[i]  = z;

            pSys->vx[i] = vx;
            pSys->vy[i] = vy;
            pSys->vz[i] = vz;

            pSys->ax[i] = ax;
            pSys->ay[i] = ay;
            pSys->az[i] = az;

            i++;
        }
    }
    fclose(file);
}

void writeTXTfile(char *name, double *dataArray, int nCount, int thisIter){
    FILE *file;
    char fname[64];
    int i;
    sprintf(fname, "output/%s%06d.dat", name, thisIter);

    if ((file = fopen(fname,"w")) == NULL)
        ReportMessage("writeTXTfile(): File not opened.\n");
    else{
        for(i = 0; i < nCount; i++)
           fprintf(file,"%f\n", dataArray[i]);
    }
    fclose(file);
}

/** \brief print to default output */
static void print (t_pSys *pSys, t_opts *opts, t_dump *dump){
    if (dump->out.kIter) ReportMessage("%d\t ", opts->thisIter);
    if (dump->out.kPe)   ReportMessage("%f\t ", pSys->ePot);
    if (dump->out.kKe)   ReportMessage("%f\t ", pSys->eKin);
    if (dump->out.kE)    ReportMessage("%f\t ", pSys->eKin + pSys->ePot);
    if (dump->out.kTemp) ReportMessage("%f\t ", pSys->temperature);
    ReportMessage("\n");
}

