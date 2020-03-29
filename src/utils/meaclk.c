//
// Created by yangjiegogogo@gmail.com on 9/3/19.
//

#include <sys/time.h>
#include <unistd.h>
#include "utils/meaclk.h"
#include "utils/debug.h"

#define MEASUREMENTS 200
#define USECSTEP 10
#define USECSTART 100

/*
 * Use linear regression to calculate cycles per microsecond.
 * */

double sample_get_cpu_mhz(void){
    struct timeval tv1,tv2;
    unsigned long start;
    double sx=0,sy=0,sxx=0,syy=0,sxy=0;
    double tx,ty;
    int i;

    long x[MEASUREMENTS];
    unsigned long y[MEASUREMENTS];
    //double a;
    double b;
    double r_2;

    for(i=0;i<MEASUREMENTS;++i){
        start=get_cycles();
        if(gettimeofday(&tv1,NULL)){
            DIE("gettimeofday failed");
        }
        do{
            if(gettimeofday(&tv2,NULL)){
                DIE("gettimeofday failed");
            }
        }while ((tv2.tv_sec-tv1.tv_sec)*1000000+(tv2.tv_usec-tv1.tv_usec)<USECSTART+i*USECSTEP);

        x[i]=(tv2.tv_sec-tv1.tv_sec)*1000000+tv2.tv_usec-tv1.tv_usec;
        y[i]=get_cycles() - start;
    }
    for(i=0;i<MEASUREMENTS;++i){
        tx=x[i];
        ty=y[i];
        sx+=tx;
        sy+=ty;
        sxx+=tx*tx;
        syy+=ty*ty;
        sxy+=tx*ty;
    }

    b=(MEASUREMENTS*sxy-sx*sy)/(MEASUREMENTS*sxx-sx*sx);
    //a=(sy-b*sx)/MEASUREMENTS;
    r_2=(MEASUREMENTS*sxy-sx*sy)*(MEASUREMENTS*sxy - sx*sy)/(MEASUREMENTS*sxx-sx*sx)/(MEASUREMENTS*syy-sy*sy);
    if(r_2<0.9){
        DIE("correlation coefficient r^2: %g<0.9",r_2);
    }
    return b;
}

double proc_get_cpu_mhz(){
    FILE * f;
    char buf[256];
    double mhz=0.0;
    f=fopen("/proc/cpuinfo","r");
    if(!f)
        return 0.0;
    while (fgets(buf,sizeof(buf),f)){
        double m;
        int rc;
        rc=sscanf(buf,"cpu MHz : %lf",&m);
        if(rc!=1){
            rc=sscanf(buf,"clock : %lf",&m);
            if(rc!=1)
                continue;
        }
        if(mhz==0.0){
            mhz=m;
            continue;
        }
        if(mhz!=m)
            DIE("conflicting CPU frequency values detected: %lf != %lf",mhz,m);
    }
    fclose(f);
    return mhz;
}

double get_cpu_mhz(){
    double sample,proc,delta;
    sample=sample_get_cpu_mhz();
    proc=proc_get_cpu_mhz();

    if(!proc || !sample)
        DIE("get cpu mhz failed");

    delta=proc>sample?proc-sample:sample-proc;
    if(delta/proc>0.01)
    {
        WRN("measured timestamp frequency %g differs from nominal %g MHz",sample,proc);
        return sample;
    }
    return proc;
}

unsigned int _get_cycles_to_nsec_scale(unsigned int tsc_frequency_khz){
    return (unsigned int)((1000000)<<10)/(unsigned int)tsc_frequency_khz;
}

/*
 * ns=cycles/(freq/ns_per_sec)
 * ns=cycles*(10^9/(cpu_khz*10^3))
 * ns=cycles*(10^9/(cpu_mhz*10^6))
 * */
unsigned long cycles_2_ns(unsigned long cycles){
    double cpu_khz=get_cpu_mhz()*1000;
    unsigned int tsc_frequency_khz=cpu_khz;
    unsigned int cycles_nsec_scale=_get_cycles_to_nsec_scale(tsc_frequency_khz);
    return (cycles*cycles_nsec_scale)>>10;
}


unsigned long ns_2_cycles(unsigned long ns){
    double cpu_khz=get_cpu_mhz()*1000;
    unsigned int tsc_frequency_khz=cpu_khz;
    unsigned int cycles_nsec_scale=_get_cycles_to_nsec_scale(tsc_frequency_khz);
    return ((ns<<10)/cycles_nsec_scale);
}

unsigned long ms_2_cycles(unsigned int ms){
    unsigned  long ns=ms*1000000;
    return ns_2_cycles(ns);
}